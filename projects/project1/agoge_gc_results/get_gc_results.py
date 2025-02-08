import numpy as np
import pandas as pd

import glob

#this whole circus assumes you are running in the project1 folder of our repo.

#TODO: Document this
#first, we will want a function to read in each output file and extract the relevant data.
def extract_perf_data(filename):

    #first, read the file in;
    with open(filename) as f:
        lines = f.readlines()
        
        datalines = []

        #add the first 25 not empty lines to the new list
        for i in range(1, len(lines)):
            if(len(datalines) > 20):
                break

            if(lines[-i] != "\n"):
                datalines.append(lines[-i])

    datalines = datalines[::-1]

    #cleaning lines
    for i in range(len(datalines)):
        datalines[i] = datalines[i].strip()
        datalines[i] = datalines[i].replace('\n', '')

    #prepping output rows;
    agoge_rows = pd.DataFrame(columns = ["timer_name","total_time", "calls", "avg_time_ms", "mega_zone_updates_ps"])


    #iterating through agoge output data;
    lines_passed = 0
    for i in range(len(datalines)):

        if "-" in datalines[i]:
            lines_passed += 1
            continue

        #skipping possible junk rows;
        if(lines_passed == 0):
            continue
        elif(lines_passed >= 2):
            break

        #we now know this is a data row;
        value_list = datalines[i].split()

        agoge_rows = pd.concat([agoge_rows, pd.DataFrame([value_list], columns = agoge_rows.columns)], ignore_index = True)
    
    #getting perf data;
    perflines = datalines[-10:]
    perf_values = []

    for i in range(len(perflines)):
        perf_values += [float(perflines[i].split()[0].replace(",",""))]

    perf_row = pd.DataFrame([perf_values], columns = ["L1-dcache-loads",
                                                      "fp-128b_packed_double", 
                                                      "fp-128b_packed_single",
                                                      "fp-256b_packed_double",
                                                      "fp-256b_packed_single",
                                                      "fp-scalar_double",
                                                      "fp-scalar_single",
                                                      "time_elapsed",
                                                      "time_user",
                                                      "time_sys"])

    #pulling resolution and kernel info;
    kernel = filename.split("_")[1].split("-")[1].replace(".out","")
    resolution = int(filename.split("_")[1].split("-")[0])

    agoge_rows["kernel"] = kernel
    agoge_rows["resolution"] = resolution

    perf_row["kernel"] = kernel
    perf_row["resolution"] = resolution

    return(agoge_rows, perf_row)

# ==========================
# MAIN
# ==========================

#getting files;
files = glob.glob("*.out")

output_df = pd.DataFrame()
perf_df = pd.DataFrame()

for i in range(len(files)):
    #print(i, files[i])
    agoge_rows, perf_row = extract_perf_data(files[i])
    
    output_df = pd.concat([output_df, agoge_rows], ignore_index = True)
    perf_df = pd.concat([perf_df, perf_row], ignore_index = True)

perf_df['arithmetic_intensity'] = perf_df['fp-scalar_double'] / (128 * perf_df['L1-dcache-loads'])
perf_df['flop_rate'] = perf_df['fp-scalar_double'] / perf_df['time_elapsed']

#saving data as csv.
output_df.to_csv("agoge_output_data.csv", index = False)
perf_df.to_csv("agoge_perf_data.csv", index = False)

