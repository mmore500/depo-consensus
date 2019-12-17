import numpy as np
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

from itertools import zip_longest

def grouper(iterable, n, fillvalue=None):
    args = [iter(iterable)] * n
    return zip_longest(*args, fillvalue=fillvalue)

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

    # make sure that we have data from all runs
    assert( len(data_fil[data_fil["Update"] == max_upd]) == 150 )

    # BARPLOT ##################################################################

    # for some reason, doing it this way is necessary to make this work...
    fig, ax1 = plt.subplots(1,1)

    # https://stackoverflow.com/a/51639516
    g = sns.barplot(
        data=data_fil[data_fil["Update"] == data_fil["Update"].max()-1],
        x="Problem Difficulty",
        y="Best Fitness",
        hue="Selector",
        ax=ax1
    )
    xs = sorted(p.get_x() for p in ax1.patches)
    width = ax1.patches[0].get_width()
    for i, (a, b, c) in enumerate(grouper(xs, 3)):
        plt.plot(
            (a, c+width),
            (1- i/9, 1- i/9),
            'k:'
        )
    box = g.get_position()
    g.set_position([box.x0, box.y0, box.width * 0.85, box.height])
    g.legend(
        title="Selector",
        loc='center right',
        bbox_to_anchor=(1.2, 0.5),
        ncol=1
    )

    # save to disk
    outfile = "depo-consensus+" +  kn.pack({
        "size" : size,
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

    # SWARMPLOT ################################################################

    # https://stackoverflow.com/a/51639516
    g = sns.swarmplot(
        data=data_fil[data_fil["Update"] == data_fil["Update"].max()-1],
        x="Problem Difficulty",
        y="Best Fitness",
        hue="Selector",
    )
    for i, (a, b) in enumerate(zip(
        np.linspace(*plt.gca().get_xlim(), num=6),
        np.linspace(*plt.gca().get_xlim(), num=6)[1:]
    )):
        plt.plot(
            (a, b),
            (1- i/9, 1- i/9),
            'k:'
        )
    box = g.get_position()
    g.set_position([box.x0, box.y0, box.width * 0.85, box.height])
    g.legend(
        title="Selector",
        loc='center right',
        bbox_to_anchor=(1.2, 0.5),
        ncol=1
    )

    # save to disk
    outfile = "depo-consensus+" +  kn.pack({
        "size" : size,
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
    g.legend(
        title="Selector",
        loc='center right',
        bbox_to_anchor=(1.2, 0.5),
        ncol=1
    )
    g.set_ylabel("Perfect Solution Count")


    # save to disk
    outfile = "depo-consensus+" +  kn.pack({
        "size" : size,
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
        lambda difficulty, size, **_: plt.gca().axhline(
            y=1 - difficulty.iloc[0] / { "big" : 81, "small" : 9}[size.iloc[0]],
            linewidth=1,
            linestyle=":",
            color='k',
        ),
        "Problem Difficulty",
        "size",
    )
    g.map(
        sns.lineplot,
        "Update",
        "Upper Quartile Fitness",
    )

    # save to disk
    outfile = "depo-consensus+" +  kn.pack({
        "size" : size,
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
    def draw_barplot_baselines(**_):
        xs = sorted(p.get_x() for p in plt.gca().patches)
        width = plt.gca().patches[0].get_width()
        for i, (a, c) in enumerate(grouper(xs, 2)):
            plt.plot(
                (a, c+width),
                (1- i/9, 1- i/9),
                'k:'
            )
    g.map(
        sns.barplot,
        "Problem Difficulty",
        "Best Fitness",
        "Evaluation Grid Dimensions",
        hue_order=list(map(lambda x: specific[x], ["small", "big"])),
        palette=list(reversed(sns.color_palette())),
        dodge=True,
    )
    g.map(draw_barplot_baselines)
    g.add_legend(title="Evaluation Grid\nDimensions")
    g.axes.flat[0].set_ylabel("Best Fitness")
    for ax in g.axes.flat:
        ax.set_xlabel("Problem Difficulty")

    # save to disk
    outfile = "depo-consensus+" +  kn.pack({
        "size" : size,
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
        )
    g.map(
        countplot,
        "Problem Difficulty",
        "Evaluation Grid Dimensions",
        hue_order=list(map(lambda x: specific[x], ["small", "big"])),
        palette=list(reversed(sns.color_palette())),
        dodge=True,
    )
    g.add_legend(title="Evaluation Grid\nDimensions")
    g.axes.flat[0].set_ylabel("Perfect Solution Count")

    # save to disk
    outfile = "depo-consensus+" + kn.pack({
        "size" : size,
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
