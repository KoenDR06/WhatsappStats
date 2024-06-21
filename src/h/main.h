//
// Created by KoenDR06 on 6/12/24.
//

#include <iostream>
#include <map>
#include <regex>
#include "Poll.h"

using namespace std;

#ifndef WHATSAPPSTATS_MAIN_H
#define WHATSAPPSTATS_MAIN_H


int main(int argc, char * argv[]);
void processChat(
        ifstream& file,
        int lines,
        bool displayProgress
);
char* getCmdOption(char ** begin, char ** end, const std::string & option);
bool cmdOptionExists(char** begin, char** end, const std::string& option);


#endif //WHATSAPPSTATS_MAIN_H
