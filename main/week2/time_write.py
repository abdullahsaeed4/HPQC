import sys
import time

def main():
    n = int(sys.argv[1])

    start = time.time()

    f = open("../../data/numbers.txt", "w")

    for i in range(n):
        f.write(f"{i}\n")

    f.close()

    end = time.time()

    print("Write time:", end - start)

if __name__ == "__main__":
    main()
