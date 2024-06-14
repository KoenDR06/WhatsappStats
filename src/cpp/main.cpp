//
// Created by KoenDR06 on 6/12/24.
//

#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include "../h/main.h"

using namespace std;

int main() {
    string fileLocation = "/home/horseman/Programming/WhatsappStats/chat/DadChat.txt";
    ifstream inputFile(fileLocation);
    ifstream countLinesFile(fileLocation);

    int lines = std::count(std::istreambuf_iterator<char>(countLinesFile),
                std::istreambuf_iterator<char>(), '\n');


    if (!inputFile.is_open()) {
        cerr << "Error opening the file!\n";
        return 1;
    }

    processChat(inputFile, lines);
    inputFile.close();

    return 0;
}

void processChat(ifstream& file, int lines) {
    string line;

    int messageCount = 0;

    // Needed for average time between texts calculation
    long epochSum = 0;
    long oldEpoch = 0;

    map<string, int> personalMessageCounter;
    map<string, int> commonWordMap;

    int linesProcessed = 0;

    int wordCount = 0;

    string text;
    while (getline(file, line)) {
        // Checks if this line starts a new message
        regex pattern(R"((\d{1,2})\/(\d{1,2})\/(\d{2}), (\d{2}):(\d{2}).*)");
        if (regex_match(line, pattern)) {
            // If this line starts an admin message, don't count the words
            if (line.find(": ") != 18446744073709551615) {
                wordCount += countWordsInString(text);

                vector<string> tokens = splitString(text, ' ');
                for (const auto& word : tokens) {
                    if (word == "") {
                        continue;
                    }

                    commonWordMap[word] += 1;
                }
            }


            // General things about the content of this message
            string dateTime = line.substr(0, line.find(" - "));
            string msg = line.substr(line.find(" - ") + 3, line.length());
            string sender = msg.substr(0, msg.find(": "));
            text = msg.substr(msg.find(": ") + 2, msg.length());

            // Time calculations
            if (oldEpoch != 0) {
                epochSum += calculateEpochTime(dateTime) - oldEpoch;
                messageCount++;
            }
            oldEpoch = calculateEpochTime(dateTime);

            // Who sent this message?
            if (msg.find(": ") != 18446744073709551615) {
                personalMessageCounter[sender] += 1;
            } else {
                personalMessageCounter["Management of Group Chat"] += 1;
            }
        } else {
            text += " ";
            text += line;
        }

//        if (lines != -1 && linesProcessed % 128 == 0) {
//            cout << "\rProcessed: " << linesProcessed << " / " << lines;
//            cout.flush();
//        }
        linesProcessed++;
    }

    // Print out the results
    cout << "\nAverage amount of seconds between messages: " << static_cast<double>(epochSum) / static_cast<double>(messageCount);
    cout << "\nAmount of messages sent: " << messageCount;

    auto it = personalMessageCounter.begin();
    while (it != personalMessageCounter.end()) {
        cout << "\n\"" << it->first << "\" has sent " << it->second << " messages.";
        it++;
    }

    cout << "\nAverage amount of words per message: " << static_cast<double>(wordCount) / static_cast<double>(messageCount);
    cout << "\nTotal word count: " << wordCount;

    cout << "\nMost used words: \n";

    for (int index = 1; index <= 10; index++) {
        it = commonWordMap.begin();
        string mostUsedWord;
        int mostUsages = 0;
        while (it != commonWordMap.end()) {
            if (it->second > mostUsages) {
                mostUsedWord = it->first;
                mostUsages = it->second;
            }
            it++;
        }
        cout << index << ".\t'" << mostUsedWord << "' with " << mostUsages << " usages.\n";
        commonWordMap[mostUsedWord] = 0;
    }

    cout.flush();
}

long calculateEpochTime(const string& dateTime) {
    string rest = dateTime;

    int month = stoi(rest.substr(0, rest.find('/')));
    rest = rest.substr(rest.find('/')+1, rest.length());

    int day = stoi(rest.substr(0, rest.find('/')));
    rest = rest.substr(rest.find('/')+1, rest.length());

    int year = stoi(rest.substr(0, rest.find(", ")));
    rest = rest.substr(rest.find(", ")+2, rest.length());

    int hour = stoi(rest.substr(0, rest.find(':')));
    rest = rest.substr(rest.find(':')+1, rest.length());

    int minute = stoi(rest.substr(0, rest.find(" -")));

    struct tm t = {0};
    t.tm_year = 100 + year;
    t.tm_mon = month-1;
    t.tm_mday = day;
    t.tm_hour = hour-1;
    t.tm_min = minute;
    t.tm_sec = 0;

    return mktime(&t);
}

int countWordsInString(const string& str) {
    int spaceCount = 0;
    for (char ch : str) {
        if (ch == ' ' || ch == '\n') {
            spaceCount++;
        }
    }
    return spaceCount + 1;
}

vector<string> splitString(const string& str, const char& delim) {
    stringstream ss(str);
    string token;
    vector<string> tokens;
    char delimiter = delim;
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}