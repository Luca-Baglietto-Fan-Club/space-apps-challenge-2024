import sys
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

if len(sys.argv) != 2:
    print("Usage: python file.py path/to/file.csv")
    sys.exit(1)

csv_file = sys.argv[1]

df = pd.read_csv(csv_file)

minval = float(input("Minvalue: "))
maxval = float(input("Maxvalue: "))
precision = float(input("Precision (in %): ")) / 100

filtered = df[(df.iloc[:, 1] >= minval) & (df.iloc[:, 1] <= maxval)]

sampled = filtered.sample(frac=precision, random_state=1)

xcol = sampled.iloc[:, 1]
ycol = sampled.iloc[:, 2]

plt.figure(figsize=(10, 6))

plt.bar(xcol, ycol, color='blue')

plt.xlabel('Time')
plt.ylabel('Intensity')

plt.title('Godo')

plt.tight_layout()
plt.show()
