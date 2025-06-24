# Longest UTXO Chain

This project implements an algorithm to find the longest chain of UTXOs (Unspent Transaction Outputs) in systems similar to Bitcoin. It is written in C++ and is based on the concept of transactions with one input and two outputs.

## How to Run

### Running with g++

1. **Navigate to the Source Directory:**
   Open a terminal and navigate to the `src` directory.

2. **Compile the Project:**
   ```bash
   g++ -std=c++17 -O2 main.cpp -o longest_utxo_chain
   ```

3. **Run the Executable:**
   Provide the path to the CSV file with transactions:
   ```bash
   ./longest_utxo_chain ../data/transactions.csv
   ```

### Running in Visual Studio (Windows)

1. **Open the Project:**
   Open the project in Visual Studio.

2. **Configure C++ Standard:**
   - Go to **Project Properties** > **Configuration Properties** > **C/C++ > Language**.
   - Set the language standard to **ISO C17 (/std:c17)**.

3. **Configure and Run:**
   - Configure the project if needed.
   - Run the executable, passing the CSV file path as a command-line argument.

## How to Configure

1. **CSV File with Transactions:**
   - The file must be formatted as follows:
     ```
     txid,input,output1,output2
     ```
   - Each line represents a transaction with a unique `txid`, one input, and two outputs. You can edit or create transaction files in the `data` directory for testing.

2. **Compilation Settings:**
   - Ensure that you compile with the C++17 standard enabled:
     ```bash
     g++ -std=c++17 -O2 main.cpp -o longest_utxo_chain
     ```

## Additional Information

1. **Algorithm Overview:**
   - The project builds a transaction graph linking each transaction via shared outputs.
   - It uses Depth-First Search (DFS) with memoization (using `std::optional`) to find the longest chain of transactions.

2. **Potential Extensions:**
   - Add filters, integrate real blockchain data, or expand the project with logging and testing modules.

3. **Complexity Analysis:**
   - **Time Complexity:** O(n), where *n* is the number of transactions. Both transaction filtering and graph construction run in linear time, and DFS with memoization visits each transaction only once.
   - **Space Complexity:** O(n), due to the use of vectors and hash tables.