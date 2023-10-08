#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int isValidSpell(char* spell, char* lastSpell) {
    if (spell[0] != lastSpell[strlen(lastSpell) - 1]) {
        return 0; // Invalid spell
    }
    return 1; //if valid
}

int main() {
    char spells[100][100];
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
        printf("%s\n", spells[i]);
    }

    char player1Name[20];
    char player2Name[20];
    printf("Name of player 1: ");
    scanf("%s", player1Name);
    printf("Name of player 2: ");
    scanf("%s", player2Name);

    char currentPlayer[20];
    char currentSpell[100];
    char lastSpell[100] = ""; 

    strcpy(currentPlayer, player1Name); // Player 1 starts

    fclose(spellsp);

    // turn taking impl
    while (1) {
        printf("%s, choose a spell: ", currentPlayer);
        scanf("%s", currentSpell);

        // is the word valid
        if (strlen(lastSpell) > 0 && !isValidSpell(currentSpell, lastSpell)) {
            printf("Invalid spell! %s wins.\n", currentPlayer == player1Name ? player2Name : player1Name);
            break;
        }

        // we check if spell is in the list of spells
        int validSpell = 0;
        for (int i = 0; i < numofspells; i++) {
            if (strcmp(spells[i], currentSpell) == 0) {
                validSpell = 1;
                strcpy(spells[i], ""); //we rmeove the spell form the list 
                break;
            }
        }

        if (!validSpell) {
    if (strcmp(currentPlayer, player1Name)) {
        printf("%s chose an invalid spell! %s wins.\n", currentPlayer, player1Name);
    } else {
        printf("%s chose an invalid spell! %s wins.\n", currentPlayer, player2Name);
    }
    break;
}


        // Set last spell
        strcpy(lastSpell, currentSpell);

        // Switch players
        if (strcmp(currentPlayer, player1Name) == 0) {
            strcpy(currentPlayer, player2Name);
        } else {
            strcpy(currentPlayer, player1Name);
        }
    }

    return 0;
}

