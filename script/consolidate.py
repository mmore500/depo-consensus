from keyname import keyname as kn
import sys
import pandas as pd

filenames = sys.argv[1:]

dataframes = []

for filename in filenames:
    df = pd.read_csv(filename)

    for k, v in kn.unpack(filename).items():
        df[k] = v

    dataframes.append(df)

res = pd.concat(
    dataframes,
    ignore_index=True
)

res.to_csv("consolidated.csv", index=False)
