#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/* IsValidSpell checks if the chosen spell is valid according to the two parameters,
    spell represents the string chosen, and last spell is the string representing the last spell casted.
    The function compares the first character of the current spell (spell[0]) to the last character of the last spell (lastSpell[strlen(lastSpell) - 1]).
    If they match (invalid spell), it returns 0; otherwise, it returns 1.
*/
int isValidSpell(char* spell, char* lastSpell) {
    if (spell[0] != lastSpell[strlen(lastSpell) - 1]) {
        return 0; // Invalid spell
    }
    return 1; // Valid spell
}

// Function for the bot player to choose a spell using a smarter strategy
char* chooseSmartBotSpell(char spells[][100], int numofspells, char lastSpell[100]) {
    char* bestSpell = NULL;
    int bestScore = 0;

    for (int i = 0; i < numofspells; i++) {
        if (strlen(spells[i]) > 0 && isValidSpell(spells[i], lastSpell)) {//to check if spells[i] is not empty and is valid 
            int score = 1;
            char tempLastSpell[100];
            strcpy(tempLastSpell, lastSpell);//to copy the contents of lastspell to templastspell
            strcat(tempLastSpell, spells[i]); // to apend the contents of spells[i] to templastspell
            
            for (int j = 0; j < numofspells; j++) {
                if (strcmp(spells[j], tempLastSpell) == 0) {
                    score++;
                }
            }

            if (score > bestScore) {
                bestScore = score;
                bestSpell = spells[i];
            }
        }
    }

    if (bestSpell != NULL) {
        return bestSpell;
    } else {
        // If no optimal spell is found, choose randomly
        int validOptions[100];
        int numValidOptions = 0;
        for (int i = 0; i < numofspells; i++) {
            if (strlen(spells[i]) > 0 && isValidSpell(spells[i], lastSpell)) {
                validOptions[numValidOptions++] = i;
            }
        }
        if (numValidOptions > 0) {
            int randomIndex = rand() % numValidOptions;
            return spells[validOptions[randomIndex]];
        } else {
            // No valid spell found; this should not happen in most cases
            // Return an empty string as a fallback
            return "";
        }
    }
}

int main() {
    char spells[100][100]; // Array to store spells
    int numofspells; // Number of spells
    FILE* spellsp; // Declare a file pointer to interact with the given file
    spellsp = fopen("spells.txt", "r"); // Open the file containing spells
    if (spellsp == NULL) {
        perror("File was not opened ");
        return 1; // Return 1 to indicate an error in opening the file
    }
    fscanf(spellsp, "%d", &numofspells); // Read the number of spells from the file

    for (int i = 0; i < numofspells; i++) {
        fscanf(spellsp, "%s", spells[i]); // Read each spell from the file and store it in an array
    }

    char player1Name[20]; // Player 1 name (human player)
    char player2Name[20] = "Bot"; // Player 2 name (bot player)
    printf("Enter your name: ");
    scanf("%s", player1Name); // Scanner to get the human player's name

    char currentPlayer[20]; // Name of the current player
    char currentSpell[100]; // Current spell chosen
    char lastSpell[100] = ""; // Last spell casted; initialize it as an empty string because at the beginning of the game no spell has been cast yet

    srand(time(NULL)); // Seed the random number generator with the current time

    int isHumanPlayerTurn = 1; // 1 for human, 0 for bot; Initialize as human player's turn

    while (1) { // Continuous loop
        if (isHumanPlayerTurn) {
            printf("%s, choose a spell: ", player1Name);
            scanf("%s", currentSpell); // Scan chosen spell from the human player
        } else {
            strcpy(currentSpell, chooseSmartBotSpell(spells, numofspells, lastSpell));
            printf("Bot chooses: %s\n", currentSpell);
        }

        // Check if the spell is valid, if valid, we don't enter the for loop
        if (strlen(lastSpell) > 0 && !isValidSpell(currentSpell, lastSpell)) {
            printf("Invalid spell! %s wins.\n", isHumanPlayerTurn ? player2Name : player1Name);
            break; // Exit the loop if the spell is invalid
        }

        // Check if the spell has already been used
        int validSpell = 0; // Initially assume it is not valid until proven otherwise
        for (int i = 0; i < numofspells; i++) { // Check if the chosen spell is valid and present in the list of available spells
            if (strcmp(spells[i], currentSpell) == 0) {
                validSpell = 1;
                strcpy(spells[i], ""); // Remove the spell from the list
                break;
            }
        }

        // Check if the spell is valid and not a repetition
        if (!validSpell) {
            printf("Repetition! %s wins.\n", isHumanPlayerTurn ? player2Name : player1Name);
            break; // Exit the loop in case of repetition
        }

        // Set the last spell
        strcpy(lastSpell, currentSpell); // Update the last spell to the current one just used.

        // Switch players
        isHumanPlayerTurn = !isHumanPlayerTurn; // Toggle between human and bot
    }

    return 0;
}

