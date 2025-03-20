import numpy as np
import pandas as pd

import matplotlib.pyplot as plt

#provided function to plot a roofline model.
def plot_roofline(l1_bw=242.7, l2_bw=174.5, dram_bw=40.2, peak_perf=47.3):
    """
    Plots an Empirical Roofline Model with customizable L1, L2, and DRAM bandwidths, 
    as well as peak performance.
    
    Parameters:
        l1_bw (float): L1 cache bandwidth in GB/s.
        l2_bw (float): L2 cache bandwidth in GB/s.
        dram_bw (float): DRAM bandwidth in GB/s.
        peak_perf (float): Peak floating point performance in GFLOPs/sec.
    """
    # Define FLOP/Byte operational intensity range
    intensity = np.logspace(-2.2, 2, 100)  # Log scale from 0.01 to 100

    # Compute roofline limits
    l1_limit = np.minimum(intensity * l1_bw, peak_perf)
    l2_limit = np.minimum(intensity * l2_bw, peak_perf)
    dram_limit = np.minimum(intensity * dram_bw, peak_perf)

    # Plot setup
    fig, ax = plt.subplots(figsize=(14, 8))
    ax.set_xscale("log")
    ax.set_yscale("log")
    ax.set_xlabel("FLOPs / Byte")
    ax.set_ylabel("GFLOPs / sec")
    ax.set_title("Empirical Roofline Model")

    # Plot the roofline bands
    ax.plot(intensity, l1_limit, label=f"L1 - {l1_bw} GB/s", linestyle='-', linewidth=2, color='r')
    ax.plot(intensity, l2_limit, label=f"L2 - {l2_bw} GB/s", linestyle='-', linewidth=2, color='g')
    ax.plot(intensity, dram_limit, label=f"DRAM - {dram_bw} GB/s", linestyle='-', linewidth=2, color='b')

    # Peak Performance Line
    ax.axhline(y=peak_perf, color='black', linestyle='--', linewidth=2, label=f"Peak Perf: {peak_perf} GFLOPs/s")

    # Grid and legend
    ax.grid(True, which="both", linestyle="--", linewidth=0.5)
    ax.legend()

    return ax 

#loading data
perf_df = pd.read_csv("agoge_perf_data.csv")
vtune_df = pd.read_csv("agoge_vtune_data.csv")

#splitting into euler and gravity sets
vtune_euler_df = vtune_df.loc[vtune_df['kernel'] == "euler"].copy()
vtune_gravity_df = vtune_df.loc[vtune_df['kernel'] == "gravity"].copy()
perf_euler_df = perf_df.loc[perf_df['kernel'] == "euler"].copy()
perf_gravity_df = perf_df.loc[perf_df['kernel'] == "gravity"].copy()

#applying given roofline plotter
ax = plot_roofline()

ax.scatter(vtune_euler_df['AI'], vtune_euler_df['GFLOPS'], marker = '+', color = 'blue', label = 'Euler Solve')
ax.scatter(vtune_gravity_df['AI'], vtune_gravity_df['GFLOPS'], marker = '.', color = 'red', label = 'Gravity Solve')

ax.legend()

plt.savefig("roofline.pdf", bbox_inches = "tight")
plt.savefig("roofline.png", bbox_inches = "tight")