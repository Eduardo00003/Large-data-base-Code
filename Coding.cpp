#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <execution>
#include <chrono>
#include <numeric>
#include <future>
#include <stdexcept>

using namespace std;

using Row = vector<string>;

// Function to parse a CSV line into a vector of strings
Row parseCSVLine(const string& line, char delimiter = ',') {
    Row row;
    istringstream ss(line);
    string cell;
    while (getline(ss, cell, delimiter)) {
        row.push_back(cell);
    }
    return row;
}

// Function to safely convert a string to a double; returns 0.0 if conversion fails
double toDouble(const string& str) {
    try {
        return stod(str);
    } catch (const exception&) {
        return 0.0;
    }
}

// Function to compute sum and average of a numeric column (index provided)
// Uses parallel algorithms for efficiency
pair<double, double> computeStats(const vector<Row>& dataset, size_t colIndex) {
    // Filter out rows that do not have enough columns
    vector<double> values;
    for (const auto& row : dataset) {
        if (row.size() > colIndex) {
            values.push_back(toDouble(row[colIndex]));
        }
    }
    
    if (values.empty())
        return {0.0, 0.0};
    
    // Use parallel execution policy for computing the sum
    double sum = reduce(execution::par, values.begin(), values.end(), 0.0);
    double average = sum / values.size();
    return {sum, average};
}

int main() {
    const string filename = "large_dataset.csv";
    ifstream infile(filename);
    if (!infile) {
        cerr << "Error: Could not open file " << filename << endl;
        return 1;
    }
    
    vector<Row> dataset;
    string line;
    
    // Optional: read header (if CSV has one)
    bool hasHeader = false;
    Row header;
    if (hasHeader && getline(infile, line)) {
        header = parseCSVLine(line);
    }
    
    // Read the file line by line and parse each line
    while (getline(infile, line)) {
        if (!line.empty()) {
            try {
                dataset.push_back(parseCSVLine(line));
            } catch (const exception& ex) {
                cerr << "Warning: Failed to parse line: " << line << " (" << ex.what() << ")\n";
            }
        }
    }
    infile.close();
    
    cout << "Loaded " << dataset.size() << " rows from " << filename << "\n";
    
    // Sorting the dataset based on the first column using parallel execution
    auto sortStart = chrono::high_resolution_clock::now();
    sort(execution::par, dataset.begin(), dataset.end(),
         [](const Row& a, const Row& b) {
             // Use the first column for sorting, ensure rows are non-empty.
             return (!a.empty() && !b.empty()) ? a.front() < b.front() : false;
         });
    auto sortEnd = chrono::high_resolution_clock::now();
    chrono::duration<double> sortElapsed = sortEnd - sortStart;
    cout << "Sorting completed in " << sortElapsed.count() << " seconds.\n";
    
    // Compute statistics on a numeric column (e.g., the second column, index 1)
    size_t numericColumnIndex = 1;
    auto statsFuture = async(launch::async, computeStats, cref(dataset), numericColumnIndex);
    auto [sum, average] = statsFuture.get();
    cout << "Column " << numericColumnIndex + 1 << " statistics:\n";
    cout << "  Sum: " << sum << "\n";
    cout << "  Average: " << average << "\n";
    
    // Optionally, print the first few sorted rows for verification
    cout << "\nFirst 5 rows after sorting:\n";
    for (size_t i = 0; i < min(dataset.size(), size_t(5)); ++i) {
        for (const auto& cell : dataset[i]) {
            cout << cell << " ";
        }
        cout << "\n";
    }
    
    return 0;
}
