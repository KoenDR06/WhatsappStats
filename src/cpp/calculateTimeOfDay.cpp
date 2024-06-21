//
// Created by KoenDR06 on 6/19/24.
//

#include "../h/globals.h"
#include "../h/calculateTimeOfDay.h"

using namespace std;

int getTimeOfDay(string timeString) {
    string rest = std::move(timeString);
    if (dateTimeFormat == 0) {
        rest = rest.substr(rest.find(", ") + 2, rest.length());

        int hour = stoi(rest.substr(0, rest.find(':')));
        rest = rest.substr(rest.find(':') + 1, rest.length());

        int minute = stoi(rest.substr(0, rest.find(" -")));


        return 60 * hour + minute;
    } else if (dateTimeFormat == 1) {
        rest = rest.substr(rest.find(' ') + 1, rest.length());

        int hour = stoi(rest.substr(0, rest.find(':')));
        rest = rest.substr(rest.find(':') + 1, rest.length());

        int minute = stoi(rest.substr(0, rest.find(" -")));


        return 60 * hour + minute;
    } else {
        coutError("No dateTime format found\n");
    }
}

void calculateTimeOfDay() {
    int timeOfDay = getTimeOfDay(dateTime);
    messagesOnTimeStamp[timeOfDay]++;
}