#!/usr/bin/python3

import random
import time
import sys

def generate_bids(filename, num_rows):
    # Simulate a realistic environment with 10k users and 100 ad campaigns
    users = range(1, 10001)       
    campaigns = range(1, 101)     
    
    print(f"Generating {num_rows:,} rows of data into {filename}...")
    start_time = time.time()

    with open(filename, 'w') as f:
        chunk_size = 100_000
        chunk = []
        
        # Start timestamp (simulating a point in time in milliseconds)
        current_time = int(time.time() * 1000) 
        
        for i in range(1, num_rows + 1):
            # Advance the timestamp by 1-5 milliseconds per bid
            timestamp = current_time + i * random.randint(1, 5) 
            user_id = random.choice(users)
            campaign_id = random.choice(campaigns)
            
            # Generate a bid between $0.10 and $5.50
            bid_amount = round(random.uniform(0.10, 5.50), 2)
            
            # Format: Timestamp, UserID, CampaignID, BidAmount
            chunk.append(f"{timestamp},{user_id},{campaign_id},{bid_amount}\n")
            
            # Write to disk in batches to keep the script running fast
            if i % chunk_size == 0:
                f.writelines(chunk)
                chunk.clear()
                sys.stdout.write(f"\rProgress: {i / num_rows * 100:.0f}%")
                sys.stdout.flush()

        # Write any remaining data in the final chunk
        if chunk:
            f.writelines(chunk)

    print(f"\nDone! Generated {filename} in {time.time() - start_time:.2f} seconds.")

if __name__ == "__main__":
    # 10,000,000 rows will create a file roughly 300MB in size
    generate_bids("bids_data.csv", 10_000_000)
