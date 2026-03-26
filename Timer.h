#ifndef TIMER_H
#define TIMER_H
#include <chrono>

class Timer {
    public:
 
    /*takes another snapshot and inserts it into m_time
    to reset the snapshot time distance */
    void reset() {
        m_time = Clock::now();
    }

    //returns elapsed time in milliseconds.
    double timeElapsed() const {
        return std::chrono::duration_cast<Duration>(Clock::now() - m_time).count();
    }

    private:

    using Clock = std::chrono::steady_clock;

    /*Formot for outputting the duration of the program
    in milliseconds. */
    using Duration = std::chrono::duration<double, std::ratio<1,1000>>;
    using TimePoint = std::chrono::time_point<Clock>;

    //Variable keeps track of latest snapshot of time.
    TimePoint m_time {Clock::now()};
};

#endif