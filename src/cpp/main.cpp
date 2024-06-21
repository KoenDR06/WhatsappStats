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

int main(int argc, char * argv[]) {
    if(cmdOptionExists(argv, argv+argc, "--help") ||
       cmdOptionExists(argv, argv+argc, "-h")) {
        ifstream helpFile("/home/horseman/Programming/WhatsappStats/assets/help.txt");
        while (getline(helpFile, line)) {
            cout << line << "\n";
        }
        return 0;
    }

    char * filename;
    if (cmdOptionExists(argv, argv+argc, "--file")) {
        filename = getCmdOption(argv, argv + argc, "--file");
    } else {
        coutError("No input file supplied.\n");
        return 1;
    }

    bool displayProgress = true;
    if (cmdOptionExists(argv, argv+argc, "--quiet")) {
         displayProgress = false;
    }


    if (cmdOptionExists(argv, argv+argc, "--format")) {
        char* formatCommand = getCmdOption(argv, argv + argc, "--format");
        int format = atoi(formatCommand);
        
        if (format == 0) {
            pattern.assign(R"((\d{1,2})\/(\d{1,2})\/(\d{2}), (\d{2}):(\d{2}).*)");
            noMediaAttachedIdentifier = "<Media omitted>";
            locationIdentifier.assign(R"(location: .*)");
            pollIdentifier = "POLL:";
            pollVoteOffset = 1;
            dateTimeFormat = 0;
        } else if (format == 1) {
            pattern.assign(R"((\d{2})\-(\d{2})\-(\d{4}) (\d{2}):(\d{2}).*)");
            noMediaAttachedIdentifier = "<Media weggelaten>";
            locationIdentifier.assign(R"(locatie: .*)");
            pollIdentifier = "PEILING:";
            pollVoteOffset = 4;
            dateTimeFormat = 1;
        }
    } else {
        coutError("No dateTime format was supplied.\n");
        return 1;
    }

    if (cmdOptionExists(argv, argv+argc, "--print_stats")) {
        string command = string(getCmdOption(argv, argv + argc, "--print_stats"));
        auto stats = splitString(command, ',');

        for (const string& stat: stats) {
            if (stat == "time_between_msgs") {
                printTimeBetweenMessages = true;
            } else if (stat == "msg_count") {
                printMessageCount = true;
            } else if (stat == "msg_count_personal") {
                printPersonalMessageCount = true;
            } else if (stat == "avg_words_per_msg") {
                printAverageWordsPerMessage = true;
            } else if (stat == "word_count") {
                printWordCount = true;
            } else if (stat == "common_words") {
                printCommonWords = true;
            } else if (stat == "common_times") {
                printCommonTimes = true;
            } else if (stat == "locations") {
                printLocations = true;
            } else if (stat == "polls") {
                printPolls = true;
            } else {
                coutError("stat \""+stat+"\" is unknown.");
                return 1;
            }
        }

    } else {
        printTimeBetweenMessages = true;
        printMessageCount = true;
        printPersonalMessageCount = true;
        printAverageWordsPerMessage = true;
        printWordCount = true;
        printCommonWords = true;
        printCommonTimes = true;
        printLocations = true;
        printPolls = true;
    }

    string fileLocation = string(filename);
    ifstream inputFile(fileLocation);
    ifstream countLinesFile(fileLocation);

    int lines = count(istreambuf_iterator<char>(countLinesFile),
                      istreambuf_iterator<char>(), '\n');


    if (!inputFile.is_open()) {
        coutError("Error opening the file.\n");
        return 1;
    }

    processChat(inputFile, lines, displayProgress);
    inputFile.close();

    return 0;
}

void processChat(
    ifstream& file,
    int lines,
    bool displayProgress
) {
    while (getline(file, line)) {
        bool multiLine = false;
        bool countWords = true;

        // Checks if this line starts a new message
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
            if (regex_match(text.substr(0, 100), locationIdentifier)) {
                countWords = false;
                findLocationData();
            } else if (text == pollIdentifier) {
                countWords = false;
                findPollData(file, pollVoteOffset);
            }

        } else {
            text = line;
            multiLine = true;
        }

        if (text == noMediaAttachedIdentifier) {
            countWords = false;
        } else if (text == "Waiting for this message") {
            countWords = false;
        }

        // Count the words of the message
        if ((line.find(": ") != -1 or multiLine) and countWords) {
            countWordsOfMessage();
        }

        // Progress Bar
        linesProcessed++;
        if (displayProgress) {
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

    cout << "\n";

    // Print out the results
    if (displayProgress) {
        cout << "---------------------------------------------------------------------------------------------------------------------------\n";
    }

    if (printTimeBetweenMessages) {
        cout << "Average amount of seconds between messages: "
             << static_cast<double>(epochSum) / static_cast<double>(messageCount) << "\n\n";
    }

    if (printMessageCount) {
        cout << "Amount of messages sent: " << messageCount << "\n\n";
    }

    if (printPersonalMessageCount) {
        auto personIterator = personalMessageCounter.begin();
        while (personIterator != personalMessageCounter.end()) {
            if (personIterator->first != "Management of Group Chat") {
                cout << "\"" << personIterator->first << "\" has sent " << personIterator->second << " messages and "
                     << personalMediaCounter[personIterator->first] << " media items.\n";
            } else {
                cout << "This group has had " << personIterator->second << " updates.\n";
            }
            personIterator++;
        }
        cout << "\n";
    }

    if (printAverageWordsPerMessage) {
        cout << "Average amount of words per message: " << static_cast<double>(wordCount) / static_cast<double>(messageCount) << "\n\n";
    }

    if (printWordCount) {
        cout << "Total word count: " << wordCount << "\n\n";
    }

    if (printCommonWords) {
        cout << "Most used words: \n";
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
        cout << "\n";
    }

    if (printCommonTimes) {
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
        cout << "\n";
    }

    if (printLocations) {
        if (!personalLocations.empty()) {
            cout << "Locations: \n";
        }
        auto locationIterator = personalLocations.begin();
        while (locationIterator != personalLocations.end()) {
            cout << "\"" << locationIterator->first << "\" has been at: \n";
            for (const pair<float, float> &location: locationIterator->second)
                cout << "* " << fixed << setprecision(5) << location.first << ", " << location.second << endl;
            locationIterator++;
        }
        cout << "\n";
    }

    if (printPolls) {
        if (!polls.empty()) {
            cout << "Polls: \n";
        }
        for (const Poll &poll: polls) {
            cout << poll.print() << endl;
        }
    }

    cout.flush();
}

char* getCmdOption(char ** begin, char ** end, const std::string & option) {
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return nullptr;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option) {
    return std::find(begin, end, option) != end;
}
