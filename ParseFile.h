#ifndef PARSEFILE_H
#define PARSEFILE_H

#include <charconv> //std::from_chars()
#include <cstdlib>
#include <iostream>
#include <algorithm> //for std::find()
#include "AdEngine.h"
#include "AdCampaign.h"


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

            auto BA = std::from_chars(fields, endLine, currEvent.bidAmount);

            if (BA.ec != std::errc {} || BA.ptr != endLine) return false;
            //CI.ptr doesnt need to be a comma we should be at the end

            fields = BA.ptr; //no more comma should be it.


            
             engine.processBidEvent(currEvent); 
             //finally insert parsed information only when the CSV
             //is in a valid format, a non empty line, and std::from_chars suceeds.
        }

        //remember std::find could either return the next newline or 
        //endofFile!

        //dont use EndLine here because carriage returns can mess up where the actual
        //end of each line is

        iterator = (newLine == endOfFile) ? endOfFile : newLine + 1;

        //now that line has been parsed, insert to the Engine to be processed

       

        
    }
    return true;
}

#endif