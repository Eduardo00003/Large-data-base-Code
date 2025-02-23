# CSV Processing and Analysis in C++

This repository contains a sample C++ project that demonstrates how to efficiently read, process, and analyze large CSV datasets using modern C++17 features, along with a Python script to generate a sample CSV file.

## Project Overview

- **C++ Code (`main.cpp`):**  
  This program reads a CSV file named `large_dataset.csv`, parses each line into a vector of strings, sorts the dataset in parallel using C++17's parallel algorithms, and computes statistics (sum and average) on a specified numeric column. It includes robust error handling and performance measurement using `std::chrono`.

- **CSV Generator (`generate_csv.py`):**  
  A Python script that creates a sample CSV file with a header and 1,000 rows of data. Each row contains an incremental ID, a random floating-point value, and a descriptive label. This file is used to test and demonstrate the C++ code.

## Getting Started

### Prerequisites

- **C++ Compiler:**  
  A compiler supporting C++17 (e.g., GCC 7 or later, Clang 6 or later, or MSVC 2017 or later).

- **Python:**  
  Python 3.x is required to run the CSV generator script.

### Building and Running the C++ Code

1. **Clone the Repository:**
   ```bash
   git clone https://github.com/yourusername/csv-processing.git
   cd csv-processing
