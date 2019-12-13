import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
import matplotlib
import sys
from keyname import keyname as kn
matplotlib.rcParams['pdf.fonttype'] = 42
sns.set(style='whitegrid')

# import dataset
data_in = pd.read_csv(sys.argv[1])

data_in["Update"] = data_in["update"]
data_in["Problem Difficulty"] = data_in["confused"]
data_in["Best Fitness"] = data_in["max_fitness"]
data_in["Upper Quartile Fitness"] = data_in["upper_quartile_fitness"]
data_in["Selector"] = data_in["selector"]

for size in "big", "small":

    data_fil = data_in[data_in["size"] == size]

    # BARPLOT ##################################################################

    sns.barplot(
        data=data_fil[data_fil["Update"] == data_fil["Update"].max()-1],
        x="Problem Difficulty",
        y="Best Fitness",
        hue="Selector",
    )

    # save to disk
    outfile = kn.pack({
        "size" : size,
        "title" : "depo-consensus-barplot",
        "ext" : ".pdf",
    })
    plt.savefig(outfile)

    plt.clf()

    sns.barplot(
        data=data_fil[data_fil["Update"] == data_fil["Update"].max()],
        x="Problem Difficulty",
        y="Best Fitness",
        hue="Selector",
        ax=ax1
    )

    # save to disk
    outfile = kn.pack({
        "size" : size,
        "title" : "depo-consensus-barplot-secondary",
        "ext" : ".pdf",
    })
    plt.savefig(outfile)

    plt.clf()

    # SWARMPLOT ################################################################

    sns.swarmplot(
        data=data_fil[data_fil["Update"] == data_fil["Update"].max()-1],
        x="Problem Difficulty",
        y="Best Fitness",
        hue="Selector",
    )

    # save to disk
    outfile = kn.pack({
        "size" : size,
        "title" : "depo-consensus-swarmplot",
        "ext" : ".pdf",
    })
    plt.savefig(outfile)

    plt.clf()

    sns.swarmplot(
        data=data_fil[data_fil["Update"] == data_fil["Update"].max()],
        x="Problem Difficulty",
        y="Best Fitness",
        hue="Selector",
    )

    # save to disk
    outfile = kn.pack({
        "size" : size,
        "title" : "depo-consensus-swarmplot-secondary",
        "ext" : ".pdf",
    })
    plt.savefig(outfile)

    plt.clf()

    # COUNTPLOT ################################################################

    sns.countplot(
        data=data_fil[
                (data_fil["Update"] == data_fil["Update"].max()-1)
                & (data_fil["Best Fitness"] == 1)
            ],
        x="Problem Difficulty",
        hue="Selector",
    )

    # save to disk
    outfile = kn.pack({
        "size" : size,
        "title" : "depo-consensus-countplot",
        "ext" : ".pdf",
    })
    plt.savefig(outfile)

    plt.clf()

    sns.countplot(
        data=data_fil[
                (data_fil["Update"] == data_fil["Update"].max())
                & (data_fil["Best Fitness"] == 1)
            ],
        x="Problem Difficulty",
        hue="Selector",
    )

    # save to disk
    outfile = kn.pack({
        "size" : size,
        "title" : "depo-consensus-countplot-secondary",
        "ext" : ".pdf",
    })
    plt.savefig(outfile)

    plt.clf()

    # LINEPLOT #################################################################


    g = sns.FacetGrid(
        data=data_fil[
            (data_fil["Update"].mod(100) == 0)
            & (data_fil["Update"] < data_fil["Update"].max()-1)
        ],
        col="Problem Difficulty",
        row="Selector",
        hue="Selector",
        margin_titles=True,
    )

    g.map(
        sns.lineplot,
        "Update",
        "Upper Quartile Fitness",
    )

    # save to disk
    outfile = kn.pack({
        "size" : size,
        "title" : "depo-consensus-lineplot",
        "ext" : ".pdf",
    })
    plt.savefig(outfile)

    plt.clf()
