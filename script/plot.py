import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
import matplotlib
import sys
from keyname import keyname as kn
import itertools
matplotlib.rcParams['pdf.fonttype'] = 42
sns.set(style='whitegrid')

fig, ax1 = plt.subplots(1,1)

# import dataset
data_in = pd.read_csv(sys.argv[1])

data_in["Update"] = data_in["update"]
data_in["Problem Difficulty"] = data_in["confused"]
data_in["Best Fitness"] = data_in["max_fitness"]
data_in["Upper Quartile Fitness"] = data_in["upper_quartile_fitness"]
data_in["Selector"] = data_in["selector"]

for size, ext in itertools.product( ("big", "small"), (".pdf", ".png") ):

    # data prep: figure out actual grid size in runs
    data_fil = data_in[data_in["size"] == size].copy()
    opposite = { "big" : "small", "small" : "big" }
    specific = { "small" : "$3 \\times 3$", "big" : "$9 \\times 9$" }
    max_upd = data_fil["Update"].max()
    data_fil["Evaluation Grid Dimensions"] = data_fil.apply(
        lambda row: (
            specific[opposite[row["size"]]]
            if row["Update"] == max_upd
            else specific[row["size"]]
        ),
        axis=1
    )

    # BARPLOT ##################################################################

    # https://stackoverflow.com/a/51639516
    g = sns.barplot(
        data=data_fil[data_fil["Update"] == data_fil["Update"].max()-1],
        x="Problem Difficulty",
        y="Best Fitness",
        hue="Selector",
        ax=ax1
    )
    box = g.get_position()
    g.set_position([box.x0, box.y0, box.width * 0.85, box.height])
    g.legend(loc='center right', bbox_to_anchor=(1.3, 0.5), ncol=1)

    # save to disk
    outfile = kn.pack({
        "size" : size,
        "title" : "depo-consensus-barplot",
        "ext" : ext,
    })
    plt.savefig(outfile)

    plt.clf()

    # https://stackoverflow.com/a/51639516
    g = sns.barplot(
        data=data_fil[data_fil["Update"] == data_fil["Update"].max()],
        x="Problem Difficulty",
        y="Best Fitness",
        hue="Selector",
        ax=ax1
    )
    box = g.get_position()
    g.set_position([box.x0, box.y0, box.width * 0.85, box.height])
    g.legend(loc='center right', bbox_to_anchor=(1.3, 0.5), ncol=1)


    # save to disk
    outfile = kn.pack({
        "size" : size,
        "title" : "depo-consensus-barplot-secondary",
        "ext" : ext,
    })
    plt.savefig(outfile)

    plt.clf()

    # SWARMPLOT ################################################################

    # https://stackoverflow.com/a/51639516
    g = sns.swarmplot(
        data=data_fil[data_fil["Update"] == data_fil["Update"].max()-1],
        x="Problem Difficulty",
        y="Best Fitness",
        hue="Selector",
    )
    box = g.get_position()
    g.set_position([box.x0, box.y0, box.width * 0.85, box.height])
    g.legend(loc='center right', bbox_to_anchor=(1.3, 0.5), ncol=1)

    # save to disk
    outfile = kn.pack({
        "size" : size,
        "title" : "depo-consensus-swarmplot",
        "ext" : ext,
    })
    plt.savefig(outfile)

    plt.clf()

    # https://stackoverflow.com/a/51639516
    g = sns.swarmplot(
        data=data_fil[data_fil["Update"] == data_fil["Update"].max()],
        x="Problem Difficulty",
        y="Best Fitness",
        hue="Selector",
    )
    box = g.get_position()
    g.set_position([box.x0, box.y0, box.width * 0.85, box.height])
    g.legend(loc='center right', bbox_to_anchor=(1.3, 0.5), ncol=1)

    # save to disk
    outfile = kn.pack({
        "size" : size,
        "title" : "depo-consensus-swarmplot-secondary",
        "ext" : ext,
    })
    plt.savefig(outfile)

    plt.clf()

    # COUNTPLOT ################################################################

    # https://stackoverflow.com/a/51639516
    g = sns.countplot(
        data=data_fil[
                (data_fil["Update"] == data_fil["Update"].max()-1)
                & (data_fil["Best Fitness"] == 1)
            ],
        x="Problem Difficulty",
        hue="Selector",
    )
    box = g.get_position()
    g.set_position([box.x0, box.y0, box.width * 0.85, box.height])
    g.legend(loc='center right', bbox_to_anchor=(1.3, 0.5), ncol=1)

    # save to disk
    outfile = kn.pack({
        "size" : size,
        "title" : "depo-consensus-countplot",
        "ext" : ext,
    })
    plt.savefig(outfile)

    plt.clf()

    # https://stackoverflow.com/a/51639516
    g = sns.countplot(
        data=data_fil[
                (data_fil["Update"] == data_fil["Update"].max())
                & (data_fil["Best Fitness"] == 1)
            ],
        x="Problem Difficulty",
        hue="Selector",
    )
    box = g.get_position()
    g.set_position([box.x0, box.y0, box.width * 0.85, box.height])
    g.legend(loc='center right', bbox_to_anchor=(1.3, 0.5), ncol=1)

    # save to disk
    outfile = kn.pack({
        "size" : size,
        "title" : "depo-consensus-countplot-secondary",
        "ext" : ext,
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
        "ext" : ext,
    })
    plt.savefig(outfile)

    plt.clf()

    # FACET BARPLOT ############################################################

    g = sns.FacetGrid(
        data=data_fil[data_fil["Update"] > data_fil["Update"].max()-2],
        col="Selector",
        margin_titles=True,
    )
    g.map(
        sns.barplot,
        "Problem Difficulty",
        "Best Fitness",
        "Evaluation Grid Dimensions",
        hue_order=list(map(lambda x: specific[x], ["small", "big"])),
        palette=sns.color_palette(),
        dodge=True,
    )
    g.add_legend(title="Evaluation Grid Dimensions")

    # save to disk
    outfile = kn.pack({
        "size" : size,
        "title" : "depo-consensus-facetbarplot",
        "ext" : ext,
    })
    plt.savefig(outfile)

    plt.clf()

    # FACET COUNTPLOT ##########################################################

    g = sns.FacetGrid(
        data=data_fil[
            (data_fil["Update"] > data_fil["Update"].max()-2)
            & (data_fil["Best Fitness"] == 1)
        ],
        col="Selector",
        margin_titles=True,
    )
    # https://stackoverflow.com/a/44035405
    def countplot(x, hue, **kwargs):
        sns.countplot(x=x, hue=hue, **kwargs)
    g.map(
        countplot,
        "Problem Difficulty",
        "Evaluation Grid Dimensions",
        hue_order=list(map(lambda x: specific[x], ["small", "big"])),
        palette=sns.color_palette(),
        dodge=True,
    )
    g.add_legend(title="Evaluation Grid Dimensions")

    # save to disk
    outfile = kn.pack({
        "size" : size,
        "title" : "depo-consensus-facetcountplot",
        "ext" : ext,
    })
    plt.savefig(outfile)

    plt.clf()
