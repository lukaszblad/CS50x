# TODO

from cs50 import get_string

# prompt the user for the text
text = get_string("Text: ")

# count number of letters, words and sentences
letters = 0.0
words = 1.0
sentences = 0.0

for char in text:
    if char.isalpha():
        letters = letters + 1.0
    elif char.isspace():
        words = words + 1.0
    elif char in [".", "!", "?"]:
        sentences = sentences + 1.0

# print(f"letters: {letters}, words: {words}, sentences: {sentences}")

# calculata grade
grade = 0.0588 * ((letters * 100) / words) - 0.296 * ((sentences * 100) / words) - 15.8

# print(f"grade: {grade}")

# print out grade
if round(grade) < 1:
    print("Before Grade 1")
elif round(grade) > 16:
    print("Grade 16+")
else:
    print(f"Grade {round(grade)}")
