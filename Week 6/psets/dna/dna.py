import csv
import sys


def main():
    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        print("Incorrect number of arguments")
        return 1

    # TODO: Read database file into a variable
    people = []

    with open(sys.argv[1]) as file:
        file_reader = csv.DictReader(file)
        for person in file_reader:
            people.append(person)

    # TODO: Read DNA sequence file into a variable
    sequence_file = open(sys.argv[2])
    sequence = sequence_file.read()
    sequence_len = len(sequence)

    # TODO: Find longest match of each STR in DNA sequence
    # extract STRs from file
    list_str = list(people[1])[1:]

    # create dict to store the consecutive number of STRs
    str_count = {}

    # select specific STR
    for current_str in list_str:
        # set counter of the current STR in counting dictionary to zero
        str_count[current_str] = 0

        # compute length of current STR
        str_len = len(current_str)

        # iterate over sequence by one char till match between STRs
        i = 0
        for _ in range(sequence_len - str_len):
            # set temp counter of max cons of current STR to zero
            cons_counter = 0

            # when match iterate over sequence by len of current STR, to determine if there is a match or not
            while sequence[i : i + str_len] == current_str:
                # increase the cons counter by one with each match
                cons_counter += 1

                # increase i of STR len to check if next sequence does match
                i += str_len

            # if temp counter is greater than current vaue in dict overwrite it
            if cons_counter > str_count[current_str]:
                str_count[current_str] = cons_counter

            # increase i by one to proceed iterating the sequence by one char
            i += 1

    # TODO: Check database for matching profiles
    # iterate over each person in database, to check if there is a match
    for person in people:
        # create temporary dictionary that does not contain name
        temp_dict = {}
        for current_str in list_str:
            temp_dict[current_str] = int(person[current_str])

        # check if match
        if temp_dict == str_count:
            print(person["name"])
            return 0

    print("No match")

    return 1


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):
        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:
            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
