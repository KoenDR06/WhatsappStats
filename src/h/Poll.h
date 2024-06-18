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
};


#endif //WHATSAPPSTATS_POLL_H
