#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure definition
typedef struct {
    int code;
    char name[50];
    float price;
    int quantity;
} Product;

// Global inventory
Product **products = NULL;
int num_products = 0;
int max_products = 100;

// Initialize inventory
void initProducts() {
    products = malloc(max_products * sizeof(Product *));
    if (products == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    num_products = 0;
}

// Resize product array
void resizeProducts() {
    max_products *= 2;
    Product **temp = realloc(products, max_products * sizeof(Product *));
    if (temp == NULL) {
        fprintf(stderr, "Memory reallocation failed\n");
        exit(1);
    }
    products = temp;
}

// Add product
void addProduct() {
    if (num_products >= max_products) {
        resizeProducts();
    }

    products[num_products] = malloc(sizeof(Product));
    if (products[num_products] == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    printf("Enter code: ");
    if (scanf("%d", &products[num_products]->code) != 1) {
        printf("Invalid input.\n");
        free(products[num_products]);
        return;
    }

    // Check for duplicate code
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
    if (scanf("%f", &products[num_products]->price) != 1) {
        printf("Invalid input.\n");
        free(products[num_products]);
        return;
    }

    printf("Enter quantity: ");
    if (scanf("%d", &products[num_products]->quantity) != 1) {
        printf("Invalid input.\n");
        free(products[num_products]);
        return;
    }

    num_products++;
    printf("Product added.\n");
}

// Find product index by code
int findProductIndex(int code) {
    for (int i = 0; i < num_products; i++) {
        if (products[i]->code == code) {
            return i;
        }
    }
    return -1;
}

// Remove product
void removeProduct() {
    int code;
    printf("Enter code to remove: ");
    if (scanf("%d", &code) != 1) {
        printf("Invalid input.\n");
        return;
    }

    int idx = findProductIndex(code);
    if (idx == -1) {
        printf("Not found.\n");
        return;
    }

    free(products[idx]);

    // Shift products array
    for (int i = idx; i < num_products - 1; i++) {
        products[i] = products[i + 1];
    }

    num_products--;
    printf("Removed.\n");
}

// Display all products
void displayAllProducts() {
    if (num_products == 0) {
        printf("Empty inventory.\n");
        return;
    }
    
    printf("%-5s %-20s %-10s %-8s\n", "Code", "Name", "Price", "Qty");
    printf("---------------------------------------------------\n");

    for (int i = 0; i < num_products; i++) {
        printf("%-5d %-20s %-10.2f %-8d\n",
            products[i]->code,
            products[i]->name,
            products[i]->price,
            products[i]->quantity);
    }
}

// Save inventory to file
void saveInventory() {
    FILE *fp = fopen("inventory.dat", "wb");
    if (fp == NULL) {
        fprintf(stderr, "Failed to open file for writing.\n");
        return;
    }

    fwrite(&num_products, sizeof(int), 1, fp);

    for (int i = 0; i < num_products; i++) {
        fwrite(products[i], sizeof(Product), 1, fp);
    }

    fclose(fp);
    printf("Inventory saved successfully.\n");
}

// Load inventory from file
void loadInventory() {
    FILE *fp = fopen("inventory.dat", "rb");
    if (fp == NULL) {
        printf("No existing inventory file found.\n");
        return;
    }

    // Free existing products
    for (int i = 0; i < num_products; i++) {
        free(products[i]);
    }

    // Read number of products
    if (fread(&num_products, sizeof(int), 1, fp) != 1) {
        fprintf(stderr, "Failed to read inventory.\n");
        fclose(fp);
        num_products = 0;
        return;
    }

    // Ensure enough space
    while (num_products > max_products) {
        resizeProducts();
    }

    // Load products
    for (int i = 0; i < num_products; i++) {
        products[i] = malloc(sizeof(Product));
        if (products[i] == NULL) {
            fprintf(stderr, "Memory allocation failed during load.\n");
            num_products = i;
            fclose(fp);
            return;
        }
        if (fread(products[i], sizeof(Product), 1, fp) != 1) {
            fprintf(stderr, "Failed to read product data.\n");
            free(products[i]);
            num_products = i;
            fclose(fp);
            return;
        }
    }

    fclose(fp);
    printf("Inventory loaded successfully.\n");
}

// Free all allocated memory
void freeInventory() {
    for (int i = 0; i < num_products; i++) {
        free(products[i]);
    }
    free(products);
    products = NULL;
    num_products = 0;
}