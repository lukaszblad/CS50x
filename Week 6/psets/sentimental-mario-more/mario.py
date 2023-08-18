# TODO

from cs50 import get_int

# prompting user for height
while True:
    height = get_int("Height: ")

    # reprompting if input invalid
    if height > 0 and height <= 8:
        break


# printing piramide
for i in range(1, height + 1):
    hashes = "#" * i
    spaces = " " * (height - i)

    print(spaces + hashes + "  " + hashes)
