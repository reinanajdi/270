#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* IsValidSpell checks if the chosen spell is valid according to the two parameters,
    spell represents the string chosen and last spell is the string representing the last spell casted.
    the fucntion compares the first characted f the current spell (spell[0]) to the last character of the last spell (lastSpell[strlen(lastSpell) - 1])
    if they match (invalid spell)it returns 1 else it returns 0
*/
int isValidSpell(char* spell, char* lastSpell) {
    if (spell[0] != lastSpell[strlen(lastSpell) - 1]) {
        return 0; // Invalid spell
    }
    return 1; // Valid spell
}

int main() {
    char spells[100][100];//Array to store spells
    int numofspells;//Number of spells
    FILE *spellsp;//declare file pointer to intercat with given file
    spellsp = fopen("spells.txt", "r");//open file containing spells
    if (spellsp == NULL) {
        perror("File was not opened ");
        return 1; //return 1 to indicate error in opening file
    }
    fscanf(spellsp, "%d", &numofspells);//read number of spells from file

    for (int i = 0; i < numofspells; i++) {
        fscanf(spellsp, "%s", spells[i]);//read each spell from file and store in an array
    }

    char player1Name[20];//player 1 name
    char player2Name[20];//player 2 name
    printf("Name of player 1: ");
    scanf("%s", player1Name);//scanner to get play 1 from user
    printf("Name of player 2: ");
    scanf("%s", player2Name);//scanner to get play 2 from user

    char currentPlayer[20]; //name of current player
    char currentSpell[100]; //current spell chosen
    char lastSpell[100] = ""; //last spell casted, we initailize it as an empty string becauseat the beginning of the game no spell has been casted yet

    strcpy(currentPlayer, player1Name); // Player 1 starts

    fclose(spellsp);//close file, no longer needed

    char usedSpells[100][100]; // Array to store used spells
    int numofusedspells = 0; // Number of used spells
    
    while (1) {
        printf("%s, choose a spell: ", currentPlayer);
        scanf("%s", currentSpell);
    
        // Check if the spell is valid
        if (strlen(lastSpell) > 0 && !isValidSpell(currentSpell, lastSpell)) {
            printf("Invalid spell! %s wins.\n", strcmp(currentPlayer, player1Name) == 0 ? player2Name : player1Name);
            break;
        }
    
        // Check if the spell has already been used
        int validSpell = 0;
        int isRepetition = 0;
    
        for (int i = 0; i < numofspells; i++) {
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
    }
    


    return 0;
}
