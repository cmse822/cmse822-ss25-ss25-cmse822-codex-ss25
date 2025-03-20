import numpy as np
import pandas as pd

import matplotlib.pyplot as plt

#reading relevant data;
agoge_output_df = pd.read_csv("agoge_output_data.csv")

#paring down;
agoge_output_df = agoge_output_df.loc[agoge_output_df['timer_name'].isin(["EulerSolve","solvePoisson"])]

#splitting;
euler_df = agoge_output_df.loc[agoge_output_df['kernel'] == "euler"].copy()
gravity_df = agoge_output_df.loc[agoge_output_df['kernel'] == "gravity"].copy()

euler_df.sort_values(by="resolution", inplace = True)
gravity_df.sort_values(by="resolution", inplace = True)

plt.figure(figsize = [14,8])

plt.scatter(euler_df['resolution'], euler_df['mega_zone_updates_ps'], color = 'blue', label = 'Euler Solve')
plt.scatter(gravity_df['resolution'], gravity_df['mega_zone_updates_ps'], color = 'red', label = 'Gravity Solve')

plt.legend()

plt.ylabel('MZU/sec')
plt.xlabel('Resolution')

plt.yscale('log')
plt.xscale('log')

plt.grid(True, which="both", linestyle="--", linewidth=0.5)

plt.savefig("complexity.pdf", bbox_inches = "tight")

