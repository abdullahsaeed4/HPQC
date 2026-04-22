import time

def main():

    start = time.time()

    with open("../../data/numbers.txt", "r") as f:
        data = f.readlines()

    end = time.time()

    print("Numbers read:", len(data))
    print("Read time:", end - start)

if __name__ == "__main__":
    main()

