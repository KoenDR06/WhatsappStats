//
// Created by KoenDR06 on 6/19/24.
//

#include "../h/findPollData.h"
#include "../h/globals.h"
#include <fstream>



void findPollData(ifstream& file) {
    string pollLine;
    getline(file, pollLine);

    Poll poll = Poll();
    poll.owner = sender;
    poll.title = pollLine;

    getline(file, pollLine);

    while (!pollLine.empty()) {
        vector<string> tokens = splitString(pollLine, ' ');
        string voteToken = tokens[tokens.size() - 2];

        // This spaghetti selects the option and the amount of votes it got and puts it in the poll object
        poll.votes
        [pollLine.substr(pollLine.find(": ") + 2, pollLine.find(" (") - pollLine.find(": ") - 2)] =
                stoi(voteToken.substr(1, voteToken.length() - 1));

        getline(file, pollLine);
    }

    polls.emplace_back(poll);
}