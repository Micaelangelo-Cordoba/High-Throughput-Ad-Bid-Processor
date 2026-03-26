# C++ High-Throughput-Ad-Bid-Processor

# Overview

This is a high-performing C++20 simulation of a Real-Time Bidding (RTB) ad exchange. Designed to process massive streams of auction data with low-latency, this engine validates transactions, enforces campaign budgets, and tracks analytics (number of impressions and unique reach) for 100 campaigns. The processor handles 10 million rows of auction data in approximately 6 seconds (~1.7 million tps) on a 2019 MacBook Air by bypassing standard C++ I/O <fstream> and string parsing entirely.

# Core Features

- High Frequency Throughput: Achieves around ~1.7 million transactions per second utilizing a custom zero-copy memory-mapped architecture.
  
- State Management: Enforces strict financial budget caps for ad campaigns using O(1) constant-time lookups.

- Real Time Analytics: Tracks campaign impressions and calculates unique user reach using two hash sets.

- Data Generation Pipeline: Includes a Python script (generate_data.py) to simulate 10 million rows of randomized, formatted bid events for local performance testing.

# Technical Implementation

- Zero-Copy File I/O (mmap): Bypasses the heavy overhead of std::fstream. The dataset is mapped directly into virtual memory using the POSIX mmap system call, allowing the parser to iterate through the data as a single char array.

- Zero-Allocation Parsing (<charconv>): Eliminates heap allocations (e.g. std::string, getline()). Leverages modern C++ through std::from_chars to translate raw ASCII bytes directly into CPU registers. (Note: includes a std::strtod implementation to bypass an Apple Clang limitation regarding the lack of floating-point std::from_chars support).

- Optimized Memory Management: The BidEvent struct used to parse the data is strictly ordered by byte size (uint64_t, double, uint32_t, uint32_t) in order to prevent wasted padding.

- Hash-Based Data Structures: Utilizes std::unordered_map to store unique campaigns and fast lookups. Uses std::unordered_set for unique user tracking.

- Performance Benchmarking: Implemented a custom high-resolution profiling class using std::chrono::steady_clock to precisely measure performance down to the millisecond, ensuring deterministic performance metrics.

# How to run
1) git clone https://github.com/Micaelangelo-Cordoba/High-Throughput-Ad-Bid-Processor.git

2) Change Directory:

   cd High-Throughput-Ad-Bid-Processor

3) Run python executable to generate csv file:

   ./generate_data.py

4) Use g++ to compile the C++20 program:

   g++ -std=c++20 main.cpp -o High_Throughput_Ad_Bid_Processor

5) Run C++ executable:

   ./High_Throughput_Ad_Bid_Processor
