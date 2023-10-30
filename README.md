# WordLengthCount

## This is a program that takes an input file as a command-line argument.

It will count the length of words, and **outputs** the words with the longest length, and a total count for words of that max length (excluding duplicates).
Additionally it will output the length of the most common length, and its occurrence count.
It will also output the amount of words in a file.

> It assumes there are words no longer than 15 characters, words of the longest size occur at most 15 times, and no line in the file is longer than 80 characters.

# Example output:

```
:~/$ ./main input.txt
input.txt contains 271 words
words of size 2 occur the most: 53 times
the longest words have 10 letters and occur 13 times
	navigation
	difference
	underlying
	determined
	properties
	fiberglass
	electrical
	magnitudes
	particular
	deviations
```
