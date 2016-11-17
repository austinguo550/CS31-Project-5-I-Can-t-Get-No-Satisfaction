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
    
    int TEST1_NRULES = 7;
    char test1w1[][21] = {"mad", "deranged", "NEFARIOUS", "half8witted", "robot", "plot", "have"};
    char test1w2[][21] = {"scientist", "robot", "PLOT", "assistant", "deranged", "Nefarious", "mad"};
    int test1dist[] = {2, 4, 1, 3, 2, 1, 13};
    //printRules(smallbergRules1, smallBergRules2, smallBergRulesD, 7); cerr << endl;
    TEST1_NRULES = normalizeRules(test1w1, test1w2, test1dist, TEST1_NRULES);
    //printRules(test1w1, test1w2, test1dist, 7);
    //assert(normalizeRules(smallbergRules1, smallBergRules2, smallBergRulesD, 7) == 4);&/
    

    assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
                                 "The mad UCLA scientist unleashed a deranged evil giant robot.") == 2);
    assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
                                 "The mad UCLA scientist unleashed    a deranged robot.") == 2);
    assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
                                 "The mad UCLA scientist unleashed    a  robot   deranged.") == 2);
    assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
                                 "**** 2016 ****") == 0);
    assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
                                 "  That plot: NEFARIOUS!") == 1);
    assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
                                 "deranged deranged robot deranged robot robot") == 1);
    assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
                                 "That scientist said two mad scientists suffer from deranged-robot fever.") == 0);

    cerr << "All Smallberg's tests succeeded" << endl;
    
    //my tests
    char sb1[][21] = {"mad", "deranged", "NEFARIO108US", "half-witted", "robot", "plot", "have"};
    char sb2[][21] = {"scientist", "robot", "PLOT", "assistant", "deranged", "Nefarious", "mad"};
    int sbD[] = {2, 4, 1, 3, 2, 10, 13};
    assert(normalizeRules(sb1, sb2, sbD, 7) == 4);
    // To see if normalizeRules will remove rules with numbers
    
    
    char johnOliver1[][21] = {"mad", "deranged", "NEFARIO108US", "half-witted", "robot", "plot", "have"};
    char johnOliver2[][21] = {"scientist", "robot", "PLOT", "assistant", "deranged", "Nefarious", "mad"};
    int johnOliverD[] = {2, 4, 1, 3, 2, 10, 13};
    assert(normalizeRules(johnOliver1, johnOliver2, johnOliverD, 7) == 4);
    // To see if normalizeRules will remove rules with non-alphabetic characters (half-witted)
    
    

    char short1[][21] = {"mad", "deranged", "NEFARIOUS", "half witted", "robot", "plot", "have"};
    char short2[][21] = {"scientist", "robot", "PLOT", "assistant", "deranged", "Nefarious", "mad"};
    int shortD[] = {2, 4, 1, 3, 2, 1, 13};
    assert(normalizeRules(short1, short2, shortD, 7) == 4);
    // to see if the normalizeRules function can remove rules with more than one word in each word of the rule
    

    char saxon1[][21] = {"", "deranged", "NEFARIOUS", "half witted", "robot", "plot", "have"};
    char saxon2[][21] = {"scientist", "robot", "PLOT", "assistant", "deranged", "Nefarious", "mad"};
    int saxonD[] = {2, 4, 1, 3, 2, 1, 13};
    //printRules(test1w1, test2w2, test1dist, 7); cerr << endl;
    TEST1_NRULES = normalizeRules(saxon1, saxon2, saxonD, TEST1_NRULES);
    // To make sure that any rules with words less than one character will be removed
    

    TEST1_NRULES = 0;
    assert(normalizeRules(test1w1, test1w2, test1dist, TEST1_NRULES) == 0);
    //Handling test case with 0 rules
    

    TEST1_NRULES = -1;
    assert(normalizeRules(test1w1, test1w2, test1dist, TEST1_NRULES) == 0);
    //Handling test case with negative rules should return 0
    

    TEST1_NRULES = 4;
    char tread1[][MAX_WORD_LENGTH+1] = {
        "mad",       "deranged", "nefarious", "have"
    };
    char tread2[][MAX_WORD_LENGTH+1] = {
        "scientist", "robot",    "plot",      "mad"
    };
    int treadD[] = {
        0,           4,          1,           13
    };
    assert(normalizeRules(tread1, tread2, treadD, TEST1_NRULES) == 3);
    //With distance of 0 (should remove the rule)
    
    
    
    
    
    
    int brorules = 0;
    assert(calculateSatisfaction(test1w1, test1w2, test1dist, brorules,
                                 "The mad UCLA scientist unleashed a deranged evil giant robot.") == 0);
    //Handle test case with 0 rules
    
    
    //cerr << "got here";
    char weird1[][21] = {"mad", "deranged", "g", "r", "f", "n", "h", "j", "i", "j", "h", "u", "o", "uoi", "ashdjf", "ahsjdfh"};
    char weird2[][21] = {"scientist", "robot", "e", "s", "r", "w", "u", "boab", "boba", "bobaba", "bio", "dk", "bkjla", "bklkia", "bijjkl", "bobbba"};
    int weirdDist[] = {2, 4, 1, 3, 2, 1, 13, 3, 5, 6, 3, 2, 5, 1, 16, 17};
    int weird = 16;
    assert(calculateSatisfaction(weird1, weird2, weirdDist, weird,
                                 "The mad UCLA scientist unleashed a deranged evil giant robot.") == 2);
    //With a lot of rules (check satisfaction normally)
    //cerr << "got here";
    assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
                                 "ThemadUCLAscientistunleashedaderangedevilgiantrobot.") == 0);
    //With document is only one word and there are more than 20 characters in the document, there should be no rules satisfied
    //cerr << "got here";
    assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
                                 "madscientist") == 0);
    //With document is only one word and there are less than 20 characters in the document, there should still be no rules satisfied since there’s only one word and not a second word
    
    
    char t1[][21] = {"mad", "deranged", "NEFARIOUS", "robot"};
    char t2[][21] = {"fat", "deranged", "PLOT", "robot"};
    int ds[] = {5, 3, 2, 5};
    TEST1_NRULES = 4;
    assert(calculateSatisfaction(t1, t2, ds, TEST1_NRULES,
                                 "deranged deranged robot deranged robot robot") == 2);
    //With word1 is the same word as word2
    
    
    assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,  "AsIvesaidfromthebeginningourswasnotacampaignbutratheranincredibleandgreatmovementmadeupofmillionsofhardworkingmenandwomenwholovetheircountryandwantabetterbrighterfutureandthisspeechwasfromtrumpitwaslo") == 0);
    //With document of full 200 characters should return 0 because rules are only 20 characters long
    
    
    assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
                                 ",.,.]\",./][]\?,.!.@$,;’;") == 0);
    //With document of only punctuation
    
    
    assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
                                 "deranged deranged deranged deranged deranged deranged") == 0);
    //With document of only word1 or word 2 of the rule returns 0 because it never gets word 2
    
    char teedee[][21] = {"he", "deranged", "NEFARIOUS", "robot"};
    char teetoo[][21] = {"of", "deranged", "PLOT", "robot"};
    int deez[] = {50, 3, 2, 5};
    TEST1_NRULES = 4;
    assert(calculateSatisfaction(teedee, teetoo, deez, TEST1_NRULES,
                                 "he seemed to say, ‘just because I’m stronger and more of a man than you are.’ We were in the same Senior Society, and while we were never intimate I always had the impression that he approved of me") == 1);
    cerr << "All my calculateSatisfaction tests succeeded." << endl;
    //With a distance of 200, and the words1 and 2 are on opposite ends of the document
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
        
        if (distance[i] <= 0) {      // Remove if distance is not positive
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
        
        if (!removed) {
            for (int j = 0; j < 21 && word2[i][j] != '\0'; j++) {   // Remove if contains a character that is not a letter
                if (!isalpha(word2[i][j])) {
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
    if (nRules <= 0) return 0;
    int satisfaction = 0;
    bool ruleUsed[nRules];  // to keep track of which rules were counted and not count repeats
    for (int i = 0; i < nRules; i++) {  //initialize all rules used as false, since none were used yet
        ruleUsed[i] = false;
    }
    
    char newDocument[201];
    long doclength = strlen(document);
    //cerr << doclength;
    int newDocLength = 0;
    //copy document into a new document cstring without any punctuation or numbers
    
    for (int i = 0; i < doclength && document[i] != '\0'; i++) { //  index should be less than doclength, 200, and should not have the end marker there
        if (isalpha(document[i]) || isspace(document[i])) {
            newDocument[newDocLength] = document[i];
            newDocLength++;
        }
    }
    
    newDocument[newDocLength] = '\0';   //mark the end of the new document cstring
    for (int i = 0; i < newDocLength; i++) {
        newDocument[i] = tolower(newDocument[i]);
    }
    
    
    
    //NEW!!!!!!!!
    int i = 0;
    while (i < newDocLength) {  //going thru entire doc
        char tempWord1[201] = "";
        int tempWord1Length = 0;
        for (; i < newDocLength && !isspace(newDocument[i]); i++) { //finding one word
            tempWord1[tempWord1Length] = newDocument[i];
            tempWord1Length++;
        }
        //i is at space after word1
        tempWord1[tempWord1Length] = '\0';
        
        //  ----    TESTING ----
        /*cerr << "Temp word 1 is " ;
        for (int s = 0; s < newDocLength; s++) {
            cerr << tempWord1[s];
        }
        cerr << endl << "Temp word 2s are " << endl;*/

        
        //see if the word is equal to any of the word1s
        for (int row = 0; row < nRules; row++) {
            char actualWord1[21] = "";
            for (int j = 0; j < 21 && word1[row][j] != '\0'; j++) {
                actualWord1[j] = word1[row][j];
            }
            if (strcmp(tempWord1, actualWord1) == 0 && !ruleUsed[row]) {
                //the word2 of the rule
                char actualWord2[21] = "";
                for (int j = 0; j < 21 && word2[row][j] != '\0'; j++) {
                    actualWord2[j] = word2[row][j];
                }
                //checking words on the right of the tempWord1
                int pos = i + 1;    //pos is the next letter
                int wordsAway = 1;  //how far tempWord2 is from tempWord1
                
                while (pos < newDocLength && wordsAway <= distance[row]) {
                    char tempWord2[201] = "";
                    int tempWord2Length = 0;
                    for (; pos < newDocLength && !isspace(newDocument[pos]); pos++) {
                        tempWord2[tempWord2Length] = newDocument[pos];
                        tempWord2Length++;
                    }
                    tempWord2[tempWord2Length] = '\0';
                    
                    if (strcmp(tempWord2, actualWord2) == 0) {
                        ruleUsed[row] = true;
                        //cerr << "Row " << row << " was used" << endl;
                        satisfaction++;
                        //cerr << satisfaction;
                        break;
                    }
                    //cerr << "got here";
                    wordsAway++;
                    pos++;  // puts position at the first letter of the next word
                }
                if (!ruleUsed[row]) {
                    wordsAway = 1;
                    pos = i - tempWord1Length - 2;
                    //cerr << newDocument[pos];
                    
                    while (pos >= 0 && wordsAway <= distance[row]) {
                        for (; pos >= 0 && !isspace(newDocument[pos]); pos--); // decrement variable until reaching space
                        pos++;  // position at first letter of word before
                        //cerr << newDocument[pos];
                        char tempVar[201] = "";
                        int tempVarLength = 0;
                        for (; pos < newDocLength && !isspace(newDocument[pos]); pos++) {
                            tempVar[tempVarLength] = newDocument[pos];
                            tempVarLength++;
                        }
                        tempVar[tempVarLength] = '\0';
                        
                        //  ----    TESTING ----
                        /*for (int s = 0; s < newDocLength; s++) {
                            cerr << tempVar[s];
                        }
                        cerr << endl;*/
                        
                        if (strcmp(tempVar, actualWord2) == 0) {
                            ruleUsed[row] = true;
                            //cerr << "Row " << row << " was used" << endl;
                            satisfaction++;
                            //cerr << satisfaction;
                            break;
                        }
                        wordsAway++;
                        pos -= tempVarLength;
                        pos -= 2;  // puts position at the first letter of the next word
                    }
                }
            }
        }
        i++;
    }
    return satisfaction;
}
