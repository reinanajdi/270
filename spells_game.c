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

    while (1) {//continuous loop 
        printf("%s, choose a spell: ", currentPlayer);
        scanf("%s", currentSpell);//scan chosen spell from current player

        // Check if the spell is valid, if valid we dont enter the for loop
        if (strlen(lastSpell) > 0 && !isValidSpell(currentSpell, lastSpell)) {
            printf("Invalid spell! %s wins.\n", strcmp(currentPlayer, player1Name) == 0 ? player2Name : player1Name);
            break;//exit the loop if spell is invalid
        }

        // Check if the spell has already been used
        int validSpell = 0;//initially assume it is not valid until proven otherwise
        for (int i = 0; i < numofspells; i++) {//check if chosen spell is valid and present in the list of available spells
            if (strcmp(spells[i], currentSpell) == 0) {
                validSpell = 1;
                strcpy(spells[i], ""); // Remove the spell from the list 
                break;
            }
        }

        // Check if the spell is valid and not a repetition
        if (!validSpell) {
            printf("Repetition! %s wins.\n", strcmp(currentPlayer, player1Name) == 0 ? player2Name : player1Name);
            break;//exit loop in case of repetition
        }

        // Set last spell
        strcpy(lastSpell, currentSpell);//strcpy() os used to copy strings. Here we update the last spell to the current one just used.

        // Switch players
        if (strcmp(currentPlayer, player1Name) == 0){//if currentPLayer == player1Name that means player 1 just casted a spell therefore entering the if condition to executing it.
            strcpy(currentPlayer, player2Name);//update currentPlayer to player2Name hence switching turns
        } else {//updates currentPlayer to player1Name, switching turns
            strcpy(currentPlayer, player1Name);
        }
    }

    return 0;
}
