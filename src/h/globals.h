//
// Created by KoenDR06 on 6/20/24.
//

#include <iostream>
#include <map>
#include <vector>
#include <regex>
#include "Poll.h"

using namespace std;

#ifndef WHATSAPPSTATS_GLOBALS_H
#define WHATSAPPSTATS_GLOBALS_H

extern int linesProcessed;
extern int messageCount;
extern int wordCount;
extern int pollVoteOffset;
extern int dateTimeFormat;

extern long epochSum;
extern long oldEpoch;

extern map<int, int> messagesOnTimeStamp;
extern map<string, int> commonWordMap;
extern map<string, int> personalMediaCounter;
extern map<string, int> personalMessageCounter;
extern map<string, vector<pair<float, float>>> personalLocations;

extern vector<Poll> polls;

extern string line;
extern string text;
extern string dateTime;
extern string sender;
extern string msg;
extern string noMediaAttachedIdentifier;
extern string pollIdentifier;

extern regex pattern;
extern regex locationIdentifier;

extern bool printTimeBetweenMessages;
extern bool printMessageCount;
extern bool printPersonalMessageCount;
extern bool printAverageWordsPerMessage;
extern bool printWordCount;
extern bool printCommonWords;
extern bool printCommonTimes;
extern bool printLocations;
extern bool printPolls;

vector<string> splitString(const string& str, const char& delim);
void padTime(string &str, size_t num);
string convertToTimeStampString(int timeStamp);
void coutError(const string& s);

#endif //WHATSAPPSTATS_GLOBALS_H
