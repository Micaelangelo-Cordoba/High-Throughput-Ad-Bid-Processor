#ifndef PARSEFILE_H
#define PARSEFILE_H
#include <iostream>
#include <cstdlib> 
#include <charconv>//high-speed parsing
#include "AdEngine.h"
#include "AdCampaign.h"
void ParseFile(const char* iterator, std::size_t fileSize, AdEngine& Engine) {

    const char* endOfFile {iterator + fileSize};

    BidEvent currEvent;

    while (iterator < endOfFile) {

        auto result = std::from_chars(iterator, endOfFile, currEvent.timestamp);
        iterator = result.ptr + 1; //move past the comma.

        //parse UserId

        result = std::from_chars(iterator, endOfFile, currEvent.userID);
        iterator = result.ptr + 1;

        //parse CampaignID

        result = std::from_chars(iterator, endOfFile, currEvent.campaignID);
        iterator = result.ptr + 1;

        //parse BidAmount
        char* nextptr;
        currEvent.bidAmount = std::strtod(iterator, &nextptr);
        iterator = nextptr;


        if (iterator < endOfFile && *iterator == '\n') {
            iterator++;
            Engine.processBidEvent(currEvent);
        }

    }
}





#endif