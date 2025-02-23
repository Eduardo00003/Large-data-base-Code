import csv
import random

# Open the CSV file for writing
with open("large_dataset.csv", "w", newline="") as csvfile:
    writer = csv.writer(csvfile)
    # Write header row
    writer.writerow(["ID", "Value", "Description"])
    # Write 1000 rows of data
    for i in range(1, 1001):
        # Generate a random floating-point number between 0 and 100 with 2 decimal places
        value = round(random.uniform(0, 100), 2)
        writer.writerow([i, value, f"Item {i}"])
