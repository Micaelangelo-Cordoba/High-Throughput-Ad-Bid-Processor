#ifndef ADCAMPAIGN_H
#define ADCAMPAIGN_H
#include <cstdint>
#include <unordered_set>


//each line of the CSV will be parsed into a struct
struct BidEvent {
    uint64_t timestamp; //time ad showed up
    double bidAmount; //amount of money spent
    uint32_t userID; //who was the user
    uint32_t campaignID; //from what campaign
};


class AdCampaign {

    public:

    /*Constuctor takes in a unique ID and a budget before
    the CSV comes in */
    AdCampaign(uint32_t campaignID, double budget)
    : m_campaignID {campaignID}, m_RemainingBudget {budget} {}


    /*Boolean member function checks if Campaign has enough balance
    for each bid */
    bool canAfford (double bidAmount) const;

    void transaction(double bidAmount, uint32_t userID);

    /*Getter functions for the end of the function
    analyses how successful the Campaign was */
    double returnBalance() const {return m_RemainingBudget;}
    uint32_t returnTotalImpressions() const {return totalImpressions;}
    size_t returnUniqueReach() const { return uniqueUsers.size();}



    private:

    double m_RemainingBudget;
    uint32_t totalImpressions {0};
    uint32_t m_campaignID;

    /*unordered_set allows only unique user Id's 
    to enter, allows us to track amount of unique users */
    std::unordered_set<uint32_t> uniqueUsers;

};

inline bool AdCampaign::canAfford(double bidAmount) const {
    return m_RemainingBudget >= bidAmount;
}
inline void AdCampaign::transaction(double bidAmount, uint32_t userID) {
    m_RemainingBudget -= bidAmount;
    totalImpressions++;

    uniqueUsers.insert(userID);
    
}





#endif