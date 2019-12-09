import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
import sys

# import dataset
data = pd.read_csv(sys.argv[1])

data["Update"] = data["update"]
data["Problem Difficulty"] = data["confused"]
data["Upper Quartile Fitness"] = data["upper_quartile_fitness"]
data["Selector"] = data["selector"]

sns.barplot(
    data=data[data["Update"] == 999],
    x="Problem Difficulty",
    y="Upper Quartile Fitness",
    hue="Selector",
)

# save to disk
plt.savefig("depo-consensus-barplot.pdf")

plt.clf()


sns.swarmplot(
    data=data[data["Update"] == 999],
    x="Problem Difficulty",
    y="Upper Quartile Fitness",
    hue="Selector",
)

# save to disk
plt.savefig("depo-consensus-swarmplot.pdf")

plt.clf()

sns.countplot(
    data=data[
            (data["Update"] == 999)
            & (data["Upper Quartile Fitness"] == 1)
        ],
    x="Problem Difficulty",
    hue="Selector",
)

# save to disk
plt.savefig("depo-consensus-countplot.pdf")