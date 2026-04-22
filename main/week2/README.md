# Week 2 – File I/O Performance

## Overview

In this exercise I compared the performance of C and Python for:

- Basic programs (hello world, repeat adder)
- Writing to a file
- Reading from a file

Execution time was measured using the `time` command in the terminal.

---

## Results

### Basic Program Performance

C is consistently faster than Python.

- Hello World:
  - Python: ~0.034s
  - C: ~0.007s

- Repeat Adder:
  - Python: ~0.03s (constant)
  - C: ~0.004–0.006s

Python runtime stays almost constant even for large inputs, while C remains significantly faster overall.

---

### File Writing

| Input Size | Python (real) | C (real) |
|------------|--------------|---------|
| 10,000     | 0.041s       | 0.013s  |
| 100,000    | 0.066s       | 0.026s  |

C is about 2–3× faster for writing.

---

### File Reading

| Language | Time (real) |
|----------|------------|
| Python   | 0.046s     |
| C        | 0.019s     |

C is again noticeably faster, though the gap is smaller than for writing.

---

## Discussion

C performs better in all cases due to being a compiled language with less overhead.

Python has a higher startup and execution cost, which is why its runtime stays nearly constant even when input size increases.

For file I/O, both languages scale with input size, but C remains more efficient.

---

## Conclusion

C is significantly faster for both computation and file operations.

Python is easier to use but comes with performance overhead, making it less suitable for performance-critical tasks.

---

## Notes

- Data file stored outside the repository as instructed
- File sizes kept small (≤ 100,000 lines)
