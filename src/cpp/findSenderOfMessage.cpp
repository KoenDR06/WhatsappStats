//
// Created by KoenDR06 on 6/19/24.
//

#include "../h/globals.h"

void findSenderOfMessage() {
    if (msg.find(": ") != 18446744073709551615) {
        if (text == "<Media omitted>") {
            personalMediaCounter[sender] += 1;
        }
        personalMessageCounter[sender] += 1;
    } else {
        personalMessageCounter["Management of Group Chat"] += 1;
    }
}