# Week 3 – MPI and Vector Summation

## Part 1 – MPI Hello World

I created an MPI Hello World program, compiled it with `mpicc`, and ran it using `mpirun`.

### Timings

| Processes | Real | User | Sys |
|---|---:|---:|---:|
| 1 | 0.376s | 0.077s | 0.071s |
| 2 | 0.416s | 0.104s | 0.095s |
| 4 | 0.429s | 0.126s | 0.159s |
| 8 | 0.436s | 0.200s | 0.261s |
| 16 | 0.487s | 0.408s | 0.483s |

The output appeared in a different order each time because the MPI processes run independently. As the number of processes increased, the user and system times became much larger than the real time, which shows that multiple processes were running in parallel.

## Part 2 – Analysis of `proof.c`

The `proof.c` program uses MPI to divide work between processes.

- `main()` checks the input, starts MPI, gets the rank and communicator size, checks the universe size, assigns the task, and finalises MPI.
- `check_args()` checks that one numerical argument was provided and converts it to an integer.
- `check_uni_size()` checks that there is at least one process available.
- `check_task()` decides whether a process is the root or a client.
- `client_task()` calculates `rank * input_number` and sends the result to the root process.
- `root_task()` receives values from the other processes, adds them together, and prints the final result.

This program could also be replaced by a direct mathematical formula:

`n * (p - 1) * p / 2`

where `n` is the input number and `p` is the number of processes.

## Part 3 – Serial and MPI Vector Summation

I created a serial program called `my_vector_serial.c` and an MPI version called `my_vector_mpi.c`.

Instead of using a trivial vector of zeros, I set each element to `i * i` so that the calculation was more meaningful.

### Serial timings

| Input | Sum | Real |
|---|---:|---:|
| 10 | 285 | 0.006s |
| 100000 | 216474736 | 0.007s |
| 500000 | -1223635408 | 0.012s |

For `500000`, the result overflowed because the program used `int`, which cannot store values that large.

### MPI timings for input 100000

| Processes | Real |
|---|---:|
| 2 | 0.408s |
| 4 | 0.427s |
| 8 | 0.449s |
| 16 | 0.493s |

The MPI version produced the same result as the serial version for `100000`, but it was much slower in real time. The serial program took only `0.007s`, while the MPI version took roughly `0.4–0.5s`.

This shows that for this problem size, the communication overhead of MPI is larger than the benefit of parallelism.

## Conclusion

For this vector summation task, the serial implementation was much faster than the MPI implementation at the tested input size.

MPI successfully distributed the work across processes, but the task itself was too small and simple to benefit from parallel execution. Overflow was also encountered for larger inputs when using `int`, which limits the range of valid tests unless a larger data type such as `long long` is used.
