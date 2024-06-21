//
// Created by KoenDR06 on 6/19/24.
//

#include <algorithm>
#include "../h/countWords.h"
#include "../h/globals.h"

using namespace std;

int countWordsInString(const string& str) {
    int spaceCount = 0;
    for (char ch : str) {
        if (ch == ' ') {
            spaceCount++;
        }
        if (ch == '\n') {
            coutError("How?\n");
        }
    }
    return spaceCount + 1;
}

void countWordsOfMessage() {
    wordCount += countWordsInString(text);

    vector<string> tokens = splitString(text, ' ');
    for (const auto &word: tokens) {
        string lowerCaseWord = word;
        std::transform(lowerCaseWord.begin(), lowerCaseWord.end(), lowerCaseWord.begin(),
                              [](unsigned char c){ return std::tolower(c); });

        if (word.empty()) {
            continue;
        }

        commonWordMap[lowerCaseWord] += 1;
    }
}