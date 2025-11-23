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



