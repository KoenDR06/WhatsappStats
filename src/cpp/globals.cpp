//
// Created by KoenDR06 on 6/20/24.
//

#include <sstream>
#include "../h/globals.h"

int linesProcessed = 0;
int messageCount = 0;
int wordCount = 0;

long epochSum = 0;
long oldEpoch = 0;

map<int, int> messagesOnTimeStamp;
map<string, int> commonWordMap;
map<string, int> personalMediaCounter;
map<string, int> personalMessageCounter;
map<string, vector<pair<float, float>>> personalLocations;

vector<Poll> polls;

string line;
string text;
string dateTime;
string sender;
string msg;

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