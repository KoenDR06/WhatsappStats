//
// Created by KoenDR06 on 6/19/24.
//

#include "../h/globals.h"

long calculateEpochTime(const string& dateTimeString, int dateTimeFormat) {
    int minute;
    int hour;
    int day;
    int month;
    int year;
    if (dateTimeFormat == 0) {
        // 8/29/18, 08:16
        string rest = dateTimeString;

        month = stoi(rest.substr(0, rest.find('/')));
        rest = rest.substr(rest.find('/') + 1, rest.length());

        day = stoi(rest.substr(0, rest.find('/')));
        rest = rest.substr(rest.find('/') + 1, rest.length());

        year = stoi(rest.substr(0, rest.find(", ")));
        rest = rest.substr(rest.find(", ") + 2, rest.length());

        hour = stoi(rest.substr(0, rest.find(':')));
        rest = rest.substr(rest.find(':') + 1, rest.length());

        minute = stoi(rest.substr(0, rest.find(" -")));;
    } else if (dateTimeFormat == 1) {
        // 29-08-2018 08:16
        string rest = dateTimeString;

        day = stoi(rest.substr(0, rest.find('-')));
        rest = rest.substr(rest.find('-') + 1, rest.length());

        month = stoi(rest.substr(0, rest.find('-')));
        rest = rest.substr(rest.find('-') + 1, rest.length());

        year = stoi(rest.substr(0, rest.find(' '))) % 100;
        rest = rest.substr(rest.find(' ') + 1, rest.length());

        hour = stoi(rest.substr(0, rest.find(':')));
        rest = rest.substr(rest.find(':') + 1, rest.length());

        minute = stoi(rest.substr(0, rest.find(" -")));
    } else {
        coutError("Unknown dateTime format.\n");
    }

    struct tm t = {0};
    t.tm_year = 100 + year;
    t.tm_mon = month - 1;
    t.tm_mday = day;
    t.tm_hour = hour - 1;
    t.tm_min = minute;
    t.tm_sec = 0;

    return mktime(&t);
}

void calculateTimeDifference() {
    if (oldEpoch != 0) {
        epochSum += calculateEpochTime(dateTime, dateTimeFormat) - oldEpoch;
        messageCount++;
    }
    oldEpoch = calculateEpochTime(dateTime, dateTimeFormat);
}