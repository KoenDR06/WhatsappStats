//
// Created by KoenDR06 on 6/19/24.
//

#include "../h/globals.h"

void findSenderOfMessage() {
    if (msg.find(": ") != -1) {
        if (text == noMediaAttachedIdentifier) {
            personalMediaCounter[sender] += 1;
        }
        personalMessageCounter[sender] += 1;
    } else {
        personalMessageCounter["Management of Group Chat"] += 1;
    }
}