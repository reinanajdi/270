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

// Function to check if the chosen spell is a repetition
int isRepetition(char* spell, char spells[][100], int numofspells) {
    for (int i = 0; i < numofspells; i++) {
        if (strcmp(spells[i], spell) == 0) {
            return 1; // Repetition
        }
    }
    return 0;
}

// Function to check the validity of the spell (Invalid, Repetition, or Valid)
int checkSpellValidity(char* spell, char* lastSpell, char spells[][100], int numofspells) {
    if (strlen(lastSpell) > 0 && !isValidSpell(spell, lastSpell)) {
        return 0; // Invalid spell
    } else if (isRepetition(spell, spells, numofspells)) {
        return 1; // Repetition
    } else {
        return 2; // Valid spell
    }
}

// Function for the bot player to choose a spell using a smarter strategy
char* chooseSmartBotSpell(char spells[][100], int numofspells, char lastSpell[100], int difficulty) {

     if (difficulty == 0) { // runs if player chose the easiest difficulty level, simply chooses the next available valid spell.
            // Loop through each spell in the list
        for (int i = 0; i < numofspells; i++) {
            // Check if the spell is valid and can be used after the last spell
            if (strlen(spells[i]) > 0 && isValidSpell(spells[i], lastSpell)) {
                // Return the first valid spell found
                return spells[i];
            }
        }
        // If no valid spell is found, return an empty string
        return "";
    } 
    
    char* bestSpell = NULL;
    int bestScore = 0;

    // Loop through each spell to find the optimal spell for the bot
    for (int i = 0; i < numofspells; i++) {
        if (strlen(spells[i]) > 0 && isValidSpell(spells[i], lastSpell)) {
            int score = 1;
            char tempLastSpell[100];
            strcpy(tempLastSpell, lastSpell);
            strcat(tempLastSpell, spells[i]);

            // Loop through each spell to calculate the score
            for (int j = 0; j < numofspells; j++) {
                if (strcmp(spells[j], tempLastSpell) == 0) {
                    score++;
                }
            }

            if (difficulty == 2) { /* runs if the player chooses hard difficulty
                                      which allows the bot to adopt a more aggressive strategy
                                    * it works by prioritizing spells whose last letters 
                                      rarely occur as first letters, forcing the player to choose from
                                      a smaller list of spells. */
                
                char lastLetter = spells[i][strlen(spells[i]) - 1]; 
                int lastAsFirstCount = 0;
                for (int k = 0; k < numofspells; k++) {
                    if (lastLetter == spells[k][0]) {
                        lastAsFirstCount++;
                    } 
                } score -= lastAsFirstCount;

            }

            // Update the best spell if the current spell has a higher score
            if (score > bestScore) {
                bestScore = score;
                bestSpell = spells[i];
            }
        }
    }

    // If no optimal spell is found, choose randomly
    if (bestSpell != NULL) {
        return bestSpell;
    } else {
        int validOptions[100];
        int numValidOptions = 0;

        // Loop through each spell to find valid options for the bot's random choice
        for (int i = 0; i < numofspells; i++) {
            if (strlen(spells[i]) > 0 && isValidSpell(spells[i], lastSpell)) {
                validOptions[numValidOptions++] = i;
            }
        }

        if (numValidOptions > 0) {
            // Choose a random spell from the valid options
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
    int difficulty;
    char spells[100][100]; // Array to store spells
    int numofspells;       // Number of spells
    FILE* spellsp;          // File pointer to interact with the given file
    spellsp = fopen("spells.txt", "r"); // Open the file containing spells

    // Check if the file was opened successfully
    if (spellsp == NULL) {
        perror("File was not opened ");
        return 1; // Return 1 to indicate an error in opening the file
    }

    // Read the number of spells from the file
    fscanf(spellsp, "%d", &numofspells);

    // Loop to read each spell from the file and store it in the array
    for (int i = 0; i < numofspells; i++) {
        fscanf(spellsp, "%s", spells[i]); // Read each spell from file and store in an array
    }

  char player1Name[20]; // Player 1 name (human player)
    char player2Name[20] = "Bot"; // Player 2 name (bot player)
    printf("Enter your name: ");
    scanf("%s", player1Name);
    printf("Enter the desired difficuly level (0 = Easy, 1 = Medium, 2 = Hard): ");
    scanf("%d", &difficulty); // Scanner to get the desired difficulty level

    char currentSpell[100]; // Current spell chosen
    char lastSpell[100] = ""; // Last spell casted; initialize as an empty string because at the beginning of the game, no spell has been cast yet

    srand(time(NULL)); // Seed the random number generator with the current time

    int isHumanPlayerTurn = 1; // 1 for human, 0 for bot; Initialize as human player's turn

    // Continuous loop for the game
    while (1) {
        // Check whose turn it is and prompt accordingly
        if (isHumanPlayerTurn) {
            printf("%s, choose a spell: ", player1Name);
            scanf("%s", currentSpell); // Scan the chosen spell from the player
        } else {
            // Bot's turn: Choose a spell
            strcpy(currentSpell, chooseSmartBotSpell(spells, numofspells, lastSpell, difficulty));
            printf("Bot chooses: %s\n", currentSpell);
        }

        int validSpell = 0; 

        // is the chosen spell valid and present in the list of available spells
        for (int i = 0; i < numofspells; i++) {
            if (strcmp(spells[i], currentSpell) == 0) {
                validSpell = 1;
                strcpy(spells[i], ""); // Remove the spell from the list
                break;
            }
        }
    // Check if spell is invalid and exit loop
        if (!validSpell) {
            printf("Invalid spell! %s wins.\n", isHumanPlayerTurn ? player2Name : player1Name);
            break;
        } else if (strlen(lastSpell) > 0 && !isValidSpell(currentSpell, lastSpell)) {
            // Check if the spell is a repetition and exit loop
            printf("Repetition! %s wins.\n", isHumanPlayerTurn ? player2Name : player1Name);
            break;
        }

        strcpy(lastSpell, currentSpell); // Update the last spell to the current one just used
        isHumanPlayerTurn = !isHumanPlayerTurn; // Toggle between human and bot
    }

    return 0;
}
