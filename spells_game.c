#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// Function to check if the chosen spell is valid
int isValidSpell(char *spell, char *lastSpell) {
    if (strlen(lastSpell) > 0 && tolower(spell[0]) != tolower(lastSpell[strlen(lastSpell) - 1])) {
        return 0; // Invalid spell
    }
    return 1; // Valid spell
}

// Function to check if the chosen spell is a repetition
int isRepetition(char *spell, char spells[][100], int numofspells) {
    for (int i = 0; i < numofspells; i++) {
        if (strlen(spells[i]) > 0 && strcasecmp(spells[i], spell) == 0) {
            return 1; // Repetition
        }
    }
    return 0; // Not a repetition
}

// Function to check if the chosen spell is in the list of valid spells
int isSpellInList(char *spell, char spells[][100], int numofspells) {
    for (int i = 0; i < numofspells; i++) {
        if (strlen(spells[i]) > 0 && strcmp(spells[i], spell) == 0) {
            return 1; // Spell is in the list
        }
    }
    return 0; // Spell is not in the list
}

// Function to check if choosing a specific spell leads to a dead end
int leadsToDeadEnd(char spells[][100], int numofspells, int spellIndex, char chosenSpells[][100], int currentChosenSpell) {
    char lastLetter = spells[spellIndex][strlen(spells[spellIndex]) - 1];

    for (int i = 0; i < numofspells; i++) {
        if (strlen(spells[i]) > 0 && spells[i][0] == lastLetter && !isRepetition(spells[i], chosenSpells, currentChosenSpell + 1)) {
            return 0; // There is a valid option, does not lead to a dead end
        }
    }

    return 1; // Leads to a dead end
}

// Function to choose a spell for the bot based on the difficulty level
char *chooseSmartBotSpell(char spells[][100], int numofspells, char lastSpell[100], int difficulty, char chosenSpells[][100], int currentChosenSpell) {
    char *chosenSpell = malloc(100 * sizeof(char)); // Dynamic memory allocation

    if (difficulty == 0) {
        // Easy: Choose the first word that matches the last character
        for (int i = 0; i < numofspells; i++) {
            if (strlen(spells[i]) > 0 && isValidSpell(spells[i], lastSpell) && !isRepetition(spells[i], chosenSpells, currentChosenSpell + 1)) {
                strcpy(chosenSpell, spells[i]);
                break;
            }
        }
    } else if (difficulty == 1) {
        // Medium: Randomly choose a word that starts with the last character of human input
        char lastLetter = lastSpell[strlen(lastSpell) - 1];
        int validOptions[100];
        int numValidOptions = 0;

        for (int i = 0; i < numofspells; i++) {
            if (strlen(spells[i]) > 0 && spells[i][0] == lastLetter && !isRepetition(spells[i], chosenSpells, currentChosenSpell + 1)) {
                validOptions[numValidOptions++] = i;
            }
        }

        if (numValidOptions > 0) {
            // Choose a random spell from the valid options
            int randomIndex = rand() % numValidOptions;
            strcpy(chosenSpell, spells[validOptions[randomIndex]]);
        }
    } else if (difficulty == 2) {
        // Hard: Choose words that lead to a dead end, or randomly choose a word that starts with the last character of human input
        char lastLetter = lastSpell[strlen(lastSpell) - 1];
        int validOptions[100];
        int numValidOptions = 0;

        for (int i = 0; i < numofspells; i++) {
            if (strlen(spells[i]) > 0 && !isRepetition(spells[i], chosenSpells, currentChosenSpell + 1)) {
                if (spells[i][0] == lastLetter || (spells[i][0] != lastLetter && leadsToDeadEnd(spells, numofspells, i, chosenSpells, currentChosenSpell + 1))) {
                    validOptions[numValidOptions++] = i;
                }
            }
        }

        if (numValidOptions > 0) {
            // Choose a word that leads to a dead end or randomly starts with the last character of human input
            int randomIndex = rand() % numValidOptions;
            strcpy(chosenSpell, spells[validOptions[randomIndex]]);
        }
    }

    // Remove the chosen spell from the list of available spells
    for (int i = 0; i < numofspells; i++) {
        if (strcmp(spells[i], chosenSpell) == 0) {
            spells[i][0] = '\0'; // Empty the spell to mark it as chosen
            break;
        }
    }

    return chosenSpell;
}

// Function to simulate a coin toss (heads or tails)
int coinToss() {
    return rand() % 2; // Returns 0 for heads (player goes first) or 1 for tails (bot goes first)
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
    srand(time(NULL)); // Seed the random number generator with the current time

    int isHumanPlayerTurn = 1; // 1 for human, 0 for bot; Initialize as human player's turn
    int coinResult = coinToss();

    if (coinResult == 1) {
        printf("Heads! %s goes first.\n", player1Name);
        isHumanPlayerTurn = 1; // Human player goes first (heads)
    } else {
        printf("Tails! Bot goes first.\n");
        isHumanPlayerTurn = 0; // Bot goes first (tails)
    }

    while (1) {
        if (isHumanPlayerTurn) {
            printf("%s, choose a spell: ", player1Name);
            scanf("%s", currentSpell);

            // Check if the player's spell is in the list
            if (!isSpellInList(currentSpell, spells, numofspells)) {
                printf("Invalid spell! %s loses.\n", player1Name);
                break;
            }
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
