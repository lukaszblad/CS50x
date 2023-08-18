# TODO

from cs50 import get_float

# prompting user for the change owed
while True:
    change = get_float("Change owed: ")
    if change > 0:
        change = change * 100
        break

# computing amount of coins
coins = 0

while change >= 25:
    coins = coins + 1
    change = change - 25

while change >= 10:
    coins = coins + 1
    change = change - 10

while change >= 5:
    coins = coins + 1
    change = change - 5

while change >= 1:
    coins = coins + 1
    change = change - 1

print(coins)
