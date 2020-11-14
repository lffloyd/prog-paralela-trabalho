'''
Usage:
python3 src/graphs/graphs.py arquivo1.csv arquivo2.csv arquivo3.csv
'''

from matplotlib import pyplot as plt
import numpy
import sys
import os

RESULTS_DIR = "results/graphs"


def get_csv_data(csv_filename):
    return numpy.genfromtxt(csv_filename, delimiter=",", names=["x", "y"])


def generate_combined_execution_time_graph(*args):
    datas = [get_csv_data(csv) for csv in args]
    legends = [csv.split("/")[-1].replace(".csv", "") for csv in args]

    fig, ax = plt.subplots(1)

    plots = []

    for data in datas:
        plot, = ax.plot(data["x"], data["y"])
        plots.append(plot)
    
    ax.legend(tuple(plots), tuple(legends), loc="upper left", shadow=True)

    ax.set(xlabel="n", ylabel="tempo de execução (s)")

    filename = f'{RESULTS_DIR}/combined_execution_time.png'
    os.makedirs(os.path.dirname(filename), exist_ok=True)
    fig.savefig(filename)


def generate_execution_time_graph(csv_filename):
    data = get_csv_data(csv_filename)

    fig, ax = plt.subplots(1)

    ax.plot(data["x"], data["y"])
    ax.set(xlabel="n", ylabel="tempo de execução (s)")

    filename = f'{RESULTS_DIR}/{csv_filename.split("/")[-1].replace(".csv", ".png")}'
    os.makedirs(os.path.dirname(filename), exist_ok=True)
    fig.savefig(filename)


def main():
    csvs = [sys.argv[i] for i in range(1, len(sys.argv))]

    print("Generating individual execution time graphs...")
    for csv in csvs:
        generate_execution_time_graph(csv)

    print("Generating combined execution time graph...")
    generate_combined_execution_time_graph(*csvs)
    
    print(f'Graphs generated at {RESULTS_DIR} folder')

main()
