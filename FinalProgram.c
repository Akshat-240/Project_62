#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure definition
typedef struct {
    int code;
    char name[50];
    float price;
    int quantity;
    int categoryId;
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

/*******************************
 CATEGORY SYSTEM
*******************************/

typedef struct {
    int id;
    char name[30];
} Category;

Category categories[20];
int categoryCount = 0;

void addCategory() {
    printf("Enter category ID: ");
    scanf("%d", &categories[categoryCount].id);

    printf("Enter category name: ");
    scanf(" %[^\n]", categories[categoryCount].name);

    categoryCount++;
    printf("Category added.\n");
}

void listCategories() {
    printf("\n--- Categories ---\n");
    for (int i = 0; i < categoryCount; i++) {
        printf("%d. %s\n", categories[i].id, categories[i].name);
    }
}

void filterByCategory() {
    int id;
    printf("Enter category ID: ");
    scanf("%d", &id);

    printf("\nProducts in category %d:\n", id);
    for (int i = 0; i < num_products; i++) {
        if (products[i]->categoryId == id) {
            printf("%s (Code %d)\n", products[i]->name, products[i]->code);
        }
    }
}

/*******************************
 PRODUCT EDIT FEATURE
*******************************/

void editProduct() {
    int code;
    printf("Enter product code: ");
    scanf("%d", &code);

    int idx = -1;
    for (int i = 0; i < num_products; i++)
        if (products[i]->code == code)
            idx = i;

    if (idx == -1) {
        printf("Not found.\n");
        return;
    }

    printf("1. Edit name\n");
    printf("2. Edit price\n");
    printf("3. Edit quantity\n");
    printf("Choose: ");
    int ch;
    scanf("%d", &ch);

    if (ch == 1) {
        printf("New name: ");
        scanf(" %[^\n]", products[idx]->name);
    } else if (ch == 2) {
        printf("New price: ");
        scanf("%f", &products[idx]->price);
    } else if (ch == 3) {
        printf("New quantity: ");
        scanf("%d", &products[idx]->quantity);
    }

    printf("Updated.\n");
}

/*******************************
 EXPIRY DATE SYSTEM
*******************************/

typedef struct {
    int day, month, year;
} Date;

Date expiry[200];

void setExpiry() {
    int code;
    printf("Enter product code: ");
    scanf("%d", &code);

    int idx = -1;
    for (int i = 0; i < num_products; i++)
        if (products[i]->code == code)
            idx = i;

    if (idx == -1) {
        printf("Not found.\n");
        return;
    }

    printf("Enter expiry date (dd mm yyyy): ");
    scanf("%d %d %d", &expiry[idx].day, &expiry[idx].month, &expiry[idx].year);

    printf("Expiry set.\n");
}

void checkExpired() {
    printf("\n--- Expired Items ---\n");

    for (int i = 0; i < num_products; i++) {
        if (expiry[i].year != 0) {
            printf("Product: %s expiring on %d/%d/%d\n",
                   products[i]->name,
                   expiry[i].day, expiry[i].month, expiry[i].year);
        }
    }
}

/*******************************
 SORTING FEATURES
*******************************/

void sortByPrice() {
    for (int i = 0; i < num_products - 1; i++) {
        for (int j = i + 1; j < num_products; j++) {
            if (products[i]->price > products[j]->price) {
                Product *temp = products[i];
                products[i] = products[j];
                products[j] = temp;
            }
        }
    }
    printf("Sorted by price.\n");
}

void extrasMenu() {
    int ch;

    printf("\n--- EXTRA FEATURES ---\n");
    printf("1. Add Category\n");
    printf("2. List Categories\n");
    printf("3. Filter Products by Category\n");
    printf("4. Edit Product\n");
    printf("5. Set Expiry Date\n");
    printf("6. View Expiry List\n");
    printf("7. Sort by Price\n");
    printf("8. Back\n");

    printf("Enter choice: ");
    scanf("%d", &ch);

    switch (ch) {
        case 1: addCategory(); break;
        case 2: listCategories(); break;
        case 3: filterByCategory(); break;
        case 4: editProduct(); break;
        case 5: setExpiry(); break;
        case 6: checkExpired(); break;
        case 7: sortByPrice(); break;
        default: return;
    }
}

int main() {
    initProducts();
    loadInventory();
    
    int choice;
    
    while (1) {
        printf("\n========== INVENTORY MANAGEMENT AND BILLING SYSTEM ==========\n");
        printf("1. Add Product\n");
        printf("2. Remove Product\n");
        printf("3. Display All Products\n");
        printf("4. Add to Cart\n");
        printf("5. View Cart\n");
        printf("6. Remove from Cart\n");
        printf("7. Update Cart Quantity\n");
        printf("8. Generate Invoice\n");
        printf("9. Daily Sales Report\n");
        printf("10. Extra Features\n");
        printf("11. Save & Exit\n");
        printf("Enter choice: ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            while (getchar() != '\n');
            continue;
        }
        
        switch (choice) {
            case 1: addProduct(); break;
            case 2: removeProduct(); break;
            case 3: displayAllProducts(); break;
            case 4: {
                int code, qty;
                printf("Enter product code: ");
                scanf("%d", &code);
                printf("Enter quantity: ");
                scanf("%d", &qty);
                addToCart(code, qty);
                break;
            }
            case 5: viewCart(); break;
            case 6: removeFromCart(); break;
            case 7: updateCartQuantity(); break;
            case 8: generateInvoice(); break;
            case 9: dailySalesReport(); break;
            case 10: extrasMenu(); break;
            case 11:
                saveInventory();
                freeInventory();
                printf("Data saved. Goodbye!\n");
                return 0;
            default:
                printf("Invalid choice.\n");
        }
    }
    
    return 0;
}



