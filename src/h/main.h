//
// Created by KoenDR06 on 6/12/24.
//

#include <iostream>

using namespace std;

#ifndef WHATSAPPSTATS_MAIN_H
#define WHATSAPPSTATS_MAIN_H

int main();
int processChat(ifstream& file, int lines = -1);
long calculateEpochTime(const string& dateTime);

#endif //WHATSAPPSTATS_MAIN_H
