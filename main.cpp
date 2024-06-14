//
// Created by KoenDR06 on 6/12/24.
//

#include <fstream>
#include <iostream>
#include <string>
#include <regex>
#include "main.h"

using namespace std;

int main() {
    string file_location = "/home/horseman/Programming/WhatsappStats/chat/chat.txt";
    ifstream inputFile(file_location);
    ifstream countLinesFile(file_location);

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

int processChat(ifstream& file, int lines) {
    string line;

    int message_count = 0;

    // Needed for average time between texts calculation
    long epoch_sum = 0;
    long old_epoch = 0;

    map<string, int> personalMessageCounter;

    int linesProcessed = 0;
    while (getline(file, line)) {
        if (line.find("Messages and calls are end-to-end encrypted. No one outside of this chat, not even WhatsApp, can read or listen to them. Tap to learn more.") != 18446744073709551615) {
            continue;
        }

        // Checks if this line starts a new message
        regex pattern(R"((\d{1,2})\/(\d{1,2})\/(\d{2}), (\d{2}):(\d{2}).*)");
        if (regex_match(line, pattern)) {
            // General things about the content of this message
            string dateTime = line.substr(0, line.find(" - "));
            string msg = line.substr(line.find(" - ") + 3, line.length());
            string sender = msg.substr(0, msg.find(": "));
            string text = line.substr(msg.find(": ") + 2, msg.length());

            // Time calculations
            if (old_epoch != 0) {
                epoch_sum += calculateEpochTime(dateTime) - old_epoch;
                message_count++;
            }
            old_epoch = calculateEpochTime(dateTime);

            // Who sent this message?
            personalMessageCounter[sender] += 1;
        }

        if (lines != -1 && linesProcessed % 128 == 0) {
            cout << "\rProcessed: " << linesProcessed << " / " << lines;
            cout.flush();
        }
        linesProcessed++;
    }

    cout << "\nAverage amount of seconds between messages: " << epoch_sum / message_count;
    cout << "\nAmount of messages sent: " << message_count;

    auto it = personalMessageCounter.begin();

    // Iterate through the map and print the elements
    while (it != personalMessageCounter.end()) {
        cout << "\n" << it->first << " has sent " << it->second << " messages.";
        it++;
    }

    return 0;
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