//
// Created by KoenDR06 on 6/18/24.
//

#ifndef WHATSAPPSTATS_POLL_H
#define WHATSAPPSTATS_POLL_H

#include <iostream>
#include <map>

using namespace std;

class Poll {
    public:
        string title;
        string owner;
        map<string, int> votes;
        [[nodiscard]] string print() const {
            string str = "Title: " + this->title + "\t\tOwner: " + this->owner + "\n";

            for(const auto& vote : this->votes) {
                str += "*   " + vote.first + ": " + to_string(vote.second) + " votes\n";
            }

            return str;
        }
};


#endif //WHATSAPPSTATS_POLL_H
