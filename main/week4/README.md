# Week 4 – MPI Communications

## Overview

This week focused on MPI communication patterns, latency and bandwidth measurement, and collective communication routines. The work was divided into three parts:

- basic point-to-point communication
- ping-pong benchmarking for latency and bandwidth
- collective communication comparisons

---

## Part 1: Demonstrating Communications

### Original communication test

The original `comm_test_mpi` program was run with different numbers of processes. In all cases, rank 0 acted as the root and received one message from every other process. Each non-root process sent the value `rank × 10`.

### Observations

The order of printed output varied between runs, especially for the sending processes. This is expected in MPI because processes run independently and message ordering is not guaranteed at the print level. However, the final received values were always correct.

### Timing results for `comm_test_mpi`

| Processes | Real Time (s) | User Time (s) | Sys Time (s) |
|---|---:|---:|---:|
| 4  | 0.443 | 0.137 | 0.161 |
| 8  | 0.453 | 0.239 | 0.268 |
| 16 | 0.499 | 0.393 | 0.505 |

### Functionalised version

The code was rewritten into smaller functions in `comm_test_mpi_functions`. This made the program easier to read, test, and maintain. The functionality remained the same.

### Timing results for `comm_test_mpi_functions`

| Processes | Real Time (s) | User Time (s) | Sys Time (s) |
|---|---:|---:|---:|
| 4  | 0.425 | 0.108 | 0.173 |
| 8  | 0.436 | 0.222 | 0.260 |
| 16 | 0.501 | 0.403 | 0.472 |

### Send type experiments

Four variants of `MPI_Send()` were tested:

- `MPI_Ssend()`
- `MPI_Bsend()`
- `MPI_Rsend()`
- `MPI_Isend()`

All produced the expected output in this case, but their behavior differs:

- `MPI_Send()` is the standard and most reliable option
- `MPI_Ssend()` is synchronous and waits for the matching receive
- `MPI_Bsend()` uses an attached buffer
- `MPI_Rsend()` assumes the receive is already posted, so it is riskier
- `MPI_Isend()` is non-blocking and requires completion handling

### Timing results for send variants at 4 processes

| Version | Real Time (s) | User Time (s) | Sys Time (s) |
|---|---:|---:|---:|
| `MPI_Ssend` | 0.416 | 0.129 | 0.155 |
| `MPI_Bsend` | 0.418 | 0.119 | 0.173 |
| `MPI_Rsend` | 0.435 | 0.133 | 0.148 |
| `MPI_Isend` | 0.450 | 0.114 | 0.174 |

### Internal timing

The timed version showed that send times were extremely short, on the order of microseconds, and root receive time increased with the number of sending processes.

Examples:

- 4 processes: root receive time about `0.000141 s`
- 8 processes: root receive time about `0.000224 s`
- 16 processes: root receive time about `0.000351 s`

This shows that timing variability is large relative to the operation size, which makes this benchmark noisy.

### Conclusion for Part 1

The different send methods all worked for this simple case, but `MPI_Send()` and `MPI_Ssend()` were the most reliable and easiest to reason about. Performance differences were very small because the messages were tiny and the total workload was minimal.

---

## Part 2: Benchmarking Latency and Bandwidth

### Ping-pong latency test

The `pingpong.c` program sent a counter back and forth between exactly two processes. The root process sent the value, the client incremented it and sent it back, and this repeated until the requested number of pings was reached.

### Latency results

| Number of Pings | Elapsed Time (s) | Average Round Trip Time (s) |
|---|---:|---:|
| 10 | 0.000057 | 0.000006 |
| 100 | 0.000159 | 0.000002 |
| 1000 | 0.001059 | 0.000001 |
| 10000 | 0.010613 | 0.000001 |
| 100000 | 0.062567 | 0.000001 |
| 1000000 | 0.590289 | 0.000001 |

As the number of pings increased, the average round-trip time converged to approximately `1 × 10^-6 s`, so the latency is about 1 microsecond per round trip.

### External timing results

| Number of Pings | Real Time (s) |
|---|---:|
| 10 | 0.421 |
| 100 | 0.412 |
| 1000 | 0.360 |
| 10000 | 0.426 |
| 100000 | 0.480 |
| 1000000 | 1.013 |

The external timings include MPI startup overhead, so the internal timings are more meaningful for latency measurement.

### Latency plot

![Latency plot](Latency%20Plot.png)

The linear fit shown on the graph is:

`y = 6E-07x + 0.0018`

This shows that elapsed time increases approximately linearly with the number of pings.

---

### Bandwidth test

The `pingpong_bandwidth.c` program sent an array back and forth between two processes. Message size was varied from 8 bytes up to 2 MiB.

### Bandwidth results

