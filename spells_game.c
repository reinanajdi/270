#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// Function to check if the chosen spell is valid
int isValidSpell(char* spell, char* lastSpell) {
    return (spell[0] == lastSpell[strlen(lastSpell) - 1]);
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

// Function to choose a spell for the bot based on the difficulty level
char* chooseSmartBotSpell(char spells[][100], int numofspells, char lastSpell[100], int difficulty, char chosenSpells[][100], int currentChosenSpell) {
    char* chosenSpell = "-----";

    if (difficulty == 0) {
        // Easy: Choose the first word that matches the last character
        for (int i = 0; i < numofspells; i++) {
            if (strlen(spells[i]) > 0 && isValidSpell(spells[i], lastSpell)) {
                chosenSpell = spells[i];
                break;
            }
        }
    } else if (difficulty == 1) {
        // Medium: Randomly choose a word that starts with the last character of human input
        char lastLetter = lastSpell[strlen(lastSpell) - 1];
        int validOptions[100];
        int numValidOptions = 0;

        for (int i = 0; i < numofspells; i++) {
            if (strlen(spells[i]) > 0 && spells[i][0] == lastLetter) {
                validOptions[numValidOptions++] = i;
            }
        }

        if (numValidOptions > 0) {
            // Choose a random spell from the valid options
            int randomIndex = rand() % numValidOptions;
            chosenSpell = spells[validOptions[randomIndex]];
        }
    } else if (difficulty == 2) {
        // Hard: Choose words that lead to a dead end, or choose randomly if in a complete dead end
        char lastLetter = lastSpell[strlen(lastSpell) - 1];
        int validOptions[100];
        int numValidOptions = 0;

        for (int i = 0; i < numofspells; i++) {
            if (strlen(spells[i]) > 0 && spells[i][0] == lastLetter) {
                int leadsToDeadEnd = 1;
                for (int j = 0; j < numofspells; j++) {
                    if (strlen(spells[j]) > 0 && spells[j][0] == spells[i][strlen(spells[i]) - 1] && i != j) {
                        leadsToDeadEnd = 0;
                        break;
                    }
                }
                if (leadsToDeadEnd && !isRepetition(spells[i], chosenSpells, currentChosenSpell + 1)) {
                    validOptions[numValidOptions++] = i;
                }
            }
        }

        if (numValidOptions > 0) {
            // Choose a word that leads to a dead end
            int randomIndex = rand() % numValidOptions;
            chosenSpell = spells[validOptions[randomIndex]];
        } else {
            // If in a complete dead end, choose randomly
            int randomIndex = rand() % numofspells;
            chosenSpell = spells[randomIndex];
        }
    }

    return chosenSpell;
}

int main() {
    int difficulty;
    char spells[100][100]; // Array to store spells
    char chosenSpells[100][100];
    int currentChosenSpell = 0;
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

    int isHumanPlayerTurn = 0; // 1 for human, 0 for bot; Initialize as human player's turn
    printf("%s, choose a spell: ", player1Name);
    scanf("%s", lastSpell); // Scan the chosen spell from the player

    // Continuous loop for the game
    while (1) {
        // Check whose turn it is and prompt accordingly
        if (isHumanPlayerTurn) {
            printf("%s, choose a spell: ", player1Name);
            scanf("%s", currentSpell); // Scan the chosen spell from the player
        } else {
            // Bot's turn: Choose a spell
            strcpy(currentSpell, chooseSmartBotSpell(spells, numofspells, lastSpell, difficulty, chosenSpells, currentCh
