//
//  main.cpp
//  CS31 Project 5
//
//  Created by Austin Guo on 11/8/16.
//  Copyright © 2016 Austin Guo. All rights reserved.
//

#include <iostream>
#include <cassert>
#include <cstring>
#include <cctype>
//not allowed to use string objects!
using namespace std;


const int MAX_WORD_LENGTH = 20;

int normalizeRules(char word1[][21],
                   char word2[][21],
                   int distance[],
                   int nRules);

int calculateSatisfaction(const char word1[][MAX_WORD_LENGTH+1],
                          const char word2[][MAX_WORD_LENGTH+1],
                          const int distance[],
                          int nRules,
                          const char document[]);



//helper functions
/**
 * Prints all rules
 */
void printRules(char word1[][21], char word2[][21], int distance[], int nRules) {
    for (int i = 0; i < nRules; i++) {
        for (int j = 0; j < 21 && word1[i][j] != '\0'; j++) {
            cerr << word1[i][j];
        }
        cerr << " ";
        for (int j = 0; j < 21 && word2[i][j] != '\0'; j++) {
            cerr << word2[i][j];
        }
        cerr << " " << distance[i] << endl;
    }
}

/**
 *Converts all rules to lowercase
 */
void convertToLower(char word1[][21], char word2[][21], int nRules) {
    for (int i = 0; i < nRules; i++) {
        for (int j = 0; j < 21; j++) {
            word1[i][j] = tolower(word1[i][j]);
            word2[i][j] = tolower(word2[i][j]);
        }
    }
}

/**
 * Removes a rule
 */
int shiftLeft(char word1[][21],
              char word2[][21],
              int distance[],
              int nRules,
              int index) {
    for (int i = index + 1; i < nRules; i++) { // shift all the elements of the arrays of cstrings and the array of distances left by one: notice the last cstring at nRules - 1 index won't be changed but it doesn't matter because smallberg's spec doesn't care about any of the cstrings past the new normalized rules number
        for (int j = 0; j < 21; j++) {
            word1[i - 1][j] = word1[i][j];
            word2[i - 1][j] = word2[i][j];
            distance[i - 1] = distance[i];
        }
    }
    return index;
}


int main() {
    // insert code here...
    
    /*char smallbergRules1[][21] = {"mad", "deranged", "NEFARIOUS", "half-witted", "robot", "plot", "have"};
    char smallBergRules2[][21] = {"scientist", "robot", "PLOT", "assistant", "deranged", "Nefarious", "mad"};
    int smallBergRulesD[] = {2, 4, 1, 3, 2, 1, 13};
    printRules(smallbergRules1, smallBergRules2, smallBergRulesD, 7); cerr << endl;
    cerr << normalizeRules(smallbergRules1, smallBergRules2, smallBergRulesD, 7) << endl;
    printRules(smallbergRules1, smallBergRules2, smallBergRulesD, 7);
    //assert(normalizeRules(smallbergRules1, smallBergRules2, smallBergRulesD, 7) == 4);&/
    return 0;*/
    
    const int TEST1_NRULES = 4;
    char test1w1[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
        "mad",       "deranged", "nefarious", "have"
    };
    char test1w2[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
        "scientist", "robot",    "plot",      "mad"
    };
    int test1dist[TEST1_NRULES] = {
        2,           4,          1,           13
    };
    
    cerr << calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
                          "The mad UCLA scientist unleashed a deranged evil giant robot.");
    
    /*assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
                                 "The mad UCLA scientist unleashed a deranged evil giant robot.") == 2);
    assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
                                 "The mad UCLA scientist unleashed    a deranged robot.") == 2);
    assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
                                 "**** 2016 ****") == 0);
    assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
                                 "  That plot: NEFARIOUS!") == 1);
    assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
                                 "deranged deranged robot deranged robot robot") == 1);
    assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
                                 "That scientist said two mad scientists suffer from deranged-robot fever.") == 0);
    cout << "All tests succeeded" << endl;*/
}






/**
 *The first four parameters represent a set of rules that this function is allowed to assume is in normal form. (In other words, it need not check this, and is allowed to not work correctly or even do something undefined if the set is not in normal form.) The last argument is a C string containing the entire text of a document. You are allowed to assume that no document is longer than 200 characters. (We guarantee we will not test your function with documents that exceed this limit.) The function must return the satisfaction score of the document (i.e., the number of rules that the document satisfies). Treat a negative nRules parameter as if it were 0.
 */
