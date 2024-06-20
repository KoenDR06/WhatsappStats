//
// Created by KoenDR06 on 6/19/24.
//

#include "../h/globals.h"
#include "../h/calculateTimeOfDay.h"

using namespace std;

int getTimeOfDay(string timeString) {
    string rest = std::move(timeString);
    rest = rest.substr(rest.find(", ") + 2, rest.length());

    int hour = stoi(rest.substr(0, rest.find(':')));
    rest = rest.substr(rest.find(':') + 1, rest.length());

    int minute = stoi(rest.substr(0, rest.find(" -")));


    return 60 * hour + minute;
}

void calculateTimeOfDay() {
    int timeOfDay = getTimeOfDay(dateTime);
    messagesOnTimeStamp[timeOfDay]++;
}