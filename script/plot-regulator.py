import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
import matplotlib
import sys
from keyname import keyname as kn
import itertools
matplotlib.rcParams['pdf.fonttype'] = 42
matplotlib.rcParams["legend.frameon"] = False
sns.set(style='whitegrid')
sns.set_palette(list(sns.color_palette())[3:])

# import dataset
data_in = pd.read_csv(sys.argv[1])

data_in["Update"] = data_in["update"]
data_in["Problem Difficulty"] = data_in["confused"]
data_in["Best Fitness"] = data_in["max_fitness"]
data_in["Upper Quartile Fitness"] = data_in["upper_quartile_fitness"]
data_in["Selector"] = data_in["selector"]
data_in["Regulator"] = data_in["regulator"]

selectors = sorted(list(data_in["Selector"].unique()))
regulators = sorted(list(data_in["Regulator"].unique()))

for difficulty, ext in itertools.product( (1, 4), (".pdf", ".png") ):

    # data prep: figure out actual grid size in runs
    data_fil = data_in[data_in["Problem Difficulty"] == difficulty].copy()
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

    # make sure we have data from all runs
    assert( len(data_fil[data_fil["Update"] == max_upd]) == 90 )


    # BARPLOT ##################################################################

    # for some reason, doing it this way is necessary to make this work...
    fig, ax1 = plt.subplots(1,1)

    ax1.axhline(
        y=1 - difficulty/9,
        linewidth=1,
        linestyle=":",
        color='k',
    )
    # https://stackoverflow.com/a/51639516
    g = sns.barplot(
        data=data_fil[data_fil["Update"] == data_fil["Update"].max()-1],
        x="Selector",
        y="Best Fitness",
        hue="Regulator",
        ax=ax1
    )
    box = g.get_position()
    g.set_position([box.x0, box.y0, box.width * 0.8, box.height])
    g.legend(
        title="Regulator",
        loc='center right',
        bbox_to_anchor=(1.25, 0.5),
        ncol=1
    )

    # save to disk
    outfile = "depo-consensus+" +  kn.pack({
        "difficult" : difficulty,
        "title" : "barplot",
        "ext" : ext,
    })
    plt.gcf().set_figwidth(11)
    plt.gcf().set_figheight(3)
    plt.savefig(
        outfile,
        transparent=True,
        bbox_inches='tight',
        pad_inches=0,
    )

    plt.clf()

    # for some reason, doing it this way is necessary to make this work...
    fig, ax1 = plt.subplots(1,1)

    plt.gca().axhline(
        y=1 - difficulty/9,
        linewidth=1,
        linestyle=":",
        color='k',
    )
    # https://stackoverflow.com/a/51639516
    g = sns.barplot(
        data=data_fil[data_fil["Update"] == data_fil["Update"].max()],
        x="Selector",
        y="Best Fitness",
        hue="Regulator",
        ax=ax1,
    )
    box = g.get_position()
    g.set_position([box.x0, box.y0, box.width * 0.8, box.height])
    g.legend(
        title="Regulator",
        loc='center right',
        bbox_to_anchor=(1.25, 0.5),
        ncol=1
    )


    # save to disk
    outfile = "depo-consensus+" +  kn.pack({
        "difficulty" : difficulty,
        "title" : "barplot-secondary",
        "ext" : ext,
    })
    plt.gcf().set_figwidth(11)
    plt.gcf().set_figheight(3)
    plt.savefig(
        outfile,
        transparent=True,
        bbox_inches='tight',
        pad_inches=0,
    )

    plt.clf()

    # SWARMPLOT ################################################################

    plt.gca().axhline(
        y=1 - difficulty/9,
        linewidth=1,
        linestyle=":",
        color='k',
    )
    # https://stackoverflow.com/a/51639516
    g = sns.swarmplot(
        data=data_fil[data_fil["Update"] == data_fil["Update"].max()-1],
        x="Selector",
        y="Best Fitness",
        hue="Regulator",
    )
    box = g.get_position()
    g.set_position([box.x0, box.y0, box.width * 0.8, box.height])
    g.legend(
        title="Regulator",
        loc='center right',
        bbox_to_anchor=(1.25, 0.5),
        ncol=1
    )

    # save to disk
    outfile = "depo-consensus+" +  kn.pack({
        "difficulty" : difficulty,
        "title" : "swarmplot",
        "ext" : ext,
    })
    plt.gcf().set_figwidth(11)
    plt.gcf().set_figheight(3)
    plt.savefig(
        outfile,
        transparent=True,
        bbox_inches='tight',
        pad_inches=0,
    )

    plt.clf()

    plt.gca().axhline(
        y=1 - difficulty/9,
        linewidth=1,
        linestyle=":",
        color='k',
    )
    # https://stackoverflow.com/a/51639516
    g = sns.swarmplot(
        data=data_fil[data_fil["Update"] == data_fil["Update"].max()],
        x="Selector",
        y="Best Fitness",
        hue="Regulator",
    )
    box = g.get_position()
    g.set_position([box.x0, box.y0, box.width * 0.8, box.height])
    g.legend(
        title="Regulator",
        loc='center right',
        bbox_to_anchor=(1.25, 0.5),
        ncol=1
    )

    # save to disk
    outfile = "depo-consensus+" +  kn.pack({
        "difficulty" : difficulty,
        "title" : "swarmplot-secondary",
        "ext" : ext,
    })
    plt.gcf().set_figwidth(11)
    plt.gcf().set_figheight(3)
    plt.savefig(
        outfile,
        transparent=True,
        bbox_inches='tight',
        pad_inches=0,
    )

    plt.clf()

    # COUNTPLOT ################################################################

    # https://stackoverflow.com/a/51639516
    g = sns.countplot(
        data=data_fil[
                (data_fil["Update"] == data_fil["Update"].max()-1)
                & (data_fil["Best Fitness"] == 1)
            ],
        x="Selector",
        order=selectors,
        hue="Regulator",
        hue_order=regulators,
    )
    box = g.get_position()
    g.set_position([box.x0, box.y0, box.width * 0.8, box.height])
    g.legend(
        title="Regulator",
        loc='center right',
        bbox_to_anchor=(1.25, 0.5),
        ncol=1
    )
    g.set_ylabel("Perfect Solution Count")


    # save to disk
    outfile = "depo-consensus+" +  kn.pack({
        "difficulty" : difficulty,
        "title" : "countplot",
        "ext" : ext,
    })
    plt.gcf().set_figwidth(11)
    plt.gcf().set_figheight(3)
    plt.savefig(
        outfile,
        transparent=True,
        bbox_inches='tight',
        pad_inches=0,
    )

    plt.clf()

    # https://stackoverflow.com/a/51639516
    g = sns.countplot(
        data=data_fil[
                (data_fil["Update"] == data_fil["Update"].max())
                & (data_fil["Best Fitness"] == 1)
            ],
        x="Selector",
        order=selectors,
        hue="Regulator",
        hue_order=regulators,
    )
    box = g.get_position()
    g.set_position([box.x0, box.y0, box.width * 0.8, box.height])
    g.legend(
        title="Regulator",
        loc='center right',
        bbox_to_anchor=(1.25, 0.5),
        ncol=1
    )
    g.set_ylabel("Perfect Solution Count")

    # save to disk
    outfile = "depo-consensus+" +  kn.pack({
        "difficulty" : difficulty,
        "title" : "countplot-secondary",
        "ext" : ext,
    })
    plt.gcf().set_figwidth(11)
    plt.gcf().set_figheight(3)
    plt.savefig(
        outfile,
        transparent=True,
        bbox_inches='tight',
        pad_inches=0,
    )

    plt.clf()

    # LINEPLOT #################################################################


    g = sns.FacetGrid(
        data=data_fil[
            (data_fil["Update"].mod(100) == 0)
            & (data_fil["Update"] < data_fil["Update"].max()-1)
        ],
        col="Regulator",
        row="Selector",
        hue="Regulator",
        margin_titles=True,
    )

    g.map( # **_ means ignore kwargs
        lambda **_: plt.gca().axhline(
            y=1 - difficulty/9,
            linewidth=1,
            linestyle=":",
            color='k',
        )
    )
    g.map(
        sns.lineplot,
        "Update",
        "Upper Quartile Fitness",
    )

    # save to disk
    outfile = "depo-consensus+" +  kn.pack({
        "difficulty" : difficulty,
        "title" : "lineplot",
        "ext" : ext,
    })
    plt.savefig(
        outfile,
        transparent=True,
        bbox_inches='tight',
        pad_inches=0,
    )

    plt.clf()

    # FACET BARPLOT ############################################################

    g = sns.FacetGrid(
        data=data_fil[data_fil["Update"] > data_fil["Update"].max()-2],
        col="Selector",
        margin_titles=True,
    )
    g.map( # **_ means ignore kwargs
        lambda **_: plt.gca().axhline(
            y=1 - difficulty/9,
            linewidth=1,
            linestyle=":",
            color='k',
        )
    )
    g.map(
        sns.barplot,
        "Regulator",
        "Best Fitness",
        "Evaluation Grid Dimensions",
        order=regulators,
        hue_order=list(map(lambda x: specific[x], ["small", "big"])),
        palette=list(reversed(sns.color_palette())),
        dodge=True,
    )
    g.add_legend(title="Evaluation Grid\nDimensions")

    # save to disk
    outfile = "depo-consensus+" +  kn.pack({
        "difficulty" : difficulty,
        "title" : "facetbarplot",
        "ext" : ext,
    })
    plt.gcf().set_figwidth(11)
    plt.gcf().set_figheight(3)
    plt.savefig(
        outfile,
        transparent=True,
        bbox_inches='tight',
        pad_inches=0,
    )

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
        sns.countplot(
            x=x, hue=hue, **kwargs
        ).set_ylabel(
            "Perfect Solution Count"
        )
    g.map(
        countplot,
        "Regulator",
        "Evaluation Grid Dimensions",
        order=regulators,
        hue_order=list(map(lambda x: specific[x], ["small", "big"])),
        palette=list(reversed(sns.color_palette())),
        dodge=True,
    )
    g.add_legend(title="Evaluation Grid\nDimensions")

    # save to disk
    outfile = "depo-consensus+" + kn.pack({
        "difficulty" : difficulty,
        "title" : "facetcountplot",
        "ext" : ext,
    })
    plt.gcf().set_figwidth(11)
    plt.gcf().set_figheight(3)
    plt.savefig(
        outfile,
        transparent=True,
        bbox_inches='tight',
        pad_inches=0,
    )

    plt.clf()
