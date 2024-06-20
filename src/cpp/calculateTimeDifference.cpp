//
// Created by KoenDR06 on 6/19/24.
//

#include "../h/globals.h"

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

void calculateTimeDifference() {
    if (oldEpoch != 0) {
        epochSum += calculateEpochTime(dateTime) - oldEpoch;
        messageCount++;
    }
    oldEpoch = calculateEpochTime(dateTime);
}