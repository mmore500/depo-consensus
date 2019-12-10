import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
import matplotlib
import sys

matplotlib.rcParams['pdf.fonttype'] = 42
sns.set(style='whitegrid')

# import dataset
data = pd.read_csv(sys.argv[1])

data["Update"] = data["update"]
data["Problem Difficulty"] = data["confused"]
data["Best Fitness"] = data["max_fitness"]
data["Upper Quartile Fitness"] = data["upper_quartile_fitness"]
data["Selector"] = data["selector"]

sns.barplot(
    data=data[data["Update"] == data["Update"].max()],
    x="Problem Difficulty",
    y="Best Fitness",
    hue="Selector",
)

# save to disk
plt.savefig("depo-consensus-barplot.pdf")

plt.clf()


sns.swarmplot(
    data=data[data["Update"] == data["Update"].max()],
    x="Problem Difficulty",
    y="Best Fitness",
    hue="Selector",
)

# save to disk
plt.savefig("depo-consensus-swarmplot.pdf")

plt.clf()

sns.countplot(
    data=data[
            (data["Update"] == data["Update"].max())
            & (data["Best Fitness"] == 1)
        ],
    x="Problem Difficulty",
    hue="Selector",
)

# save to disk
plt.savefig("depo-consensus-countplot.pdf")
