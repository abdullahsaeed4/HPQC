# Week 5 – Oscillations on a String

## Overview

This week was about simulating a vibrating string in C, then improving it and making a parallel version using MPI.

The main goals were:

* run and understand the provided serial code
* remove hard-coded values
* parallelise the simulation using MPI
* compare performance (serial vs MPI)
* improve the physical model

---

## Part 1 – Serial Code

I started with the provided `string_wave.c` and modified it so it no longer used hard-coded values.

The program now takes:

* number of points on the string
* number of cycles
* number of samples
* output file location

### Compile

```bash
gcc string_wave.c -o ~/bin/my_string_wave -lm
```

### Run

```bash
~/bin/my_string_wave 50 5 25 ~/data/string_wave.csv
```

### Visualisation

```bash
python3 animate_line_file.py ~/data/string_wave.csv
```

---

## Output (Serial)

The simulation produces an animation of the string over time.

### Serial animation

![Serial Simulation](my_string_wave.gif)

Even though the frame looks flat in a screenshot, the animation shows the motion over time.

---

## Part 2 – MPI Version

The code was then parallelised using MPI.

### Approach

* The string is split into chunks across processes
* Each process updates its own section
* Neighbouring processes exchange boundary values
* Rank 0 gathers everything and writes to file

---

### Compile

```bash
mpicc my_string_wave_mpi.c -o ~/bin/my_string_wave_mpi -lm
```

### Run

```bash
mpirun -np 4 ~/bin/my_string_wave_mpi 1000 5 25 ~/data/string_mpi_1000.csv
```

---

## Output (MPI)

### MPI animation

![MPI Simulation](my_string_wave_mpi.gif)

Again, the movement is visible in the animation rather than a single frame.

---

## Performance Results

### Serial

| Points | Time (s) |
| -----: | -------: |
|   1000 |    0.050 |
|  10000 |    0.356 |
| 100000 |    3.393 |

---

### MPI (2 processes)

| Points | Time (s) |
| -----: | -------: |
|   1000 |    0.467 |
|  10000 |    0.795 |
| 100000 |    4.112 |

---

### MPI (4 processes)

| Points | Time (s) |
| -----: | -------: |
|   1000 |    0.484 |
|  10000 |    0.855 |
| 100000 |    4.170 |

---

## Discussion

The MPI version is slower than the serial version for all test cases.

This is mainly because:

* communication between processes adds overhead
* boundary values must be exchanged every step
* all data is gathered to one process for writing
* the actual computation per point is very small

So the cost of communication is larger than the benefit of parallelism here.

---

## Conclusion

For the problem sizes tested, the serial version is clearly better.

Parallelisation doesn’t help because:

* the task is too small
* communication dominates runtime

A parallel approach might only become useful for much larger simulations.

---

## Part 3 – Improved Model

The original model was quite simple and not very realistic.

I improved it by introducing a **spring-based model**:

* each point interacts with its neighbours
* forces follow Hooke’s Law
* motion evolves more realistically

---

### Improved model animation

![Spring Simulation](my_string_wave_spring.gif)

This produces much more realistic oscillations compared to the original version.

---

## Files

### Code

* `string_wave.c` (modified serial version)
* `my_string_wave_mpi.c` (MPI version)
* improved spring model code

### Data

All generated CSV files are stored in:

```
main/week5/data/
```

### Animations

* `my_string_wave.gif`
* `my_string_wave_mpi.gif`
* `my_string_wave_spring.gif`


