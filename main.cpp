#include <iostream>
#include <conio.h>
#include <stdio.h>
#include <process.h>
#include <fstream>
using namespace std;

class product
{
public:
    int product_no;
    char name[50];
    float price, quantity, tax, discount;            //data memberes of product

    void create_product()                            //function where products were entered by admin.
    {
        cout << "\nProduct No. of The Product : ";
        cin >> product_no;
        cout << "\n\nName of The Product : ";
        cin >> gets(name);                          //gets(name) is displaying the name
        cout << "\nPrice of The Product : ";
        cin >> price;
        cout << "\nDiscount (%) : ";
        cin >> discount;
    }

    void show_product()                              //This function is used to show all the product details.
    {
        cout << "\nProduct No. of The Product : " << product_no;
        cout << "\nName of The Product : ";
        puts(name);                                 //input name of the product with the help of puts.
        cout << "\nPrice of The Product : " << price;
        cout << "\nDiscount : " << discount << "%";
    }

}; // class ends here

// global declaration for stream object, object.
fstream fp;                                              //fstream object created so that we can read or write in file "shop.dat"
product pr;                                              //object of class to define product

// function to write in file
void write_product()                                      //admin can create product with the help of this function
{
    fp.open("Shop.dat", ios::out | ios::app);
    pr.create_product();
    fp.write((char *)&pr, sizeof(product));               //With the help of pointer to char we are writing on this file(creating products)
    fp.close();
    cout << "\n\nProduct Has Been Created !";
    getch();                                              //we can hold the output screen with this function
}

// function to read all records from file
void display_all()                                         //This function is used to display all the data in an invoice.
{

    cout << "\n\n\n\t\tDisplay All Records !!!\n\n";
    fp.open("Shop.dat", ios::in);                           //Opening the "shop.dat" file 
    while (fp.read((char *)&pr, sizeof(product)))           //With the help of pointer to char we are reading the file
    {
        pr.show_product();
        cout << "\n\n====================================\n";
        getch();
    }
    fp.close();                                              //closing the "shop.dat" file after use
    getch();                                                 //getch() is used to hold the screen
}

// function to read specific record from file
void display_sp(int n)
{
    int flag = 0;
    fp.open("Shop.dat", ios::in);                            //opening file "shop.dat" in ios::in mode which allows to give input
    while (fp.read((char *)&pr, sizeof(product)))            //using pointer to char reading the file"shop.dat" where our file is getting stored
    {
        if (pr.product_no == n)                              //check if productno matches with n
        {

            pr.show_product();                                //calling show function 
            flag = 1;
        }
    }
    fp.close();
    if (flag == 0)                                             //checking if record exists or not
        cout << "\n\nrecord not exist";
    getch();                                                    //getch() is used to hold the screen
}

// function to modify record of file
void modify_product()
{
    int no, found = 0;                             

    cout << "\n\n\tTo Modify ";
    cout << "\n\n\tEnter Product No. of The Product";
    cin >> no;
    fp.open("Shop.dat", ios::in | ios::out);                               //opening the "shop.dat" file to write or modify the product
    while (fp.read((char *)&pr, sizeof(product)) && found == 0)            //here we are reading the file with fstream fp(declaread globally in line 39)
    {
        if (pr.product_no == no)
        {
            pr.show_product();                                              //show product functiion is called
            cout << "\nEnter The New Details of Product" << endl;
            pr.create_product();                                            //function calling for creating a product
            int pos = -1 * sizeof(pr);                                      //defining the position so that we can seek through it easily
            fp.seekp(pos, ios::cur);                                        //The seekp(pos) method  is used to set the position of the pointer in the output sequence with the specified position.
            fp.write((char *)&pr, sizeof(product));
            cout << "\n\n\t Record Updated";
            found = 1;
        }
    }
    fp.close();                                                              //closing of file
    if (found == 0)                                                          //checking if the file is empty 
        cout << "\n\n Record Not Found ";                                    //if the file is empty diplay "record not found"
    getch();                                                                 //using getch() we are holding the output screen
}

// function to delete record of file
void delete_product()
{
    int no;

    cout << "\n\n\n\tDelete Record";
    cout << "\n\nEnter Product no. of The Product you want to Delete : ";
    cin >> no;
    fp.open("Shop.dat", ios::in | ios::out);   //opening the "shop.dat" file in read mode so that we can check and delete the uneanted product.
    fstream fp2;
    fp2.open("Temp.dat", ios::out);            //opening other file "temp.dat" where we will add the deleted product
    fp.seekg(0, ios::beg);                     //seekg() is a function in the iostream library that allows us to seek an arbitrary position in a file
    while (fp.read((char *)&pr, sizeof(product))) //reading the file "shop.dat" using pointer to  char.
    {
        if (pr.product_no != no)                         //checking if the product_no input(given by user) and product_no in file are same.
        { 
            fp2.write((char *)&pr, sizeof(product));      //here only we added the deletd the data to other file named as "temp.dat"
        }
    }
    fp2.close();                                 //closed the "temp.dat" file.
    fp.close();                                  //closed the "shop.dat" file.
    remove("Shop.dat");
    rename("Temp.dat", "Shop.dat");
    cout << "\n\n\tRecord Deleted ..";
    getch();                                     //getch is used to hold the output screen
}

