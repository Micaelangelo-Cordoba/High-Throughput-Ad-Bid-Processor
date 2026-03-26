#include <iostream>
#include <chrono>
#include <iomanip>
#include <fcntl.h> //open()
#include <sys/stat.h> //fstat()
#include <sys/mman.h> //mmap()
#include <unistd.h> //close()
#include "Timer.h"
#include "AdCampaign.h"
#include "AdEngine.h"
#include "ParseFile.h"


int main() {


AdEngine Engine {};

//There are 100 different Campaigns, where each start with $50,000 before CSV processing.
for (int i = 1; i < 101; ++i) {
    Engine.insertCampaign(i, 50000.0);
}






const char* filepath = "bids_data.csv";


//asks the OS for permission to access file
//O_RDONLY -> im only going to read it
int canOpen = open(filepath, O_RDONLY);
if(canOpen == -1) {
    std::cerr << "Error: Could not open bids_data.csv\n";
    return 1;
}

//stat struct build into Linux
//pass file ticket canOpen with address
//of empty struct into fstat.
//OS fills struct with file info,
//we must get its size to iteratre through CSV
struct stat fileInfo;
fstat(canOpen, &fileInfo);
size_t fileSize = fileInfo.st_size;


//nullptr ->tell OS to find an empty spot in RAM
//fileSize -> im requesting the size of the file in RAM
//PROT_READ -> lock the memory to read_only
//MAP_PRIVATE -> if accidentally writen, do not update the file
//canOpen as filedescriptor ticket
//0 -> start reading from byte 0 onward.
//static_cast<const char*> mmap returns void*, must cast the a char ptr to dereference and read.
const char* fileData = static_cast<const char*>(mmap(nullptr, fileSize, PROT_READ, MAP_PRIVATE, canOpen, 0));

//if we dont have enough RAM, prevent seg faults
if (fileData == MAP_FAILED) {
    std::cerr << "Error: mmap failed\n";
    close(canOpen);
    return 1;
}

//start our std::chrono timer
std::cout << "Starting High Speed Ad-Bid Processor\n";
Timer t;

ParseFile(fileData, fileSize, Engine);
double timeElapsed = t.timeElapsed();

std::cout << std::right << std::setw(50) << std::setfill('=') << '\n';
std::cout << std::left << std::setfill(' ') << "Processing Comlete!\n";
std::cout << "Elapsed Time to process Ads: " << timeElapsed << " milliseconds\n";
std::cout << "Total Revenue Processed: $" << std::fixed << std::showpoint << Engine.getRevenue() << '\n';
std::cout << std::right << std::setw(50) << std::setfill('=') << '\n';


//Memory clean up give memory back to OS

munmap((void*)fileData, fileSize);
close(canOpen);

    return 0; 
}