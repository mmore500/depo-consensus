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

from scipy import stats

print(
    stats.kruskal(
        data_in[
            (data_in["Selector"] == "depo")
            & (data_in["size"] == "small")
            & (data_in["Problem Difficulty"] == 2)
            & (data_in["update"] == 2500)
        ]["Best Fitness"],
        data_in[
            (data_in["Selector"] == "ranked")
            & (data_in["size"] == "small")
            & (data_in["Problem Difficulty"] == 2)
            & (data_in["update"] == 2500)
        ]["Best Fitness"],
    )
)

print(
    stats.kruskal(
        data_in[
            (data_in["Selector"] == "depo")
            & (data_in["size"] == "small")
            & (data_in["Problem Difficulty"] == 2)
            & (data_in["update"] == 2500)
        ]["Best Fitness"],
        data_in[
            (data_in["Selector"] == "sieve")
            & (data_in["size"] == "small")
            & (data_in["Problem Difficulty"] == 2)
            & (data_in["update"] == 2500)
        ]["Best Fitness"],
    )
)

print(
    stats.kruskal(
        data_in[
            (data_in["Selector"] == "depo")
            & (data_in["size"] == "big")
            & (data_in["Problem Difficulty"] == 9)
            & (data_in["update"] == 1000)
        ]["Best Fitness"],
        data_in[
            (data_in["Selector"] == "sieve")
            & (data_in["size"] == "big")
            & (data_in["Problem Difficulty"] == 9)
            & (data_in["update"] == 1000)
        ]["Best Fitness"],
    )
)

print(
    stats.kruskal(
        data_in[
            (data_in["Selector"] == "depo")
            & (data_in["size"] == "big")
            & (data_in["Problem Difficulty"] == 9)
            & (data_in["update"] == 1000)
        ]["Best Fitness"],
        data_in[
            (data_in["Selector"] == "ranked")
            & (data_in["size"] == "big")
            & (data_in["Problem Difficulty"] == 9)
            & (data_in["update"] == 1000)
        ]["Best Fitness"],
    )
)

print(
    stats.kruskal(
        data_in[
            (data_in["Selector"] == "depo")
            & (data_in["size"] == "big")
            & (data_in["Problem Difficulty"] == 18)
            & (data_in["update"] == 1000)
        ]["Best Fitness"],
        data_in[
            (data_in["Selector"] == "sieve")
            & (data_in["size"] == "big")
            & (data_in["Problem Difficulty"] == 18)
            & (data_in["update"] == 1000)
        ]["Best Fitness"],
    )
)

print(
    stats.kruskal(
        data_in[
            (data_in["Selector"] == "depo")
            & (data_in["size"] == "big")
            & (data_in["Problem Difficulty"] == 18)
            & (data_in["update"] == 1000)
        ]["Best Fitness"],
        data_in[
            (data_in["Selector"] == "ranked")
            & (data_in["size"] == "big")
            & (data_in["Problem Difficulty"] == 18)
            & (data_in["update"] == 1000)
        ]["Best Fitness"],
    )
)

print(
    stats.kruskal(
        data_in[
            (data_in["Selector"] == "ranked")
            & (data_in["size"] == "small")
            & (data_in["Problem Difficulty"] == 4)
            & (data_in["update"] == 2500)
        ]["Best Fitness"],
        data_in[
            (data_in["Selector"] == "sieve")
            & (data_in["size"] == "small")
            & (data_in["Problem Difficulty"] == 4)
            & (data_in["update"] == 2500)
        ]["Best Fitness"],
    )
)

print(
    stats.kruskal(
        data_in[
            (data_in["Selector"] == "ranked")
            & (data_in["size"] == "big")
            & (data_in["Problem Difficulty"] == 36)
            & (data_in["update"] == 1000)
        ]["Best Fitness"],
        data_in[
            (data_in["Selector"] == "sieve")
            & (data_in["size"] == "big")
            & (data_in["Problem Difficulty"] == 36)
            & (data_in["update"] == 1000)
        ]["Best Fitness"],
    )
)

for difficulty in range(1,5):
    for selector in 'sieve', 'depo', 'ranked':
        fil = data_in[
            (data_in["Selector"] == selector)
            & (data_in["size"] == "small")
            & (data_in["Problem Difficulty"] == difficulty)
        ]

        either = 0

        both = 0

        observed_a = 0

        observed_b = 0

        assert(len(fil["seed"].unique()) == 10)

        for seed in fil["seed"].unique():

            fil2 = fil[fil["seed"] == seed]

            assert(len(fil2[fil2["Update"] == 2500]["Best Fitness"]) == 1)

            assert(len(fil2[fil2["Update"] == 2501]["Best Fitness"]) == 1)

            if (
                fil2[fil2["Update"] == 2500]["Best Fitness"].max() == 1
                or
                fil2[fil2["Update"] == 2501]["Best Fitness"].max() == 1
            ):
                either += 1

            if (
                fil2[fil2["Update"] == 2500]["Best Fitness"].max() == 1
                and
                fil2[fil2["Update"] == 2501]["Best Fitness"].max() == 1
            ):
                both += 1

            if (
                fil2[fil2["Update"] == 2500]["Best Fitness"].max() == 1
            ):
                observed_a += 1

            if (
                fil2[fil2["Update"] == 2501]["Best Fitness"].max() == 1
            ):
                observed_b += 1

        print()
        print("observed_a", observed_a)
        print("observed_b", observed_b)
        print("both", both)
        print("either", either)

        assert( both == min(observed_a, observed_b) )
        assert( either == max(observed_a, observed_b) )

for difficulty in range(9,9*5, 9):
    for selector in 'sieve', 'depo', 'ranked':
        fil = data_in[
            (data_in["Selector"] == selector)
            & (data_in["size"] == "big")
            & (data_in["Problem Difficulty"] == difficulty)
        ]

        either = 0

        both = 0

        observed_a = 0

        observed_b = 0

        assert(len(fil["seed"].unique()) == 10)

        for seed in fil["seed"].unique():

            fil2 = fil[fil["seed"] == seed]

            assert(len(fil2[fil2["Update"] == 1000]["Best Fitness"]) == 1)

            assert(len(fil2[fil2["Update"] == 1001]["Best Fitness"]) == 1)

            if (
                fil2[fil2["Update"] == 1000]["Best Fitness"].max() == 1
                or
                fil2[fil2["Update"] == 1001]["Best Fitness"].max() == 1
            ):
                either += 1

            if (
                fil2[fil2["Update"] == 1000]["Best Fitness"].max() == 1
                and
                fil2[fil2["Update"] == 1001]["Best Fitness"].max() == 1
            ):
                both += 1

            if (
                fil2[fil2["Update"] == 1000]["Best Fitness"].max() == 1
            ):
                observed_a += 1

            if (
                fil2[fil2["Update"] == 1001]["Best Fitness"].max() == 1
            ):
                observed_b += 1

        print()
        print("observed_a", observed_a)
        print("observed_b", observed_b)
        print("both", both)
        print("either", either)

        print( both == min(observed_a, observed_b) )
        print( either == max(observed_a, observed_b) )
