#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/* IsValidSpell checks if the chosen spell is valid according to the two parameters,
    spell represents the string chosen and last spell is the string representing the last spell casted.
    The function compares the first character of the current spell (spell[0]) to the last character of the last spell (lastSpell[strlen(lastSpell) - 1])
    If they match (invalid spell), it returns 1 else it returns 0
*/
int isValidSpell(char* spell, char* lastSpell) {
    if (spell[0] != lastSpell[strlen(lastSpell) - 1]) {
        return 0; // Invalid spell
    }
    return 1; // Valid spell
}

int main() {
    srand(time(NULL)); // Seed for random number generation

    char spells[100][100]; // Array to store spells
    int numofspells; // Number of spells
    FILE *spellsp; // File pointer to interact with given file
    spellsp = fopen("spells.txt", "r"); // Open file containing spells

    if (spellsp == NULL) {
        perror("File was not opened ");
        return 1; // Return 1 to indicate error in opening file
    }

    fscanf(spellsp, "%d", &numofspells); // Read number of spells from file

    for (int i = 0; i < numofspells; i++) {
        fscanf(spellsp, "%s", spells[i]); // Read each spell from file and store in an array
    }

    char player1Name[20]; // Player 1 name
    char player2Name[20]; // Player 2 name
    printf("Name of player 1: ");
    scanf("%s", player1Name); // Scanner to get player 1 from user
    printf("Name of player 2: ");
    scanf("%s", player2Name); // Scanner to get player 2 from user

    char currentPlayer[20]; // Name of current player
    char currentSpell[100]; // Current spell chosen
    char lastSpell[100] = ""; // Last spell casted, initialized as an empty string because at the beginning of the game no spell has been casted yet

    // Toss a fair coin to determine which player will start
    if (rand() % 2 == 0) {
        strcpy(currentPlayer, player1Name);
        printf("%s won the coin toss and starts.\n", player1Name);
    } else {
        strcpy(currentPlayer, player2Name);
        printf("%s won the coin toss and starts.\n", player2Name);
    }

    fclose(spellsp); // Close file, no longer needed

    char usedSpells[100][100]; // Array to store used spells
    int numofusedspells = 0; // Number of used spells

    while (1) { // Continuous loop
        printf("%s, choose a spell: ", currentPlayer);
        scanf("%s", currentSpell); // Scan chosen spell from current player

        // Check if the spell is valid
        if (strlen(lastSpell) > 0 && !isValidSpell(currentSpell, lastSpell)) {
            printf("Invalid spell! %s wins.\n", strcmp(currentPlayer, player1Name) == 0 ? player2Name : player1Name);
            break; // Exit the loop if spell is invalid
        }

        // Check if the spell has already been used
        int validSpell = 0; // Initially assume it is not valid until proven otherwise
        int isRepetition = 0;

        for (int i = 0; i < numofspells; i++) { // Check if chosen spell is valid and present in the list of available spells
            if (strcmp(spells[i], currentSpell) == 0) {
                validSpell = 1;
                strcpy(spells[i], ""); // Remove the spell from the list
                break;
            }
        }

        for (int i = 0; i < numofusedspells; i++) {
            if (strcmp(usedSpells[i], currentSpell) == 0) {
                isRepetition = 1;
                break;
            }
        }

        if (!validSpell) {
            if (isRepetition) {
                printf("Repetition! %s wins.\n", strcmp(currentPlayer, player1Name) == 0 ? player2Name : player1Name);
            } else {
                printf("Invalid input! %s wins.\n", strcmp(currentPlayer, player1Name) == 0 ? player2Name : player1Name);
            }
            break;
        }

        // Add the current spell to used spells list
        strcpy(usedSpells[numofusedspells], currentSpell);
        numofusedspells++;

        strcpy(lastSpell, currentSpell);

        if (strcmp(currentPlayer, player1Name) == 0) {
            strcpy(currentPlayer, player2Name);
        } else {
            strcpy(currentPlayer, player1Name);
        }

        int allSpellsUsed = 1;
        for (int i = 0; i < numofspells; i++) {
            if (strcmp(spells[i], "") != 0) {
                allSpellsUsed = 0;
                break;
            }
        }
        if (allSpellsUsed) {
            printf("All spells have been used. %s wins!\n", currentPlayer);
        }
    }

    return 0;
}
