*Note(Please view it in code format instead of preview for better clarity and presentation)

Grocery Management System (C Project)
***************************************
A console-based inventory and billing system written in C, designed for multi-module collaboration.
This project demonstrates file handling, structures, dynamic memory, modular programming, billing logic, category filtering, sorting, and UI formatting with ANSI colors.

Project Objective
********************
1.The objective of this project is to develop a fully functional Grocery Management System that allows:
2.Adding, removing, editing products
3.Managing stock with dynamic memory
4.Categorizing items and filtering them
5.Adding items to cart
6.Generating invoices with tax & discounts
7.Saving invoices to text files
8.Saving & loading inventory using binary files
9.Implementing expiry date checking
10.Providing colored UI for better interaction

Development Steps
1. Planning & Module Division
---------------------------------
The project is divided into 5 files to support four collaborators:

File	   Responsibilities
-----      ----------------
core.c	   Product inventory, saving, loading, searching
main.c	   Main controller, menu logic, integration
ui.c	   Colored menu, UI rendering
billing.c  Cart, invoice, sales report, invoice saving
extras.c   Categories, expiry date, sorting, edit product

2. Data Structures Used
--------------------------
Product structure for inventory items
CartItem for billing
Category management
Date for expiry
Dynamic array of Product*
Arrays for categories, expiry, cart

3. Core Functionalities
--------------------------
Add/remove/display/search product
Save/load inventory using binary file
Add to cart, update cart, remove from cart
Generate invoice (includes discount & tax)
Save invoice to text file
Daily sales report
Category system
Expiry date assignment
Sorting by price

4. File Handling
-----------------
Inventory is saved in inventory.dat
Invoices saved as:
invoice_<ID>.txt

5. UI & Experience
-------------------
ANSI colored menus
Screen clearing
Structured output formatting

6. Team Contributions 
--------------------
Team Member	              Module	      

Akshat Nigam	          Core System & Main function     
Swati Shukla	          User Interface	         
Souvik Das	              Billing System	
Arijit Dey	              Extra Features	

7. Conclusion
---------------
This project demonstrates:
Team-based development
File handling
Dynamic memory allocation
Multi-module C programming
Real-world billing logic
Clean UI design in the terminal
