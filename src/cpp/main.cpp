//
// Created by KoenDR06 on 6/12/24.
//

#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <iomanip>
#include <utility>
#include "../h/main.h"
#include "../h/Poll.h"

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
    string line;

    int messageCount = 0;

    // Needed for average time between texts calculation
    long epochSum = 0;
    long oldEpoch = 0;

    map<string, int> personalMessageCounter;
    map<string, int> personalMediaCounter;
    map<string, vector<pair<float, float>>> personalLocations;
    map<string, int> commonWordMap;
    map<int, int> messagesOnTimeStamp;
    vector<Poll> polls;
    int linesProcessed = 0;
    int wordCount = 0;
    string text;

    while (getline(file, line)) {
        bool multiLine = false;
        bool countWords = true;

        // Checks if this line starts a new message
        regex pattern(R"((\d{1,2})\/(\d{1,2})\/(\d{2}), (\d{2}):(\d{2}).*)");
        if (regex_match(line.substr(0, 100), pattern)) {
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

            int timeOfDay = getTimeOfDay(dateTime);
            messagesOnTimeStamp[timeOfDay]++;

            // Who sent this message?
            if (msg.find(": ") != 18446744073709551615) {
                if (text == "<Media omitted>") {
                    personalMediaCounter[sender] += 1;
                }
                personalMessageCounter[sender] += 1;
            } else {
                personalMessageCounter["Management of Group Chat"] += 1;
            }

            // Location
            if (text.substr(0, 10) == "location: ") {
                countWords = false;
                string locationURL = text.substr(10, text.length() - 10);
                string coordinates = locationURL.substr(locationURL.find('=') + 1, locationURL.length() - locationURL.find('='));
                float latitude = stof(coordinates.substr(0, coordinates.find(',')));
                float longitude = stof(coordinates.substr(coordinates.find(',') + 1, coordinates.length() - (coordinates.find(','))));

                personalLocations[sender].emplace_back(latitude, longitude);
            }

            //Poll
            if (text == "POLL:") {
                string pollLine;
                getline(file, pollLine);

                Poll poll = Poll();
                poll.owner = sender;
                poll.title = pollLine;

                getline(file, pollLine);

                while (!pollLine.empty()) {
                    vector<string> tokens = splitString(pollLine, " ");
                    string voteToken = tokens[tokens.size() - 2];

                    // This spaghetti selects the option and the amount of votes it got and puts it in the poll object
                    poll.votes
                        [pollLine.substr(pollLine.find(": ") + 2, pollLine.length() - pollLine.find('('))] =
                        stoi(voteToken.substr(1, voteToken.length() - 1));

                    getline(file, pollLine);
                }

                polls.emplace_back(poll);
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
        if ((line.find(": ") != 18446744073709551615 or multiLine) and not countWords) {
            wordCount += countWordsInString(text);

            vector<string> tokens = splitString(text, ' ');
            for (const auto &word: tokens) {
                if (word.empty()) {
                    continue;
                }

                commonWordMap[word] += 1;
            }
        }

/*        // Progress Bar
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
        } */
        cout << line << endl;
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

    cout << "Locations: \n";
    auto locationIterator = personalLocations.begin();
    while (locationIterator != personalLocations.end()) {
        cout << "\"" << locationIterator->first << "\" has been at: \n";
        for(const pair<float, float>& location : locationIterator->second)
            cout << "* " << location.first << ", " << location.second << endl;
        locationIterator++;
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

int getTimeOfDay(string dateTime) {
    string rest = move(dateTime);
    rest = rest.substr(rest.find(", ") + 2, rest.length());

    int hour = stoi(rest.substr(0, rest.find(':')));
    rest = rest.substr(rest.find(':') + 1, rest.length());

    int minute = stoi(rest.substr(0, rest.find(" -")));


    return 60 * hour + minute;
}

string convertToTimeStampString(int timeStamp) {
    int hour = timeStamp / 60;
    int minute = timeStamp - (60 * hour);
    string hourString = to_string(hour);
    string minuteString = to_string(minute);

    padTime(hourString, 2);
    padTime(minuteString, 2);

    return hourString + ":" + minuteString;
}


void padTime(string &str, const size_t num) {
    if(num > str.size()) {
        str.insert(0, num - str.size(), '0');
    }
}

vector<string> splitString(string s, const string& delimiter) {
    vector<string> tokens;

    size_t pos = 0;
    string token;
    while ((pos = s.find(delimiter)) != string::npos) {
        token = s.substr(0, pos);
        cout << token << endl;
        s.erase(0, pos + delimiter.length());
    }
    tokens.emplace_back(token);

    return tokens;
}