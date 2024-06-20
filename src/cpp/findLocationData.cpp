//
// Created by KoenDR06 on 6/19/24.
//

#include "../h/findLocationData.h"
#include "../h/globals.h"

void findLocationData() {
    string locationURL = text.substr(10, text.length() - 10);
    if (locationURL != "https://maps.google.com/?q=0.0,0.0") {
        string coordinates = locationURL.substr(locationURL.find('=') + 1,
                                                locationURL.length() - locationURL.find('='));
        float latitude = stof(coordinates.substr(0, coordinates.find(',')));
        float longitude = stof(coordinates.substr(coordinates.find(',') + 1,
                                                  coordinates.length() - (coordinates.find(','))));
        personalLocations[sender].emplace_back(latitude, longitude);
    }
}