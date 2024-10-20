#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> 
#define FILENAME "medications.dat"

struct Medication {
    char name[50];
    char brand[50];
    int quantity;
    float price;
    int manufacturing_year;
    int manufacturing_month;
    int manufacturing_day;
    int expiry_year;
    int expiry_month;
    int expiry_day;
};

bool isLeapYear(int year) {
    return ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
}

bool isValidDate(int year, int month, int day) {
    if (year < 0 || month < 1 || month > 12 || day < 1)
        return false;

    int daysInMonth[] = {31, 28 + isLeapYear(year), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    return (day <= daysInMonth[month - 1]);
}

bool isExpiry(struct Medication medication) {
    int currentYear, currentMonth, currentDay;

    // Loop until a valid current date is entered
    do {
        printf("Enter current date (YYYY MM DD): ");
        scanf("%d %d %d", &currentYear, &currentMonth, &currentDay);

        if (!isValidDate(currentYear, currentMonth, currentDay)) {
            printf("Invalid date entered. Please enter a valid date.\n");
        }
    } while (!isValidDate(currentYear, currentMonth, currentDay));

    // Check if the medication has expired or is near expiry based on the current date
    if (medication.expiry_year < currentYear ||
        (medication.expiry_year == currentYear && medication.expiry_month < currentMonth) ||
        (medication.expiry_year == currentYear && medication.expiry_month == currentMonth && medication.expiry_day < currentDay)) {
        return true; // Expiry date has already passed
    } else if (medication.expiry_year == currentYear && medication.expiry_month == currentMonth && medication.expiry_day - currentDay <= 30) {
        return true; // Expiry date is near
    }

    return false; // Expiry date is not near or passed
}

void addMedication(struct Medication **medications, int *numMedications) {
    struct Medication newMedication;
    printf("Enter medication name: ");
    scanf("%s", newMedication.name);
    printf("Enter medication brand: ");
    scanf("%s", newMedication.brand);
    printf("Enter available quantity: ");
    scanf("%d", &newMedication.quantity);
    printf("Enter unit price: ");
    scanf("%f", &newMedication.price);

    // Enter manufacturing date
    do {
        printf("Enter manufacturing date (YYYY MM DD): ");
    scanf("%d %d %d", &newMedication.manufacturing_year,
                       &newMedication.manufacturing_month,
                       &newMedication.manufacturing_day);
    if (!isValidDate(newMedication.manufacturing_year, newMedication.manufacturing_month, newMedication.manufacturing_day)) {
        printf("Invalid manufacturing date entered. Please enter a valid date.\n");
    }
} while (!isValidDate(newMedication.manufacturing_year, newMedication.manufacturing_month, newMedication.manufacturing_day));

    // Enter expiry date
    do {
           printf("Enter expiry date (YYYY MM DD): ");
    scanf("%d %d %d", &newMedication.expiry_year,
                       &newMedication.expiry_month,
                       &newMedication.expiry_day);
    if (!isValidDate(newMedication.expiry_year, newMedication.expiry_month, newMedication.expiry_day)) {
        printf("Invalid expiry date entered. Please enter a valid date.\n");
    }
} while (!isValidDate(newMedication.expiry_year, newMedication.expiry_month, newMedication.expiry_day));

    if (isExpiry(newMedication)) {
        printf("Warning: Expiry date is near or has already passed for %s (%s), so you can't add it to the stock\n", newMedication.name, newMedication.brand);
    } else {
        *medications = realloc(*medications, (*numMedications + 1) * sizeof(struct Medication));
        if (*medications == NULL) {
            printf("Memory reallocation failed.\n");
            return;
        }
        (*medications)[*numMedications] = newMedication;
        (*numMedications)++;
        sleep(1);
        printf("\nMedication added to stock.\n");
    }
}

void displayStock(struct Medication *medications, int numMedications) {
    printf("Current Stock:\n");
    if (numMedications == 0) {
        printf("Your stock is empty.\n");
        return;
    }
    for (int i = 0; i < numMedications; i++) {
        printf("Name: %s\n", medications[i].name);
        printf("Brand: %s\n", medications[i].brand);
        printf("Quantity: %d\n", medications[i].quantity);
        printf("Unit Price: %.2f\n", medications[i].price);
        printf("Manufacturing Date: %d-%02d-%02d\n", medications[i].manufacturing_year,
                                                      medications[i].manufacturing_month,
                                                      medications[i].manufacturing_day);
        printf("Expiry Date: %d-%02d-%02d\n", medications[i].expiry_year,
                                                medications[i].expiry_month,
                                                medications[i].expiry_day);
        printf("\n");
    } 
}

void searchMedication(struct Medication *medications, int numMedications, char *searchQuery) {
      
    printf("Search Results:\n");
    sleep(2);
    bool found = false;
    for (int i = 0; i < numMedications; i++) {
        if (strncmp(medications[i].name, searchQuery, strlen(searchQuery)) == 0) {
            printf("Name: %s\n", medications[i].name);
            printf("Brand: %s\n", medications[i].brand);
            printf("Quantity: %d\n", medications[i].quantity);
            printf("Unit Price: %.2f\n", medications[i].price);
            printf("Manufacturing Date: %d-%02d-%02d\n", medications[i].manufacturing_year,
                                                          medications[i].manufacturing_month,
                                                          medications[i].manufacturing_day);
            printf("Expiry Date: %d-%02d-%02d\n", medications[i].expiry_year,
                                                    medications[i].expiry_month,
                                                    medications[i].expiry_day);
            printf("\n");
            found = true;
        }
    }
    if (!found) {
        printf("No medications found matching the search query.\n");
    }
}

void updateQuantity(struct Medication *medications, int numMedications, char *medicationName, int newQuantity) {
    bool found = false;
    for (int i = 0; i < numMedications; i++) {
        if (strcmp(medications[i].name, medicationName) == 0) {
            medications[i].quantity = newQuantity;
            printf("Quantity updated for %s (%s).\n", medications[i].name, medications[i].brand);
            found = true;
            break;
        }
    }
    if (!found) {
        printf("Medication not found in stock.\n");
    }
}

void deleteMedication(struct Medication **medications, int *numMedications, char *medicationName) {
    bool found = false;
    for (int i = 0; i < *numMedications; i++) {
        if (strcmp((*medications)[i].name, medicationName) == 0) {
            // Shift elements to remove the medication from the array
            for (int j = i; j < (*numMedications - 1); j++) {
                (*medications)[j] = (*medications)[j + 1];
            }
            *medications = realloc(*medications, (*numMedications - 1) * sizeof(struct Medication));
            if (*medications == NULL && *numMedications > 1) {
                printf("Memory reallocation failed.\n");
                return;
            }
            (*numMedications)--;
            printf("Medication %s removed from stock.\n", medicationName);
            found = true;
            break;
        }
    }
    if (!found) {
        printf("Error: %s not found in stock. Please enter the correct name.\n", medicationName);
    }
}

void loadMedicationsFromFile(struct Medication **medications, int *numMedications) {
    FILE *file = fopen(FILENAME, "rb");
    if (file == NULL) {
        printf("Failed to open file for reading.\n");
        return;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);

    int numRecords = fileSize / sizeof(struct Medication);
    *medications = malloc(numRecords * sizeof(struct Medication));
    if (*medications == NULL) {
        printf("Memory allocation failed.\n");
        fclose(file);
        return;
    }

    fread(*medications, sizeof(struct Medication), numRecords, file);
    *numMedications = numRecords;

    fclose(file);
}

void saveMedicationsToFile(struct Medication *medications, int numMedications) {
    FILE *file = fopen(FILENAME, "wb");
    if (file == NULL) {
        printf("Failed to open file for writing.\n");
        return;
    }

    fwrite(medications, sizeof(struct Medication), numMedications, file);

    fclose(file);
}

int main() {
    struct Medication *medications = NULL;
    int numMedications = 0;
    int choice;
    loadMedicationsFromFile(&medications, &numMedications);
    printf("    WELCOME TO AKRAM & ABDELMADJID PROGRAM :)       \n");
    do {
        printf("\n what do you want to perform ?:\n");
        printf("1. Add a medication\n");
        printf("2. Display current stock\n");
        printf("3. Search for a medication\n");
        printf("4. Update quantity of a medication\n");
        printf("5. Delete a medication from stock\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addMedication(&medications, &numMedications);
                saveMedicationsToFile(medications, numMedications);
                break;
            case 2:
                displayStock(medications, numMedications);
                break;
            case 3: {
                char searchQuery[50];  
                if (numMedications == 0) 
                printf("Your stock is empty.\n");
                else {printf("Enter the first characters of the medication name: ");
                     scanf("%s", searchQuery);
                     searchMedication(medications, numMedications, searchQuery);
                 }
                break;
            }
            case 4: {
                char medicationName[50];
                int newQuantity;
                if (numMedications == 0) 
                printf("Your stock is already empty.\n");
                else { printf("Enter the name of the medication to update quantity: ");
                     scanf("%s", medicationName);
                     printf("Enter the new quantity: ");
                     scanf("%d", &newQuantity);
                     updateQuantity(medications, numMedications, medicationName, newQuantity);
                     saveMedicationsToFile(medications, numMedications);
                 }
                break;
            }
            case 5: {
                char medicationName[50];
                if (numMedications == 0) 
                printf("Your stock is already empty.\n");
                else {printf("Enter the name of the medication to delete: ");
                    scanf("%s", medicationName);
                    deleteMedication(&medications, &numMedications, medicationName);
                    saveMedicationsToFile(medications, numMedications);
                 }
                break;
            }
            case 6:
                printf("Exiting, see you again </> \n");
                break;
            default:
                printf("Invalid choice. \n");
        }
        sleep(1);
    } while (choice != 6);

    free(medications);

    return 0;
} 