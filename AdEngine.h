#ifndef ADENGINE_H
#define ADENGINE_H
#include <iostream>
#include <unordered_map>
#include "AdCampaign.h"


class AdEngine {

    public:

    AdEngine() : totalRevenue {0.0} {}

    void insertCampaign(uint32_t campaignID, double Budget);

    void processBidEvent(const BidEvent& event);

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