#ifndef ADENGINE_H
#define ADENGINE_H
#include <iostream>
#include <unordered_map>
#include "AdCampaign.h"


class AdEngine {

    public:

    //revenue starts at 0.
    AdEngine() : totalRevenue {0} {}

    //100 campaigns will be inserted into an unordered_map at the beginnning of the program.
    void insertCampaign(uint32_t campaignID, Cents Budget);

    //each line of the CSV file will be parsed into a BidEvent struct, from there it would make a proper transaction from an AdCampaign
    //object inside the unordered_map
    BidStatus processBidEvent(const BidEvent& event);

    //returns amount of revenue processed from the CSV file at the end of the program.
    Cents getRevenue() const {return totalRevenue;}

    const AdStats& getStatistics() const {return Statistics;} //return a reference to our stats
    
    private:

    std::unordered_map<uint32_t, AdCampaign> campaigns;
    
    Cents totalRevenue; //represent revenue in Cents rather than double

    AdStats Statistics {}; //adding Event statistics to be used in main()

};


inline void AdEngine::insertCampaign(uint32_t campaignID, Cents Budget) {
    //Watch out for identical campagin id that can override things
    campaigns.emplace(campaignID, AdCampaign {campaignID, Budget});
}

inline BidStatus AdEngine::processBidEvent(const BidEvent& event) {
    using enum BidStatus;
    Statistics.totalEvents++; //Applies regardless of sucess or fail

    if (event.bidAmount <= 0) { //invalid bid amount
        Statistics.InvalidBids++;
        return INVALID_BID;
    }

    auto Campaign {campaigns.find(event.campaignID)};  
    if (Campaign == campaigns.end()) { //indicated unknown campaign 
        Statistics.UnknownCampaigns++;
        return UNKNOWN_CAMPAIGN;

    }

    //since we already checked that its greater than 0, failure implies a specific Campaign
    //cannot afford a bid.

    if (!Campaign->second.canAfford(event.bidAmount)) {      
       Statistics.InsufficientBudgets++;
       return INSUFFICIENT_FUNDS;
    }
    
    //passes all tests returns accepted

    Campaign->second.transaction(event.bidAmount, event.userID);
    totalRevenue += event.bidAmount;
    Statistics.AcceptedBids++;
    return ACCEPTED;   
}

#endif