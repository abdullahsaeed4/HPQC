import numpy as np
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import pandas as pd
import sys

def get_args():
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} [INPUT_CSV] [OUTPUT_GIF]")
        sys.exit(-1)
    return sys.argv[1], sys.argv[2]

def get_data(filename, other=2):
    try:
        data = pd.read_csv(filename, skipinitialspace=True)
    except FileNotFoundError:
        print(f"Error: file not found: {filename}")
        sys.exit(-1)

    num_positions = len(data.columns) - other
    num_times = len(data)
    return data, num_positions, num_times

def extract_position(data, i=0, other=2):
    row = data.iloc[i]
    y_positions = np.array(row[other:]).flatten()
    x_positions = np.arange(len(row) - other)
    return x_positions, y_positions

def configure_plot(x_positions, y_positions):
    fig, ax = plt.subplots(1, 1)
    fig.suptitle("Simulation of Vibrations in a String")
    ax.set_ylim(-1.1, 1.1)
    rope, = ax.plot(x_positions, y_positions, "o", markersize=5, color="green", label="points on string")
    ax.legend(handles=[rope], bbox_to_anchor=(0, 0), loc="upper left")
    return fig, rope

def animate(frame, data, rope):
    x_positions, y_positions = extract_position(data, i=frame)
    rope.set_data(x_positions, y_positions)
    return rope,

def main():
    input_csv, output_gif = get_args()
    data, num_positions, num_times = get_data(input_csv)
    x_positions, y_positions = extract_position(data)
    fig, rope = configure_plot(x_positions, y_positions)

    ani = animation.FuncAnimation(
        fig,
        animate,
        num_times,
        interval=40,
        blit=True,
        fargs=(data, rope)
    )

    ani.save(filename=output_gif, writer="pillow", fps=25)

if __name__ == "__main__":
    main()