| Elements | Bytes | Elapsed Time (s) | Average Round Trip Time (s) |
|---:|---:|---:|---:|
| 2 | 8 | 0.000853 | 0.000001 |
| 4 | 16 | 0.001213 | 0.000001 |
| 8 | 32 | 0.001141 | 0.000001 |
| 16 | 64 | 0.001289 | 0.000001 |
| 32 | 128 | 0.001696 | 0.000002 |
| 64 | 256 | 0.001587 | 0.000002 |
| 128 | 512 | 0.001952 | 0.000002 |
| 256 | 1024 | 0.002410 | 0.000002 |
| 512 | 2048 | 0.002880 | 0.000003 |
| 1024 | 4096 | 0.010850 | 0.000011 |
| 2048 | 8192 | 0.015441 | 0.000015 |
| 4096 | 16384 | 0.015662 | 0.000016 |
| 8192 | 32768 | 0.019261 | 0.000019 |
| 16384 | 65536 | 0.024983 | 0.000025 |
| 32768 | 131072 | 0.044602 | 0.000045 |
| 65536 | 262144 | 0.077540 | 0.000078 |
| 131072 | 524288 | 0.140498 | 0.000140 |
| 262144 | 1048576 | 0.264581 | 0.000265 |
| 524288 | 2097152 | 0.505515 | 0.000506 |

### Bandwidth plot

![Bandwidth plot](Bandwidth%20Plot.png)

The linear fit shown on the graph is:

`y = 2E-10x + 6E-06`

For a linear model of the form `y = mx + c`:

- `c` corresponds to latency
- `m` corresponds to time per byte

So from the fit:

- latency ≈ `6 × 10^-6 s`
- slope ≈ `2 × 10^-10 s/byte`

Approximate bandwidth is:

`bandwidth ≈ 1 / m = 1 / (2 × 10^-10) = 5 × 10^9 bytes/s`

So the bandwidth is approximately **5 GB/s**.

### Conclusion for Part 2

The ping-pong experiments showed that communication time scales linearly with both number of round trips and message size. The intercept of the bandwidth graph gives the latency, while the slope gives the inverse of the bandwidth.

---

## Part 3: Collective Communications

### Correctness tests

Small input values with known sums were used to verify correctness.

For `N = 10`:

`0^2 + 1^2 + 2^2 + ... + 9^2 = 285`

For `N = 8`:

`0^2 + 1^2 + 2^2 + ... + 7^2 = 140`

All collective communication versions produced the correct results.

---

### Step 1: Broadcast vs Scatter vs DIY

Three versions were compared:

- `vector_broadcast_reduce`
- `vector_scatter_reduce`
- `vector_diy_reduce`

Each used a different way of distributing data from the root process.

### Results for 4 processes

| Method | N = 100000 | N = 1000000 |
|---|---:|---:|
| Broadcast + Reduce | 0.435 | 0.428 |
| Scatter + Reduce | 0.423 | 0.447 |
| DIY Send/Recv + Reduce | 0.432 | 0.435 |

### Results for 8 processes, N = 100000

| Method | Real Time (s) |
|---|---:|
| Broadcast + Reduce | 0.449 |
| Scatter + Reduce | 0.430 |
| DIY Send/Recv + Reduce | 0.440 |

### Conclusion

The differences were small, but `MPI_Scatter()` was generally the most efficient way to distribute the data. `MPI_Bcast()` gives every process the full array, which is less memory-efficient, while the DIY send/recv version is more verbose and less elegant.

---

### Step 2: Send/Recv vs Gather vs Reduce

Three methods were compared for collecting partial sums:

- `vector_scatter_sendrecv`
- `vector_scatter_gather`
- `vector_scatter_reduce`

### Results for 4 processes

| Method | N = 100000 | N = 1000000 |
|---|---:|---:|
| Send/Recv | 0.415 | 0.447 |
| Gather | 0.421 | 0.424 |
| Reduce | 0.419 | 0.432 |

### Results for 8 processes, N = 100000

| Method | Real Time (s) |
|---|---:|
| Send/Recv | 0.433 |
| Gather | 0.447 |
| Reduce | 0.441 |

### Conclusion

All three methods gave the same correct answer. `MPI_Reduce()` is the cleanest approach because it both collects and combines the results in one operation. `MPI_Gather()` is also neat but still requires an extra loop on the root process. Manual send/recv works, but it is the least compact and easiest to scale badly.

---

### Step 3: Custom Reduce Operation

A custom reduce operation was implemented using `MPI_Op_create()` and compared with the built-in `MPI_SUM`.

### Results for 4 processes

| Method | N = 100000 | N = 1000000 |
|---|---:|---:|
| Built-in Reduce | 0.451 / 0.441 | 0.441 |
| Custom Reduce | 0.419 / 0.418 | 0.435 |

Both versions produced the same correct sums:

- `N = 100000` → `216474736`
- `N = 1000000` → `584144992`

### Conclusion

The custom reduce operation was correct and performed similarly to the built-in version. The built-in `MPI_SUM` is still preferable in practice because it is already standard and optimised.

---

