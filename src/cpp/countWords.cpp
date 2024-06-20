//
// Created by KoenDR06 on 6/19/24.
//

#include "../h/countWords.h"
#include "../h/globals.h"

int countWordsInString(const string& str) {
    int spaceCount = 0;
    for (char ch : str) {
        if (ch == ' ') {
            spaceCount++;
        }
        if (ch == '\n') {
            cerr << "How?\n";
        }
    }
    return spaceCount + 1;
}

void countWordsOfMessage() {
    wordCount += countWordsInString(text);

    vector<string> tokens = splitString(text, ' ');
    for (const auto &word: tokens) {
        if (word.empty()) {
            continue;
        }

        commonWordMap[word] += 1;
    }
}