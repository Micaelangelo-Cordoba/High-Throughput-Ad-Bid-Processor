#ifndef ADCAMPAIGN_H
#define ADCAMPAIGN_H
#include <cstdint>
#include <unordered_set>

using Cents = uint64_t; //unsigned long long to represent amount of cents to maintain integer comparison rather
//than using double. Same size, but more accurate comparisons.


//each line of the CSV will be parsed into a struct
struct BidEvent {
    uint64_t timestamp; //time ad showed up
    Cents bidAmount; //amount of money spent
    uint32_t userID; //who was the user
    uint32_t campaignID; //from what campaign
};


class AdCampaign {

    public:

    /*Constuctor takes in a unique ID and a budget before
    the CSV comes in */
    AdCampaign(uint32_t campaignID, Cents budget)
    : m_campaignID {campaignID}, m_RemainingBudget {budget} {}


    /*Boolean member function checks if Campaign has enough balance
    for each bid */
    bool canAfford (Cents bidAmount) const;

    //if the bool member function above succeeds, make a transaction and keep track of unique impressions.
    void transaction(Cents bidAmount, uint32_t userID);

    /*Getter functions
    analyses how successful the Campaign was */
    Cents returnBalance() const {return m_RemainingBudget;}
    uint32_t returnTotalImpressions() const {return totalImpressions;}
    size_t returnUniqueReach() const { return uniqueUsers.size();}



    private:

    Cents m_RemainingBudget; //each starts off with $50,000 OR 5 million Cents
    uint32_t totalImpressions {0}; //returns amount of impressions including impressions from the same user
    uint32_t m_campaignID; //unique campaign ID

    /*unordered_set allows only unique user Id's 
    to enter, allows us to track amount of unique users */
    std::unordered_set<uint32_t> uniqueUsers;

};


//bottom member function needs to be fixed. Does double comparison. Double comparison gets messy since we cannot
//directly compare it. we can use an integer representing total cents.

//we can also validate whether it takes in negative or 0 value bidamounts

//canAfford is responsible for transaction() from getting called, so we can check here

inline bool AdCampaign::canAfford(Cents bidAmount) const {
    //returns true only if bidAmounts are positive!
    return bidAmount > 0 && m_RemainingBudget >= bidAmount; //now were comparing Cents rather than floating point numbers
}


inline void AdCampaign::transaction(Cents bidAmount, uint32_t userID) {
    m_RemainingBudget -= bidAmount;
    totalImpressions++;

    
    uniqueUsers.insert(userID);
    
}





#endif