// function to display all products price list
void menu()
{

    fp.open("Shop.dat", ios::in);                           //opened the shop file
    if (!fp)
    {
        cout << "ERROR!!! FILE COULD NOT BE OPEN\n\n\n Go To Admin Menu to create File ";

        cout << "\n\n\n Program is closing ....";
        getch();
    }

    cout << "\n\n\t\tProduct MENU\n\n";
    cout << "====================================================\n";
    cout << "P.NO.\t\tNAME\t\tPRICE\n";
    cout << "====================================================\n";

    while (fp.read((char *)&pr, sizeof(product)))
    {
        cout << pr.product_no << "\t\t" << pr.name << "\t\t" << pr.price << endl;
    }
    fp.close();                                              //closing the shop.dat file after use
}

// function to place order and generating bill for Products

void place_order()                                           //in this function we are taking the orders from customer.
{
    int order_arr[50], quan[50], c = 0;                      //we are storing the data in arryas 
    float amt, damt, total = 0;
    char ch = 'Y';
    menu();                                                  //menu function is called
    cout << "\n============================";
    cout << "\n PLACE YOUR ORDER";
    cout << "\n============================\n";
    do                                                      //using do while function
    {
        cout << "\n\nProduct No. Of The Product : ";
        cin >> order_arr[c];
        cout << "\nQuantity in number : ";
        cin >> quan[c];
        c++;
        cout << "\nDo you want to add another Product ? (y/n) : ";
        cin >> ch;
    } while (ch == 'y' || ch == 'Y');
    cout << "\n\nThank You For Placing The Order";
    getch();                                                          //getch() is used to hold the screen for output

    cout << "\n\n******************************** INVOICE ************************\n";
    cout << "\nPr No.\tPr Name\tQuantity \tPrice \tAmount \tAmount after discount \n ";

    for (int x = 0; x <= c; x++)                                     //creating a loop where we will print the invoice or all details. 
    {
        fp.open("Shop.dat", ios::in);
        fp.read((char *)&pr, sizeof(product));
        while (!fp.eof())                                             //(.eof menas end of file) while will run till end of file
        {
            if (pr.product_no == order_arr[x])
            {
                amt = pr.price * quan[x];
                damt = amt - (amt * pr.discount / 100);
                cout << "\n"
                     << order_arr[x] << "\t" << pr.name << "\t" << quan[x] << "\t\t" << pr.price << "\t" << amt << "\t\t" << damt;
                total += damt;
            }
            fp.read((char *)&pr, sizeof(product));
        }

        fp.close();
    }
    cout << "\n\n\t\t\t\t\tTOTAL = " << total;
    getch();
}

// ADMINSTRATOR MENU FUNCTION
void admin_menu()                                   //this is admin menu function where admin can perform opertaions like create,delete,modify product etc.
{
    char ch2;
    cout << "\n\n\n\tADMIN MENU";
    cout << "\n\n\t1.CREATE PRODUCT";
    cout << "\n\n\t2.DISPLAY ALL PRODUCTS";
    cout << "\n\n\t3.MODIFY PRODUCT";
    cout << "\n\n\t4.DELETE PRODUCT";
    cout << "\n\n\t5.VIEW PRODUCT MENU";
    cout << "\n\n\t6.BACK TO MAIN MENU";
    cout << "\n\n\tEnter Your Choice :";
    ch2 = getche(); 
    switch (ch2)                                    //We are making the menu using switch case operation where case is character type.
    {
    case '1':
        write_product();                            //in case 1 write product function is getting called so that admin can create new product
        break;
    case '2':
        display_all();                              //case 2 is calling the display product function so that admin can display it. 
        break;
    case '3':
        modify_product();
        break;
    case '4':
        delete_product();
        break;
    case '5':
        menu();
        getch();
    case '6':
        break;
    default:
        cout << "\a";
        admin_menu();
    }
}

// THE MAIN FUNCTION OF PROGRAM
int main()
{
    char ch;
    do                                                    //using do while loop   
    {
        cout << "\n\n\n\tMAIN MENU";
        cout << "\n\n\t01. CUSTOMER";
        cout << "\n\n\t02. ADMINISTRATOR";
        cout << "\n\n\t03. EXIT";
        cout << "\n\n\t Enter your Choice : ";

        ch = getche();//getche() function  which waits for any character input from keyboard and it will also echo the input character on to the output screen.            


        switch (ch)                                 //using switch case method for menu purpose
        {
        case '1':
            place_order();                           //in case 1 place order function will get called
            getch();
            break;
        case '2':
            admin_menu();                            //in case 2 admin_menu function will get called
            break;
        case '3':
            return 0;
        default:
            cout << "\a";
        }
    } while (ch != '3');                               //here we are giving this condition because we want an exit at ch==3.
}