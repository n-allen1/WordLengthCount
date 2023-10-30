#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Define a maximum line length for reading from the file
#define MAX_LINE_LENGTH 80


// This struct will store the word and its frequency
typedef struct 
{
    char *word;
    int count;
} WordFrequency;

// This struct will store word data for a particular length
// This includes the length, frequency, and the word frequencies
typedef struct 
{
    int length;
    int frequency;
    WordFrequency *wordFrequencies;
    int numWords;
} WordData;

// Function prototypes
void toLowercase(char* s);
int compare(const void* a, const void* b);
WordData* getWordDataForLength(WordData **data, int *maxLen, int length);
void incrementWordFrequency(WordData *data, const char *token);

int main(int argc, char** argv) 
{
    // Check for correct number of command line arguments
    if (argc != 2) 
    {
        fprintf(stderr, "Usage: %s input.txt\n", argv[0]);
        return 1;
    }

    // Open the input file
    FILE* inFile = fopen(argv[1], "r");
    if (!inFile) 
    {
        fprintf(stderr, "Error opening file: %s\n", argv[1]);
        return 1;
    }

    char line[MAX_LINE_LENGTH + 1];
    WordData *wordDataArray = NULL;
    int maxWordLen = 0;
    int totalWords = 0;

    // Read the file line by line
    while (fgets(line, sizeof(line), inFile)) 
    {
        // Skip comments and empty lines
        if (line[0] == '#' || strlen(line) == 0)
        {
            continue;
        }
        // Tokenize the line based on delimiters
        char *token = strtok(line, " \t\n.,;()");
        while (token) 
        {
            // Get the length of the word
            int len = strlen(token);

            // Increment total word count
            totalWords++;

            // Get the WordData for this word length
            WordData *data = getWordDataForLength(&wordDataArray, &maxWordLen, len);

            // Increment the frequency for this length
            data->frequency++;

            // Update the frequency of the word
            incrementWordFrequency(data, token);

            // Get the next token
            token = strtok(NULL, " \t\n.,;()");
        }
    }

    // Print the results
    WordData maxLenData = wordDataArray[maxWordLen];
    printf("%s contains %d words\n", argv[1], totalWords);
    printf("The longest words have %d letters and occur %d times\n", maxWordLen, maxLenData.frequency);
    for (int i = 0; i < maxLenData.numWords; i++) 
    {
        printf("\t%s\n", maxLenData.wordFrequencies[i].word);

        // Free the memory used by the word
        free(maxLenData.wordFrequencies[i].word);
    }

    // Cleanup the dynamically allocated memory
    for (int i = 0; i <= maxWordLen; i++) 
    {
        free(wordDataArray[i].wordFrequencies);
    }
    free(wordDataArray);

    // Close the input file
    fclose(inFile);

    return 0;
}

// Utility function to convert a string to lowercase
void toLowercase(char* s) 
{
    for (int i = 0; s[i]; i++) 
    {
        s[i] = tolower((unsigned char)s[i]);
    }
}

// Comparison function for qsort
// Used for sorting word data by frequency
int compare(const void* a, const void* b) 
{
    // We want to sort in descending order based on frequency
    return ((WordData*)b)->frequency - ((WordData*)a)->frequency;
}

// This function will get the WordData structure for a specific word length
// If the length is more than the current maximum length, we expand our array
WordData* getWordDataForLength(WordData **data, int *maxLen, int length) 
{
    if (length > *maxLen) 
    {
        // Reallocate the array to accommodate the new length
        *data = (WordData*) realloc(*data, sizeof(WordData) * (length + 1));

        // Init the new entries
        for (int i = *maxLen + 1; i <= length; i++) 
        {
            (*data)[i].length = i;
            (*data)[i].frequency = 0;
            (*data)[i].numWords = 0;
            (*data)[i].wordFrequencies = NULL;
        }

        // Update the maximum length
        *maxLen = length;
    }

    // Return the WordData for the requested length
    return &(*data)[length];
}

// This function increments the frequency of a word
void incrementWordFrequency(WordData *data, const char *token) 
{
    char* lowerToken = strdup(token); // Duplicate the token to avoid modifying the original string
    toLowercase(lowerToken);  // Convert to lowercase
    
    // Check if the word is already in our list
    for (int i = 0; i < data->numWords; i++) 
    {
        if (strcmp(data->wordFrequencies[i].word, lowerToken) == 0) 
        {
            // If found, increment its count
            data->wordFrequencies[i].count++;
            free(lowerToken);  // Free the duplicated token
            return;
        }
    }

    // If the word is not found, add it to our list
    data->numWords++;

    // Reallocate memory for the new word
    data->wordFrequencies = (WordFrequency*) realloc(data->wordFrequencies, data->numWords * sizeof(WordFrequency));

    // Store the new word and set its count to 1
    data->wordFrequencies[data->numWords - 1].word = lowerToken;
    data->wordFrequencies[data->numWords - 1].count = 1;
}
