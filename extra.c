#include <stdio.h>
#include <string.h>

extern Product **products;
extern int num_products;

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



