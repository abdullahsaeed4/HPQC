import sys

def main():
    output = 0

    try:
        # convert arguments
        in_arg = int(sys.argv[1])
        multiple = int(sys.argv[2])

    except:
        raise Exception(
            "Incorrect arguments.\n"
            "Usage: python repeat_adder.py [NUM] [NUM]"
        )

    # iterates over all numbers up to the input
    for i in range(in_arg):
        output = output + multiple

    # prints result
    print("{}\n".format(output))


if __name__ == "__main__":
    main()
