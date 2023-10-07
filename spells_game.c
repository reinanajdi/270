#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


int coinToss() {
    return rand() % 2;
}

int main() {

    srand(time(NULL));

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
    int currentPlayerNumber = 0;

    int coinResult;
    printf("%s, call heads or tails (h/t) for the coin toss: ", player1Name);
    char call[2];
    scanf("%s", call);

    
    coinResult = coinToss();
    if ((coinResult == 0 && strcmp(call, "h") == 0) || (coinResult == 1 && strcmp(call, "t") == 0)) {
        currentPlayerNumber = 0;
    } else {
        currentPlayerNumber = 1;
    }

    strcpy(currentPlayer, currentPlayerNumber == 0 ? player1Name : player2Name);

    fclose(spellsp);

    return 0;
    
}
