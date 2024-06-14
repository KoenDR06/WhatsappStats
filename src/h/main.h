//
// Created by KoenDR06 on 6/12/24.
//

#include <iostream>

using namespace std;

#ifndef WHATSAPPSTATS_MAIN_H
#define WHATSAPPSTATS_MAIN_H

int main();
void processChat(ifstream& file, int lines = -1);
long calculateEpochTime(const string& dateTime);
int countWordsInString(const string& str);
vector<string> splitString(const string& str, const char& delim);

#endif //WHATSAPPSTATS_MAIN_H
