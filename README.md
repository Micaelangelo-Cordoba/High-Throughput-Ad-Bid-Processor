# C++ High-Throughput-Ad-Bid-Processor

# Overview
Engineered a high-throughput, single-threaded bid-event processor in C++20, designed to efficiently process large volumes of simulated Real-Time Bidding (RTB) auction data.

The processor handles 10 million simulated bid events in approximately 6 seconds (~1.7 million events/sec) on a 2019 Intel-based MacBook Air. It validates campaign budgets, processes eligible transactions, and tracks impressions and unique user reach across 100 simulated advertising campaigns.

To minimize I/O and parsing overhead, the processor utilizes **POSIX memory-mapped files (mmap)**, allocation-free integer parsing with std::from_chars, and hash-based data structures for efficient campaign lookups and user tracking.

# Core Features

- High Frequency Throughput: Achieves around ~1.7 million events per second utilizing a custom zero-copy memory-mapped architecture.
  
- State Management: Enforces strict financial budget caps for ad campaigns using O(1) constant-time lookups.

- Real Time Analytics: Tracks campaign impressions and calculates unique user reach using two hash sets.

- Data Generation Pipeline: Includes a Python script (generate_data.py) to simulate 10 million rows of randomized, formatted bid events for local performance testing.

# Technical Implementation

- Zero-Copy File I/O (mmap): Bypasses the heavy overhead of std::fstream. The dataset is mapped directly into virtual memory using the POSIX mmap system call, allowing the parser to iterate through the data as a single char array.

- Allocation-free numeric parsing of file data (<charconv>): Reduces heap allocations (e.g. std::string, getline()). Leverages modern C++ through std::from_chars to translate raw ASCII bytes directly into CPU registers. (Note: includes a std::strtod implementation to bypass an Apple Clang limitation regarding the lack of floating-point std::from_chars support).

- Optimized Memory Management: The BidEvent struct used to parse the data is strictly ordered by byte size (uint64_t, double, uint32_t, uint32_t) in order to prevent wasted padding.

- Hash-Based Data Structures: Utilizes std::unordered_map to store unique campaigns and fast lookups. Uses std::unordered_set for unique user tracking.

- Performance Benchmarking: Implemented a custom high-resolution profiling class using std::chrono::steady_clock to precisely measure performance down to the millisecond, ensuring deterministic performance metrics.

# How to run (Linux/MacOS)
1) git clone https://github.com/Micaelangelo-Cordoba/High-Throughput-Ad-Bid-Processor.git

2) Change Directory:

   cd High-Throughput-Ad-Bid-Processor

3) Run python executable to generate csv file:

   ./generate_data.py

4) Use g++ to compile the C++20 program:

   g++ -std=c++20 main.cpp -o High_Throughput_Ad_Bid_Processor

5) Run C++ executable:

   ./High_Throughput_Ad_Bid_Processor
