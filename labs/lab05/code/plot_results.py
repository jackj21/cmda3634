import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

def plot_run_time(filename):

    df = pd.read_csv(filename, sep=',', header=None)

    fig = plt.figure()
    ax = plt.gca()

    # True is strong study, False is weak
    strong = "strong" in filename
    exp_type = "strong" if strong else "weak"

    # Extract the number of processors
    P = df[1][1:].to_numpy().astype(np.int)

    for c in range(4, len(df.columns)):

        # Slice out the relevant column
        exp = df[c]

        # The header has the plot legend entry
        label = exp[0]

        # The column is the run-time as a function of P
        T_P = exp[1:].to_numpy().astype(np.float32)

        # Create a log-log plot
        ax.loglog(P, T_P, label=label)

    plt.legend()
    plt.title("run-time, " + df[0][1] + ", " + exp_type)
    plt.xlabel("P")
    plt.ylabel("Avg. Run Time (s)")

    return fig

def plot_speedup(filename):

    df = pd.read_csv(filename, sep=',', header=None)

    fig = plt.figure()
    ax = plt.gca()

    # True is strong study, False is weak
    strong = "strong" in filename
    exp_type = "strong" if strong else "weak"

    P = df[1][1:].to_numpy().astype(np.int)
    plt.loglog(P, P, label="ideal speedup", color="black", linestyle="--")

    for c in range(4, len(df.columns)):

        # Slice out the relevant column
        exp = df[c]

        # The header has the plot legend entry
        label = exp[0]

        # The column is the run-time as a function of P
        T_P = exp[1:].to_numpy().astype(np.float32)

        # T_s is the first entry
        T_s = np.float32(exp.iloc[1])

        # Given T_s and knowing the study type, compute T_1
        # (Fill this in)
        if strong:
            # T_1 = ...
        else:
            # T_1 = ...

        # Compute the speedup
        # (Fill this in)
        # S_P = ...

        ax.loglog(P, S_P, label=label)


    plt.legend()
    plt.title("speedup, " + df[0][1] + ", " + exp_type)
    plt.xlabel("P")
    plt.ylabel("S")

    return fig

plot_run_time("vector_strong.csv")
plot_speedup("vector_strong.csv")

plot_run_time("vector_weak.csv")
plot_speedup("vector_weak.csv")

plot_run_time("matrix_strong.csv")
plot_speedup("matrix_strong.csv")

plot_run_time("matrix_weak.csv")
plot_speedup("matrix_weak.csv")

plt.show()
