#ifndef PARSEFILE_H
#define PARSEFILE_H
#include <iostream>
#include <cstdlib> 
#include <charconv>//high-speed parsing
#include "AdEngine.h"
#include "AdCampaign.h"
void ParseFile(const char* iterator, std::size_t fileSize, AdEngine& Engine) {

    const char* endOfFile {iterator + fileSize}; //pointer arithmetic points to the end of the file.

    BidEvent currEvent; //Only use one struct, but update the struct after every line in the CSV file

    while (iterator < endOfFile) { //keep looping until the end of the file.

        auto result = std::from_chars(iterator, endOfFile, currEvent.timestamp); //reads a number, inserts it into currEvent variables
        iterator = result.ptr + 1; //move past the comma.

        //parse UserId

        result = std::from_chars(iterator, endOfFile, currEvent.userID);
        iterator = result.ptr + 1;

        //parse CampaignID

        result = std::from_chars(iterator, endOfFile, currEvent.campaignID);
        iterator = result.ptr + 1;

        //parse BidAmount
        char* nextptr;
        currEvent.bidAmount = std::strtod(iterator, &nextptr);  //macOS doesnt support floating-point reading for std::from_chars yet, use alternate version.
        iterator = nextptr; //stops just at the newline


        if (iterator < endOfFile && *iterator == '\n') {
            iterator++; //increment to next line
            Engine.processBidEvent(currEvent); //process the parsed line
        }

    }
}





#endif