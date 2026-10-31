#ifndef PARSEFILE_H
#define PARSEFILE_H

#include <charconv> //std::from_chars()
#include <cstdlib>
#include <iostream>
#include <algorithm> //for std::find()
#include <limits>
#include "AdEngine.h"
#include "AdCampaign.h"


//should be from start of double value to endLine (pointer to where last parsable value ends)
inline bool CentsBidAmountParser(const char* start, const char* end, Cents& total) { //25.67\n
//start should be the start of that double value

uint64_t Dollars = 0; //get dollars first, the dot allows the first section of the double to be processed.

auto DolCheck = std::from_chars(start, end, Dollars);

if (DolCheck.ec != std::errc {}) return false; //std::from_chars failed.

//you could have two scenarios, no double like 25  or double like 25.78



const char* ptr = DolCheck.ptr; //would point to the end of the line or .

uint64_t FractDollars = 0; //holds cents

if (ptr != end) { //decimal point exists here. 

    //if its a weird symbol that were on, return false, improper CSV format

    if (*ptr != '.') return false; 

    ptr++; //move past the . 

    //now here we could be at 25.\n   or 25.% or 25.8
    //handle the first two example cases which are invalid

    if (ptr == end || *ptr < '0' || *ptr > '9') return false; //there should be a valid integer in here 
    //this also handles negative inputs

    //we have permission to move forward
    //we should move one integer at a time. because 25.8 to FractDollars (that represents cents) can be interpreted
    //as only 8 cents even though .8 actually implies 80 cents here!

    u_int16_t TensPlace = 0; //stores tens place

    auto FractCheck1 = std::from_chars(ptr, ptr+1, TensPlace); //move one character at a time.

    if (FractCheck1.ec != std::errc {}) return false; //std::from_chars has failed.

    ptr = FractCheck1.ptr;

    FractDollars += TensPlace * 10; //add to total cents accounting for tens place value.

    
    //now ptr can end up at   25.5\n   25.5#  25.57\n   where case 2 should be avoided.

    //ptr can legally reach the end here

    if (ptr == end) {
        //we need to multiply Dollars by 100 for its value in cents. But we must be careful of integer overflow
        //for really large bidAmounts

        const Cents maxValue = std::numeric_limits<Cents>::max(); //max possible value before overflow

        //check if the prospective total exceeds maxValue 

        if (Dollars > (maxValue - FractDollars) / 100) return false; //will result in integer overflow!

        //we can safely get total:

        Dollars *= 100;
        // total = Dollars + FractDollars;
        //total is passed by reference, we dont want to modify it before checking if 
        //its valid in the first place.

        Cents result = Dollars + FractDollars; //compare this value

        //account for if total is 0. implies an invalid bid amount
        if (result == 0) return false;

        //now its valid, now assign total
        total = result;
        return true; //can exit here.
    }

    //now we could be at either 25.5# or 25.57\n check case 1


    if (*ptr < '0' || *ptr > '9') return false; //invalid  character

    //we have reached the last decimal place.

    uint16_t OnesPlace = 0;

    //should still move one at a time, there could be 25.2555 which is illegal

    auto FractCheck2 = std::from_chars(ptr, ptr+1, OnesPlace);

    if (FractCheck2.ec != std::errc {}) return false;

    FractDollars += OnesPlace; //add ones place cents this should be the final value


    ptr = FractCheck2.ptr;  //if this is a proper CSV line this should be the end!


    if (ptr != end) return false; //also ignores any weird symbols and extra integer places

}

//we can add up total and return true here.
//Just like previously, we must account for integer overflow

const Cents maxValue = std::numeric_limits<Cents>::max();

if (Dollars > (maxValue - FractDollars) / 100) return false; //integer overflow

Dollars *= 100; 

// total = Dollars + FractDollars;  same issue as above

Cents result = Dollars + FractDollars;

if (result == 0) return false;


total = result;

return true; 

}

inline bool ParseFile(const char* iterator, std::size_t size, AdEngine& engine) {
    const char* endOfFile = iterator + size; //pointer arithmetic returns onne past end

    //need a massive while loop 
    //ensuring iterator processes data until the very end of the file!
//remember each line of CSV gets parsed by a BidEvent struct 

   

    while (iterator < endOfFile) {
        //remember, std::from_chars can fail too!

        BidEvent currEvent {}; //new struct for each row
        
        //if we encounter empty lines of spaces, skip them prematurely
        //also helps going to the next line
        while (iterator < endOfFile && (*iterator == '\n' || *iterator == '\t' || *iterator == ' ' || *iterator == '\r')) {
            iterator++;
        }

        if (iterator == endOfFile) break; //file fully parsed


        //line is probably
        // timestamp,userID,campaignID,doubleBid

        //we can parse line by getting pointers from the start to where the end of the line
        //of the CSV file is supposed to happen, at a newline.

        const char* startLine = iterator;

        
        const char* newLine = std::find(iterator, endOfFile, '\n');
        //std::find() will either point to the next newline or to endOfFile 

        const char* endLine = newLine; //endLine should be where newLine resides

        //carriage return in Windows \r can be before the newline
        if (endLine > startLine  && *(endLine -1) == '\r') {
            endLine--;
        }




        if (startLine != endLine) {

            //let [startLine, endline) be a range

            //get a pointer for parsing each line only leave master
            //iterator separate

            const char* fields = startLine;

            //std::from_chars returns a struct containing .ec and .ptr?
            //parse the data from the start of the line to the end of the line to insert into this integer
            auto TS = std::from_chars(fields, endLine, currEvent.timestamp);

            //check for correctness each time. 


            //FOR TIME_STAMP
            //std::errc means that no error  code = success!
            if (TS.ec != std::errc {} || TS.ptr == endLine || *TS.ptr != ',') return false; 
            //if (from chars failed OR the delimiter to separate values is not a comma OR we already reached the endline)

            fields = TS.ptr + 1; //ptr points one past valid data skip comma.


            //FOR USER ID
            auto UI = std::from_chars(fields, endLine, currEvent.userID);

            if (UI.ec != std::errc {} || UI.ptr == endLine || *UI.ptr != ',') return false; 
  
            fields = UI.ptr + 1;

           
        
            //FOR CAMPAIGN ID

            auto CI = std::from_chars(fields, endLine, currEvent.campaignID);

            if(CI.ec != std::errc {} || CI.ptr == endLine || *CI.ptr != ',') return false;

            fields = CI.ptr + 1;



            //FOR Bid amount 

            //CSV file has doubles we must account for that to get actual Cents 

            if (!CentsBidAmountParser(fields, endLine, currEvent.bidAmount)) return false;
            
            
             engine.processBidEvent(currEvent); 
             //finally insert parsed information only when the CSV
             //is in a valid format, a non empty line, and std::from_chars suceeds.
        }

        //remember std::find could either return the next newline or 
        //endofFile!

        //dont use EndLine here because carriage returns can mess up where the actual
        //end of each line is

        iterator = (newLine == endOfFile) ? endOfFile : newLine + 1;

        

    }
    return true;
}

#endif