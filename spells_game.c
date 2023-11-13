#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int isValidSpell(char *spell, char *lastSpell) {
    if (spell[0] != lastSpell[strlen(lastSpell) - 1]) {
        return 0; // Invalid spell
    }
    return 1; // Valid spell
}

int isRepetition(char* spell, char chosenSpells[][100], int currentChosenSpell) {
    for (int i = 0; i < currentChosenSpell; i++) {
        if (strcmp(chosenSpells[i], spell) == 0) {
            return 1; // Repetition
        }
    }
    return 0;
}


int checkSpellValidity(char *spell, char *lastSpell, char spells[][100], int numofspells) {
    if (strlen(lastSpell) > 0 && !isValidSpell(spell, lastSpell)) {
        return 0; // Invalid spell
    } else if (isRepetition(spell, spells, numofspells)) {
        return 1; // Repetition
    } else {
        return 2; // Valid spell
    }
}

// Function to choose a spell for the bot based on the difficulty level
char* chooseSmartBotSpell(char spells[][100], int numofspells, char lastSpell[100], int difficulty, char chosenSpells[][100], int currentChosenSpell) {
    char* chosenSpell = "-----";

    if (difficulty == 0) {
        // Easy: Choose the first word that matches the last character and is not repeated
        for (int i = 0; i < numofspells; i++) {
            if (strlen(spells[i]) > 0 && isValidSpell(spells[i], lastSpell) && !isRepetition(spells[i], chosenSpells, currentChosenSpell)) {
                chosenSpell = spells[i];
                break;
            }
        }
    } else if (difficulty == 1) {
        // Medium: Randomly choose a word that starts with the last character of human input and is not repeated
        char lastLetter = lastSpell[strlen(lastSpell) - 1];
        int validOptions[100];
        int numValidOptions = 0;

        for (int i = 0; i < numofspells; i++) {
            if (strlen(spells[i]) > 0 && spells[i][0] == lastLetter && !isRepetition(spells[i], chosenSpells, currentChosenSpell)) {
                validOptions[numValidOptions++] = i;
            }
        }

        if (numValidOptions > 0) {
            // Choose a random spell from the valid options
            int randomIndex = rand() % numValidOptions;
            chosenSpell = spells[validOptions[randomIndex]];
        }
    } else if (difficulty == 2) {
        // Hard: Choose words that lead to a dead end without repetition
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
                if (leadsToDeadEnd && !isRepetition(spells[i], chosenSpells, currentChosenSpell)) {
                    validOptions[numValidOptions++] = i;
                }
            }
        }

        if (numValidOptions > 0) {
            // Choose a word that leads to a dead end
            int randomIndex = rand() % numValidOptions;
            chosenSpell = spells[validOptions[randomIndex]];
        }
    }

    return chosenSpell;
}


int main() {
    int difficulty;
    char spells[100][100];
    char chosenSpells[100][100];
    int currentChosenSpell = 0;
    int numofspells;
    FILE *spellsp;
    spellsp = fopen("spells.txt", "r");

    if (spellsp == NULL) {
        perror("File was not opened ");
        return 1;
    }

    fscanf(spellsp, "%d", &numofspells);

    for (int i = 0; i < numofspells; i++) {
        fscanf(spellsp, "%s", spells[i]);
    }

    char player1Name[20];
    char player2Name[20] = "Bot";
    printf("Enter your name: ");
    scanf("%s", player1Name);

    // Prompt the user until a valid difficulty level is entered
    while (1) {
        printf("Enter the desired difficulty level (0 = Easy, 1 = Medium, 2 = Hard): ");
        int result = scanf("%d", &difficulty);
        if (result == 1 && difficulty >= 0 && difficulty <= 2) {
            break; // Exit the loop if a valid difficulty level is entered
        } else {
            // Clear the input buffer to handle invalid input
            while (getchar() != '\n')
                ;
            printf("Invalid input. Please enter a valid difficulty level.\n");
        }
    }

    char currentSpell[100];
    char lastSpell[100] = "";

    srand(time(NULL));

    int isHumanPlayerTurn = 0;
    printf("%s, choose a spell: ", player1Name);
    scanf("%s", lastSpell);

    while (1) {
        if (isHumanPlayerTurn) {
            printf("%s, choose a spell: ", player1Name);
            scanf("%s", currentSpell);
        } else {
            strcpy(currentSpell, chooseSmartBotSpell(spells, numofspells, lastSpell, difficulty, chosenSpells, currentChosenSpell));
            printf("Bot chooses: %s\n", currentSpell);
        }

        if (!isValidSpell(currentSpell, lastSpell)) {
            printf("Invalid spell! %s wins.\n", isHumanPlayerTurn ? player2Name : player1Name);
            break;
        } else if (isRepetition(currentSpell, chosenSpells, currentChosenSpell + 1)) {
            printf("Repetition! %s wins.\n", isHumanPlayerTurn ? player2Name : player1Name);
            break;
        }

        strcpy(chosenSpells[currentChosenSpell], currentSpell);
        currentChosenSpell++;
        strcpy(lastSpell, currentSpell);
        isHumanPlayerTurn = !isHumanPlayerTurn;
    }

    return 0;
}
