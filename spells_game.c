char* chooseSmartBotSpell(char spells[][100], int numofspells, char lastSpell[100], int difficulty) {
    char* chosenSpell = "-----";
 if (difficulty == 0) {
        // Easy: Iterate through words and find the first word that matches the last character
        for (int i = 0; i < numofspells; i++) {
            if (strlen(spells[i]) > 0 && isValidSpell(spells[i], lastSpell)) {
                chosenSpell = spells[i];
                break;
            }
        }
    } else if (difficulty == 1) {
        // Medium: Choose a word that starts with the last character of human input
        char lastLetter = lastSpell[strlen(lastSpell) - 1];
        int validOptions[100];
        int numValidOptions = 0;

        // Loop through each spell to find valid options for Medium's choice
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
    } 

    return chosenSpell;
}
