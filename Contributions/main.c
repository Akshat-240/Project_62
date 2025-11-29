#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//structure
typedef struct {
    int code;
    char name[50];
    float price;
    int quantity;
} Product;

//global inventory

Product **products = NULL;
int num_products = 0;
int max_products = 100;

//Intialize inventory
void initProducts() {
    products = malloc(max_products * sizeof(Product *));
    num_products = 0;
}

// Resize product array
void resizeProducts() {
    max_products *= 2;
    products = realloc(products, max_products * sizeof(Product *));
}

// Add product
void addProduct() {
    if (num_products >= max_products)
        resizeProducts();

    products[num_products] = malloc(sizeof(Product));

    printf("Enter code: ");
    scanf("%d", &products[num_products]->code);

    for (int i = 0; i < num_products; i++) {
        if (products[i]->code == products[num_products]->code) {
            printf("Duplicate code.\n");
            free(products[num_products]);
            return;
        }
    }

    printf("Enter name: ");
    scanf(" %[^\n]", products[num_products]->name);

    printf("Enter price: ");
    scanf("%f", &products[num_products]->price);

    printf("Enter quantity: ");
    scanf("%d", &products[num_products]->quantity);

    num_products++;
    printf("Product added.\n");
}

int findProductIndex(int code) {
    for (int i = 0; i< num_products; i++){
        if(products[i]->code == code){
            return i;
        }
    }
    return -1;
}

void removeProduct(){
    int code;
    printf("Enter code to remove: ");
    scanf("%d", &code);

    int idx = findProductIndex(code);
    if(idx == -1) {
        printf("Not found.\n");
        return;
    }

    free(products[idx]);

    for(int i = idx; i < num_products-1; i++)
    products[i] = products[i+1];

    num_products--;
    printf("Removed.\n");

}

void displayAllProducts() {
    if(num_products == 0){
        printf("Empty inventory.\n");
        return;
    }
    printf("%-5s %-20s %-10s %-8s\n", "Code", "Name", "Price", "Qty");

    for(int i = 0; i < num_products; i++) {
        printf("%-5d %-20s %-10.2f %-8d\n",
            products[i]->code,
            products[i]->name,
            products[i]->price,
            products[i]->quantity);
    }
}

//save inventory

void saveInventory(){
    FILE *fp = fopen("inventory.dat", "wb");
    fwrite(&num_products, sizeof(int), 1, fp);

    for (int i = 0; i < num_products; i++)
        fwrite(products[i], sizeof(Product), 1, fp);

    fclose(fp);
}

//load inventory

void loadInventory(){
    FILE *fp = fopen("inventory.dat", "rb");
    if (!fp) return;

    for(int i =0; i < num_products;i++){
        free(products[i]);
    }
    
    fread(&num_products, sizeof(int), 1, fp);
    while (num_products > max_products){
    resizeProducts();
    }
    for(int i = 0; i< num_products;i++){
        products[i] = malloc(sizeof(Product));
        fread(products[i],sizeof(Product), 1 ,fp);
    }
    fclose(fp);
    printf("Inventory loaded succesfully.\n");
}

#include <stdio.h>

typedef struct {
    int code;
    int qty;
    float total;
} CartItem;

CartItem cart[50];
int cart_size = 0;

int findProductIndex(int code); // from core.c
extern Product **products;

void initCart() { cart_size = 0; }

void addToCart(int code, int qty) {
    int idx = findProductIndex(code);
    if (idx == -1) {
        printf("Product not found.\n");
        return;
    }

    if (products[idx]->quantity < qty) {
        printf("Not enough stock.\n");
        return;
    }

    cart[cart_size].code = code;
    cart[cart_size].qty = qty;
    cart[cart_size].total = products[idx]->price * qty;
    cart_size++;

    printf("Added to cart.\n");
}

void generateInvoice() {
    float subtotal = 0;

    printf("\n--- INVOICE ---\n");
    printf("%-5s %-10s %-10s\n", "Code", "Qty", "Total");

    for (int i = 0; i < cart_size; i++) {
        printf("%-5d %-10d %-10.2f\n",
            cart[i].code, cart[i].qty, cart[i].total);
        subtotal += cart[i].total;

        int idx = findProductIndex(cart[i].code);
        products[idx]->quantity -= cart[i].qty;
    }

    float tax = subtotal * 0.10;
    float discount = (subtotal > 500) ? subtotal * 0.05 : 0;
    float final = subtotal + tax - discount;

    printf("\nSubtotal: %.2f\n", subtotal);
    printf("Tax: %.2f\n", tax);
    printf("Discount: %.2f\n", discount);
    printf("Final: %.2f\n", final);

    cart_size = 0;
}



