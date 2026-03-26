#ifndef ADENGINE_H
#define ADENGINE_H
#include <iostream>
#include <unordered_map>
#include "AdCampaign.h"


class AdEngine {

    public:

    //revenue starts at 0.
    AdEngine() : totalRevenue {0.0} {}

    //100 campaigns will be inserted into an unordered_map at the beginnning of the program.
    void insertCampaign(uint32_t campaignID, double Budget);

    //each line of the CSV file will be parsed into a BidEvent struct, from there it would make a proper transaction from an AdCampaign
    //object inside the unordered_map
    void processBidEvent(const BidEvent& event);

    //returns amount of revenue processed from the CSV file at the end of the program.
    double getRevenue() const {return totalRevenue;}
    private:

    std::unordered_map<uint32_t, AdCampaign> campaigns;
    
    double totalRevenue;

};


inline void AdEngine::insertCampaign(uint32_t campaignID, double Budget) {
    campaigns.emplace(campaignID, AdCampaign {campaignID, Budget});
}

inline void AdEngine::processBidEvent(const BidEvent& event) {
    auto Campaign {campaigns.find(event.campaignID)};
    if (Campaign != campaigns.end() && Campaign->second.canAfford(event.bidAmount)) {      
        Campaign->second.transaction(event.bidAmount, event.userID);

        totalRevenue += event.bidAmount;
    }
    
}

#endif