int normalizeRules(char word1[][MAX_WORD_LENGTH + 1], //max of 20 char + 1 null byte
                   char word2[][MAX_WORD_LENGTH + 1], //max of 20 char + 1 null byte
                   int distance[],
                   int nRules) {
    
    int count = nRules;          //to count how many rules are in the normalized array
    if (nRules <= 0) return 0; //Treat a negative nRules parameter as if it were 0.
    
    
    //convert all rules to lowercase
    convertToLower(word1, word2, nRules);
    
    
    //go through and remove all incorrect rules
    for (int i = 0; i < count; i++) {
        
        
        bool removed = false;
        
        
        //  ----    See if the rule needs to remove itself  ----
        
        if (distance[i] < 0) {      // Remove if distance is negative
            shiftLeft(word1, word2, distance, count, i);
            i--;            // to prevent skipping cstrings
            count--;
            removed = true;
        }
        
        if (!removed) {                    // only execute this code if the cstring in the row hasn't been removed yet
            if (word1[i][0] == '\0' || word2[i][0] == '\0') {  // Remove if A word in the rule contains no characters/empty
                shiftLeft(word1, word2, distance, count, i);
                i--;                // to prevent skipping cstrings
                count--;
                removed = true;
            }
        }
        
        if (!removed) {
            for (int j = 0; j < 21 && word1[i][j] != '\0'; j++) {   // Remove if contains a character that is not a letter
                if (!isalpha(word1[i][j])) {
                    shiftLeft(word1, word2, distance, count, i);
                    i--;                // to prevent skipping cstrings
                    count--;
                    removed = true;
                    break;
                }
            }
        }
        //  ----    Rule didn't remove itself   ----
        
        
        
        //  ----    If the rule didn't remove itself from the above conditions  ----
        if (!removed) {
            // separating the current cstring to make comparisons easier
            char currentWord1[21];
            char currentWord2[21];
            for (int h = 0; h < 21; h++) {
                currentWord1[h] = word1[i][h];
                currentWord2[h] = word2[i][h];
            }
            
            //debugging
            /*cerr << "Current word is ";
            for (int h = 0; h < 21; h++) {
                cerr << currentWord1[h];
            }
            cerr << " ";
            for (int h = 0; h < 21; h++) {
                cerr << currentWord2[h];
            }
            cerr << endl;*/
            
            // set the maximum distance equal to the position in rules we're on right now
            int posOfMaxDist = i;
            int maxDist = distance[i];
            //Remove all repeats or permutations of this rule
            for (int k = i; k < count; k++) {       // index k is comparing to the actual index we're on index i
                if (k != i) {               //don't remove the rule when compared to itself
                    char tempWord1[21];
                    char tempWord2[21];
                    for (int m = 0; m < 21; m++) {
                        tempWord1[m] = word1[k][m];
                        tempWord2[m] = word2[k][m];
                    }
                    
                    if ((strcmp(tempWord1, currentWord1) == 0 && strcmp(tempWord2, currentWord2) == 0) || (strcmp(tempWord1, currentWord2) == 0 && strcmp(tempWord2, currentWord1) == 0)) {  // removes the OTHER ROW (not the current row) if the two rows' w1 and w2 are equal or permutations of one another
                        
                        //debugging
                        /*cerr << "Comparison 1 is " << (strcmp(tempWord1, currentWord1) == 0 && strcmp(tempWord2, currentWord2) == 0) << " ";
                        cerr << "Comparison 2 is " << (strcmp(tempWord1, currentWord2) == 0 && strcmp(tempWord2, currentWord1) == 0) << " ";
                        cerr << "Removed ";
                        for (int h = 0; h < 21; h++) {
                            cerr << tempWord1[h];
                        }
                        cerr << " ";
                        for (int h = 0; h < 21; h++) {
                            cerr << tempWord2[h];
                        }
                        cerr << endl;*/
                        
                        
                        // TODO: Find the larger distance
                        if (distance[k] > maxDist) {
                            shiftLeft(word1, word2, distance, nRules, posOfMaxDist);
                            k--;
                            maxDist = distance[k];
                            posOfMaxDist = k;
                        }
                        else {
                            shiftLeft(word1, word2, distance, nRules, k);
                            k--;
                        }
                        count--;
                    }
                }
            }
        }
        
    }
    return count;
}


