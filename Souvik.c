#include <stdio.h>
#include <stdlib.h>
#include <time.h>

extern int findProductIndex(int code);
extern Product **products;

typedef struct {
    int code;
    int qty;
    float total;
} CartItem;

CartItem cart[100];
int cart_size = 0;
int invoiceNumber = 1;
float todaySales = 0;

void initCart() { cart_size = 0; }

void viewCart() {
    if (cart_size == 0) {
        printf("Cart is empty.\n");
        return;
    }

    printf("\n----- CART ITEMS -----\n");
    printf("%-5s %-8s %-10s\n", "Code", "Qty", "Total");

    for (int i = 0; i < cart_size; i++) {
        printf("%-5d %-8d %-10.2f\n",
               cart[i].code,
               cart[i].qty,
               cart[i].total);
    }
}

void removeFromCart() {
    if (cart_size == 0) {
        printf("Cart empty.\n");
        return;
    }

    int code;
    printf("Enter code to remove: ");
    scanf("%d", &code);

    for (int i = 0; i < cart_size; i++) {
        if (cart[i].code == code) {
            for (int j = i; j < cart_size - 1; j++)
                cart[j] = cart[j + 1];
            cart_size--;
            printf("Removed from cart.\n");
            return;
        }
    }

    printf("Code not found in cart.\n");
}

void updateCartQuantity() {
    if (cart_size == 0) {
        printf("Cart empty.\n");
        return;
    }

    int code, qty;
    printf("Enter code: ");
    scanf("%d", &code);

    for (int i = 0; i < cart_size; i++) {
        if (cart[i].code == code) {
            printf("Enter new quantity: ");
            scanf("%d", &qty);

            int idx = findProductIndex(code);
            if (products[idx]->quantity < qty) {
                printf("Insufficient stock.\n");
                return;
            }

            cart[i].qty = qty;
            cart[i].total = qty * products[idx]->price;

            printf("Quantity updated.\n");
            return;
        }
    }

    printf("Item not found.\n");
}

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
    cart[cart_size].total = qty * products[idx]->price;
    cart_size++;

    printf("Added to cart.\n");
}

void saveInvoiceToFile(float subtotal, float tax, float discount, float final) {
    char filename[50];
    sprintf(filename, "invoice_%d.txt", invoiceNumber);

    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Error saving invoice.\n");
        return;
    }

    fprintf(fp, "----------- INVOICE #%d -----------\n", invoiceNumber);
    for (int i = 0; i < cart_size; i++) {
        fprintf(fp, "Code: %d  Qty: %d  Total: %.2f\n",
                cart[i].code, cart[i].qty, cart[i].total);
    }

    fprintf(fp, "\nSubtotal: %.2f\nTax: %.2f\nDiscount: %.2f\nFinal: %.2f\n",
            subtotal, tax, discount, final);

    fclose(fp);
    printf("Invoice saved as %s\n", filename);
}

void generateInvoice() {
    if (cart_size == 0) {
        printf("Cart empty.\n");
        return;
    }

    float subtotal = 0;

    printf("\n----- INVOICE #%d -----\n", invoiceNumber);

    for (int i = 0; i < cart_size; i++) {
        int idx = findProductIndex(cart[i].code);
        printf("Code: %d  Qty: %d  Total: %.2f\n",
               cart[i].code, cart[i].qty, cart[i].total);

        subtotal += cart[i].total;
        products[idx]->quantity -= cart[i].qty;
    }

    float tax = subtotal * 0.10;
    float discount = (subtotal > 500) ? subtotal * 0.05 : 0;
    float final = subtotal + tax - discount;

    printf("\nSubtotal: %.2f\nTax: %.2f\nDiscount: %.2f\nFinal: %.2f\n",
           subtotal, tax, discount, final);

    todaySales += final;

    saveInvoiceToFile(subtotal, tax, discount, final);

    invoiceNumber++;
    cart_size = 0;
}

void dailySalesReport() {
    printf("\n--- DAILY SALES REPORT ---\n");
    printf("Total Sales Today: %.2f\n", todaySales);
}


 