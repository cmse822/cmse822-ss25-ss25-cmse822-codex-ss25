import numpy as np
import pandas as pd

import glob


def get_vtune_data(filename):

    #getting resolution, kernel
    resolution = int(filename.split("-")[1])
    kernel_indic = filename.replace("_report.csv","").split("-")[2]

    if("Eu" in kernel_indic):
        kernel = "euler"
    else:
        kernel = "gravity"

    #we only need the DFLOPS and the DRAM GB/Sec
    with open(filename) as f:
        lines = f.readlines()

        DFLOPS_line = lines[3]
        DRAM_line = lines[17]

    DFLOPS_val = float(DFLOPS_line.split("	")[2])
    DRAM_val = float(DRAM_line.split("	")[3])

    output_row = pd.DataFrame([[resolution, kernel, DFLOPS_val, DRAM_val]], columns = ["resolution", "kernel", "GFLOPS", "GBpSEC"])

    return(output_row)


files = glob.glob("*.csv")

vtune_df = pd.DataFrame()

for file in files:
    
    output_row = get_vtune_data(file)
    vtune_df = pd.concat([vtune_df, output_row])

vtune_df['AI'] = vtune_df['GFLOPS'] / vtune_df['GBpSEC']

vtune_df.to_csv('../agoge_vtune_data.csv', index=False)