int calculateSatisfaction(const char word1[][MAX_WORD_LENGTH+1],
                          const char word2[][MAX_WORD_LENGTH+1],
                          const int distance[],
                          int nRules,
                          const char document[]) {
    int satisfaction = 0;
    bool ruleUsed[nRules];  // to keep track of which rules were counted and not count repeats
    for (int i = 0; i < nRules; i++) {  //initialize all rules used as false, since none were used yet
        ruleUsed[i] = false;
    }
    
    char newDocument[201];
    long doclength = strlen(document);
    int newDocLength = 0;
    //copy document into a new document cstring without any punctuation or numbers
    
    for (int i = 0; i < doclength && i < 200 && document[i] != '\0'; i++) { //  index should be less than doclength, 200, and should not have the end marker there
        if (isalpha(document[i]) || isspace(document[i])) {
            newDocument[newDocLength] = document[i];
            newDocLength++;
        }
    }
    
    //  ----    TESTING ----
    for (int i = 0; i < newDocLength; i++) {
        cerr << newDocument[i];
    }
    
    newDocument[newDocLength] = '\0';   //mark the end of the new document cstring
    for (int i = 0; i < newDocLength; i++) {
        newDocument[i] = tolower(newDocument[i]);
    }
    
    
    
    //NEW!!!!!!!!
    int i = 0;
    while (i < newDocLength) {  //going thru entire doc
        char tempWord1[201];
        int tempWord1Length = 0;
        for (; i < newDocLength && !isspace(newDocument[i]); i++) { //finding one word
            tempWord1[i] = newDocument[i];
            tempWord1Length++;
        }
        //i is at space after word1
        tempWord1[i] = '\0';
        //see if the word is equal to any of the word1s
        for (int row = 0; row < nRules; row++) {
            char actualWord1[21];
            for (int j = 0; j < 21 && word1[row][j] != '\0'; j++) {
                actualWord1[j] = word1[row][j];
            }
            if (strcmp(tempWord1, actualWord1) == 0) {
                //the word2 of the rule
                char actualWord2[21];
                for (int j = 0; j < 21 && word2[row][j] != '\0'; j++) {
                    actualWord2[j] = word2[row][j];
                }
                //checking words on the right of the tempWord1
                int pos = i + 1;    //pos is the next letter
                int wordsAway = 0;  //how far tempWord2 is from tempWord1
                char tempWord2[201];
                int tempWord2Length = 0;
                while (pos < newDocLength && wordsAway <= distance[row]) {
                    for (; pos < newDocLength && !isspace(newDocument[pos]); pos++) {
                        tempWord2[tempWord2Length] = newDocument[pos];
                        tempWord2Length++;
                    }
                    tempWord2[tempWord2Length] = '\0';
                    if (strcmp(tempWord2, actualWord2) == 0) {
                        ruleUsed[row] = true;
                        satisfaction++;
                        cerr << satisfaction;
                        break;
                    }
                    wordsAway++;
                    pos++;  // puts position at the first letter of the next word
                }
                if (!ruleUsed[row]) {
                    wordsAway = 0;
                    pos = i - tempWord1Length - 1;
                    for (; pos >= 0 && !isspace(newDocument[pos]); pos--); // decrement variable until reaching space
                    pos++;  // position at first letter of word before
                    while (pos >= 0 && wordsAway <= distance[row]) {
                        for (; pos < newDocLength && !isspace(newDocument[pos]); pos++) {
                            tempWord2[tempWord2Length] = newDocument[pos];
                            tempWord2Length++;
                        }
                        tempWord2[tempWord2Length] = '\0';
                        if (strcmp(tempWord2, actualWord2) == 0) {
                            ruleUsed[row] = true;
                            satisfaction++;
                            cerr << satisfaction;
                            break;
                        }
                        wordsAway++;
                        pos -= tempWord2Length;
                        pos--;  // puts position at the first letter of the next word
                    }
                }
            }
        }
        i++;
    }
    return satisfaction;
}


