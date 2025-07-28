#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DESTINATIONS 50

struct Destination {
    char name[100];
    int budgetINR;
    char country[50];
};

int loadDestinations(const char* filename, struct Destination* places) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Could not open file");
        return 0;
    }

    // Step 1: Use fseek and ftell to get file size
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);

    // Step 2: Allocate buffer for entire file content
    char* buffer = (char*)malloc(fileSize + 1);
    if (!buffer) {
        perror("Memory allocation failed");
        fclose(file);
        return 0;
    }

    // Step 3: Read whole file into buffer
    fread(buffer, 1, fileSize, file);
    buffer[fileSize] = '\0';  // Null-terminate

    fclose(file);

    // Step 4: Split into lines and parse each one
    int count = 0;
    char* line = strtok(buffer, "\n");
    while (line && count < MAX_DESTINATIONS) {
        // Parse each line (name,budget,country)
        if (sscanf(line, " %99[^,],%d,%49[^\n]",
                   places[count].name,
                   &places[count].budgetINR,
                   places[count].country) == 3) {
            count++;
        }
        line = strtok(NULL, "\n");
    }

    free(buffer);
    return count;
}

void displayFilteredDestinations(struct Destination places[], int size, int maxBudget, int* availableIndexes, int* filteredCount) {
    *filteredCount = 0;
    printf("\nPlaces within your Rupees %d, budget:\n", maxBudget);
    for (int i = 0; i < size; i++) {
        if (places[i].budgetINR <= maxBudget) {
            printf("%d. %s (%s) - Around Rupees %d\n", *filteredCount + 1, places[i].name, places[i].country, places[i].budgetINR);
            availableIndexes[*filteredCount] = i;
            (*filteredCount)++;
        }
    }
    if (*filteredCount == 0) {
        printf("Oops! No places in your budget.\n");
    }
}

void showDetails(struct Destination place) {
    printf("\nYou picked: %s\n", place.name);
    printf("Country: %s\n", place.country);
    printf("Estimated Budget: Rupees %d\n", place.budgetINR);
    printf("Enjoy your adventure!\n");
}

int main() {
    int seasonChoice, placeChoice, userBudget;
    char* seasonFiles[] = {"spring.txt", "summer.txt", "autumn.txt", "winter.txt"};
    struct Destination places[MAX_DESTINATIONS];
    int placeCount;

    printf("\n\t   Travel Places According to Choice of Season\n");

    while (1) {
        printf("\nPick a season:\n");
        printf("0. Exit \n");
        printf("1. Spring \n2. Summer \n3. Autumn \n4. Winter \n");
        printf("Enter your choice (0-4): ");
        if (scanf("%d", &seasonChoice) != 1) {
            printf("Invalid input. Exiting\n");
            break;
        }

        if (seasonChoice == 0) {
            printf("Thank you!\n");
            break;
        }

        if (seasonChoice < 1 || seasonChoice > 4) {
            printf("Invalid season\n");
            continue; 
        }

        placeCount = loadDestinations(seasonFiles[seasonChoice - 1], places);
        if (placeCount == 0) {
            printf("No destinations found\n");
            continue;
        }

        printf("Enter your max budget in Rupees: ");
        if (scanf("%d", &userBudget) != 1) {
            printf("Invalid budget input. Exiting\n");
            break;
        }

        int availableIndexes[MAX_DESTINATIONS];
        int filteredCount = 0;

        displayFilteredDestinations(places, placeCount, userBudget, availableIndexes, &filteredCount);

        if (filteredCount == 0) {
            continue; 
        }

        printf("\nWhich place do you want to know more about?\n");
        printf("Enter the number (or 0 to go back): ");
        if (scanf("%d", &placeChoice) != 1) {
            printf("Invalid input. Exiting.\n");
            break;
        }

        if (placeChoice == 0) {
            printf("Returning to season selection...\n");
            continue;
        }

        if (placeChoice < 1 || placeChoice > filteredCount) {
            printf("Invalid choice\n");
            continue;
        }

        struct Destination chosen = places[availableIndexes[placeChoice - 1]];
        showDetails(chosen);

        printf("\nWant to explore more? (1 for Yes / 0 for No): ");
        int more;
        if (scanf("%d", &more) != 1 || more == 0) {
            printf("Thank you! Safe travels!\n");
            break;
        }
    }

    return 0;
}
