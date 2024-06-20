//
// Created by KoenDR06 on 6/12/24.
//

#include <iostream>
#include <fstream>
#include <regex>
#include <iomanip>
#include "../h/Poll.h"
#include "../h/main.h"
#include "../h/calculateTimeDifference.h"
#include "../h/calculateTimeOfDay.h"
#include "../h/findSenderOfMessage.h"
#include "../h/findLocationData.h"
#include "../h/findPollData.h"
#include "../h/countWords.h"
#include "../h/globals.h"

using namespace std;

int main() {
    string fileLocation = "/home/horseman/Programming/WhatsappStats/chat/chat.txt";
    ifstream inputFile(fileLocation);
    ifstream countLinesFile(fileLocation);

    int lines = count(istreambuf_iterator<char>(countLinesFile),
                      istreambuf_iterator<char>(), '\n');


    if (!inputFile.is_open()) {
        cerr << "Error opening the file!\n";
        return 1;
    }

    processChat(inputFile, lines);
    inputFile.close();

    return 0;
}

void processChat(ifstream& file, int lines) {
    while (getline(file, line)) {
        bool multiLine = false;
        bool countWords = true;

        // Checks if this line starts a new message
        regex pattern(R"((\d{1,2})\/(\d{1,2})\/(\d{2}), (\d{2}):(\d{2}).*)");
        if (regex_match(line.substr(0, 100), pattern)) {
            // General things about the content of this message
            dateTime = line.substr(0, line.find(" - "));
            msg = line.substr(line.find(" - ") + 3, line.length());
            sender = msg.substr(0, msg.find(": "));
            text = msg.substr(msg.find(": ") + 2, msg.length());

            // Do these for every message
            calculateTimeDifference();
            calculateTimeOfDay();
            findSenderOfMessage();
            if (text.substr(0, 10) == "location: ") {
                countWords = false;
                findLocationData();
            } else if (text == "POLL:") {
                countWords = false;
                findPollData(file);
            }

        } else {
            text = line;
            multiLine = true;
        }

        if (text == "<Media omitted>") {
            countWords = false;
        } else if (text == "Waiting for this message") {
            countWords = false;
        }

        // Count the words of the message
        if ((line.find(": ") != 18446744073709551615 or multiLine) and countWords) {
            countWordsOfMessage();
        }

        // Progress Bar
        linesProcessed++;
        if (lines != -1) {
            cout << "\r[";
            double completedPercentage = static_cast<double>(linesProcessed) / static_cast<double>(lines);
            for (int i = 0; i < 100; i++) {
                if (static_cast<double>(i) / 100 < completedPercentage) {
                    cout << "#";
                } else {
                    cout << "-";
                }
            }
            cout << "] " << linesProcessed << " / " << lines << ", " << fixed << setprecision(2) << static_cast<double>(linesProcessed) / static_cast<double>(lines) * 100.0 << "%";

            cout.flush();
        }
    }

    // Print out the results
    cout << "\n---------------------------------------------------------------------------------------------------------------------------"
            "\nAverage amount of seconds between messages: " << static_cast<double>(epochSum) / static_cast<double>(messageCount);
    cout << "\nAmount of messages sent: " << messageCount;

    auto personIterator = personalMessageCounter.begin();
    while (personIterator != personalMessageCounter.end()) {
        if (personIterator->first != "Management of Group Chat") {
            cout << "\n\"" << personIterator->first << "\" has sent " << personIterator->second << " messages and " << personalMediaCounter[personIterator->first] << " media items.";
        } else {
            cout << "\nThis group has had " << personIterator->second << " updates.";
        }
        personIterator++;
    }

    cout << "\nAverage amount of words per message: " << static_cast<double>(wordCount) / static_cast<double>(messageCount);
    cout << "\nTotal word count: " << wordCount;

    cout << "\nMost used words: \n";
    for (int index = 1; index <= 10; index++) {
        auto wordIterator = commonWordMap.begin();
        string mostUsedWord;
        int mostUsages = 0;
        while (wordIterator != commonWordMap.end()) {
            if (wordIterator->second > mostUsages) {
                mostUsedWord = wordIterator->first;
                mostUsages = wordIterator->second;
            }
            wordIterator++;
        }
        if (mostUsages != 0) {
            cout << index << ".\t'" << mostUsedWord << "' with " << mostUsages << " usages.\n";
            commonWordMap[mostUsedWord] = 0;
        }
    }

    cout << "Most common times of the day: \n";
    auto timeStampIterator = messagesOnTimeStamp.begin();
    for (int index = 1; index <= 10; index++) {
        timeStampIterator = messagesOnTimeStamp.begin();
        int mostCommonTimeStamp;
        int mostUsages = 0;
        while (timeStampIterator != messagesOnTimeStamp.end()) {
            if (timeStampIterator->second > mostUsages) {
                mostCommonTimeStamp = timeStampIterator->first;
                mostUsages = timeStampIterator->second;
            }
            timeStampIterator++;
        }
        if (mostUsages != 0) {
            cout << index << ".\t " << mostUsages << " messages were sent at "
                 << convertToTimeStampString(mostCommonTimeStamp) << "\n";
            messagesOnTimeStamp[mostCommonTimeStamp] = 0;
        }
    }

    if (!personalLocations.empty()) {
        cout << "Locations: \n";
    }
    auto locationIterator = personalLocations.begin();
    while (locationIterator != personalLocations.end()) {
        cout << "\"" << locationIterator->first << "\" has been at: \n";
        for(const pair<float, float>& location : locationIterator->second)
            cout << "* " << fixed << setprecision(5) << location.first << ", " << location.second << endl;
        locationIterator++;
    }

    if (!polls.empty()) {
        cout << "Polls: \n";
    }
    for(const Poll& poll : polls)
        cout << poll.print() << endl;

    cout.flush();
}