/*int calculateSatisfaction(const char word1[][MAX_WORD_LENGTH+1],
                          const char word2[][MAX_WORD_LENGTH+1],
                          const int distance[],
                          int nRules,
                          const char document[]) {  //document is cstring
    int satisfaction = 0;
    bool ruleUsed[nRules];  // to keep track of which rules were counted and not count repeats
    for (int i = 0; i < nRules; i++) {  //initialize all rules used as false, since none were used yet
        ruleUsed[i] = false;
    }
    
    char newDocument[201];
    long doclength = strlen(document);
    int newDocLength = 0;
    //copy document into a new document cstring without any punctuation or numbers
    
    for (int i = 0; i < doclength && i < 200 && document[i] != '\0'; i++) { //  index should be less than doclength, 200, and should not have the end marker there
        if (isalpha(document[i]) || isspace(document[i])) {
            newDocument[newDocLength] = document[i];
            newDocLength++;
        }
    }
    
    //  ----    TESTING ----
    for (int i = 0; i < newDocLength; i++) {
        cerr << newDocument[i];
    }
    
    newDocument[newDocLength] = '\0';   //mark the end of the new document cstring
    for (int i = 0; i < newDocLength; i++) {
        newDocument[i] = tolower(newDocument[i]);
    }
    // Done with conversion of document cstring to lower and only has letters
    
    
    // test satisfaction
    // identify the word in the document
    for (int i = 0; i < newDocLength; i++) {    //***i is the position in document***
        char tempWord1[200];        // Smallberg might test a document without spaces (only one word)
        int tempWordLength = 0;
        
        bool onOneWord = true;
        while (onOneWord) {
            if (!(isspace(newDocument[i]))) {
                tempWord1[tempWordLength] = newDocument[i];
                cerr << tempWord1[tempWordLength];
                tempWordLength++;
                i++;
            }
            else {
                //cerr << "Reached space" << endl;
                onOneWord = false;
                tempWord1[tempWordLength] = '\0';
                for (int j = 0; j < nRules; j++) {  //*** sorting through rows of rules: J is the rules rows index ***
                    //to see if the word in the doc is the same as any of the word1's in rules
                    char actualWord1[21];
                    int h = 0;
                    for (; h < 21 && word1[j][h] != '\0'; h++) {
                        actualWord1[h] = word1[j][h];
                    }
                    actualWord1[h] = '\0';
                    
                    //if the word is one of the word1's and the rule hasn't been used, check to see if any surrounding words are word2 from the same rule
                    if (strcmp(tempWord1, actualWord1) == 0 && !ruleUsed[j]) {
                        char tempWord2[200];
                        char actualWord2[21];
                        int k = 0;
                        for (; k < 21 && word1[j][k] != '\0'; k++) {
                            actualWord2[k] = word2[j][k];       //***row index is j because it has to be the word2 for the same rule as word1*** (J IS ROW INDEX)
                        }
                        actualWord2[k] = '\0';
                        int wordsAway = 1;
                        int pos = i;    // so can preserve the ending position of the word that matched with word1
                        
                        // check to see if there's word2 on the right side of word1 within distance
                        while (wordsAway <= distance[j]) {
                            for (; !isspace(document[pos]); pos++) {   //since words in document can be much longer than 21 characters, keep reading until you reach a space
                                tempWord2[pos] = document[pos];
                            }
                            tempWord2[pos] = '\0';
                            if (strcmp(tempWord2, actualWord2) == 0) {
                                ruleUsed[j] = true;
                                satisfaction++;
                                cerr << satisfaction;   // --- TESTING ---
                                break;
                            }
                            else {
                                wordsAway++;
                                pos++;
                            }
                        }
                        //if the right side didn't satisfy the rule, check the left side words
                        if (!ruleUsed[j]) {
                            pos = i - tempWordLength - 2;   // pos is now the letter before the space before the first letter of the word1
                            
                            while (pos >= 0 && wordsAway <= distance[j]) {
                                for (int decrementor = pos; decrementor >= 0 && !isspace(document[decrementor]); decrementor--){
                                    pos = decrementor;
                                } //go to the first space before the word1
                                int spaceBeforeTempWord2 = pos;
                                pos++;  // pos is now the first letter of the word we're looking at before word1
                                
                                for (; !isspace(document[pos]); pos++) {   //since words in document can be much longer than 21 characters, keep reading until you reach a space
                                    tempWord2[pos] = document[pos];
                                }
                                tempWord2[pos] = '\0';
                                if (strcmp(tempWord2, actualWord2) == 0) {
                                    ruleUsed[j] = true;
                                    satisfaction++;
                                    cerr << satisfaction;   // ---- TESTING ----
                                    break;
                                }
                                else {
                                    wordsAway++;
                                    pos = spaceBeforeTempWord2;
                                    pos--;                          // pos is now the letter before the space before tempWord2
                                }
                            }
                        }
                        onOneWord = true;
                    }
                }
            }   //at the end here, i is the space before the next word in the document
        }
    }
    return satisfaction;
}*/
