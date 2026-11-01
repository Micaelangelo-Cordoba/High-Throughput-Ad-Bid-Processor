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

//Adding enum class BidStatus that returns an indication to a caller on whether a bid suceeded or why
//it failed for testing.
enum class BidStatus {
    ACCEPTED, //Bid has a legitimate value that is being sent to a legitimate campaign 
    INVALID_BID, //bid has an improper value, either its 0, or negative amount
    INSUFFICIENT_FUNDS, //bidAmount is too large for a specific campaign to accept
    UNKNOWN_CAMPAIGN //bid is being sent from another campaign
};
//Also introducing a AdStats struct that will actually keep track of Event information!
struct AdStats {
    uint64_t totalEvents {}; //calculate the amount of actual Bid events there are
    uint64_t AcceptedBids {}; //calculate how many bids were accepted.
    uint64_t InvalidBids {}; //calculate how many bids were denied by value 
    uint64_t InsufficientBudgets {}; //how many times a specific campaign ran out of money
    uint64_t UnknownCampaigns {}; //how many events occured where a bid was being sent to an unknown 
    //campaignID
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
    uniqueUsers.insert(userID); //order has been changed. Realized that exceptions can be thrown here
    //should ensure values arent modified when exceptions are thrown

    m_RemainingBudget -= bidAmount;
    totalImpressions++;


 
}





#endif