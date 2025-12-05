/*
 * ============================================================
 * Advent of Code 2025 - Day 5: Cafeteria
 * ============================================================
 *
 * PROBLEM SUMMARY:
 * The Elves have a database with two sections:
 *   1. Fresh ingredient ID ranges (like 3-5 means 3, 4, 5 are fresh)
 *   2. Available ingredient IDs (single numbers to check)
 *
 * GOAL: Count how many available IDs fall within ANY fresh range
 *
 * APPROACH:
 *   1. Read all the ranges into an array
 *   2. Read each available ID
 *   3. For each ID, check if it's inside any range
 *   4. Count the matches
 *
 * ============================================================
 */

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>   /* For qsort */

// Define constants
#define MAX_RANGES 200  /* Maximum number of ranges we'll store
                               Our input has ~150 ranges, so 200 is safe */

#define MAX_LINE_LENGTH 64  /* Buffer size for reading each line
                               Our longest line is about 35 characters */

/* Store a fresh range with start and end values (using long long for large numbers) */
typedef struct {
    long long start;    /* First ID in the range (inclusive) */
    long long end;      /* Last ID in the range (inclusive) */
} Range;

 /*
 * Check if an ingredient ID falls within any fresh range
 * Returns 1 if fresh, 0 if not
 */
int isInAnyRange(long long id, Range ranges[], int rangeCount) {

    /* Loop through every range */
    for (int i = 0; i < rangeCount; i++) {

        /* Check if id falls within this range (inclusive on both ends) */
        if (id >= ranges[i].start && id <= ranges[i].end) {
            return 1;   /* Found a match! Return true immediately */
        }
    }

    /* If we get here, we checked all ranges and found no match */
    return 0;
}

/* Compare function for qsort - sorts ranges by start value */
int compareRanges(const void* a, const void* b) {
    Range* rangeA = (Range*)a;
    Range* rangeB = (Range*)b;

    if (rangeA->start < rangeB->start) return -1;
    if (rangeA->start > rangeB->start) return 1;
    return 0;
}

int main() {

    /* ========== VARIABLE DECLARATIONS ========== */

    /* File pointer - will hold reference to our input file */
    FILE* inputFile;

    /* Buffer to store each line as we read it */
    char line[MAX_LINE_LENGTH];

    /* Array to store all the fresh ranges */
    Range freshRanges[MAX_RANGES];

    /* Counter: how many ranges we've loaded */
    int rangeCount = 0;

    /* Counter: how many fresh ingredients we find (THIS IS OUR ANSWER) */
    int freshCount = 0;

    /* Temporary variables for parsing */
    long long start, end;       /* For parsing range lines */
    long long ingredientID;     /* For parsing available ID lines */

    /* ========== STEP 1: OPEN THE INPUT FILE ========== */

    printf("Opening input file...\n");

    /*
     * fopen() opens a file for reading
     * "r" means "read mode" (we only want to read, not write)
     * Returns NULL if the file doesn't exist or can't be opened
     */
    inputFile = fopen("Input.txt", "r");

    /* ALWAYS check if file opened successfully */
    if (inputFile == NULL) {
        printf("ERROR: Could not open Inputday5.txt\n");
        printf("Make sure the file is in the same folder as your program.\n");
        return 1;   /* Exit with error code 1 */
    }

    printf("File opened successfully!\n\n");

    /* ========== STEP 2: READ ALL FRESH RANGES ========== */

    printf("Reading fresh ranges...\n");

    /* Read lines until we hit a blank line (end of ranges section) */
    while (fgets(line, MAX_LINE_LENGTH, inputFile) != NULL) {

        /* Blank line means end of ranges section */
        if (strlen(line) <= 1) {
            printf("Found blank line - end of ranges section\n");
            break;
        }

        /* Parse "start-end" format and store in array */
        if (sscanf(line, "%lld-%lld", &start, &end) == 2) {
            freshRanges[rangeCount].start = start;
            freshRanges[rangeCount].end = end;
            rangeCount++;
        }
    }

    printf("Loaded %d fresh ranges\n\n", rangeCount);

    /* ========== STEP 3: CHECK EACH AVAILABLE INGREDIENT ========== */

    printf("Checking available ingredients...\n");

    /* Read remaining lines (each is an ingredient ID to check) */
    while (fgets(line, MAX_LINE_LENGTH, inputFile) != NULL) {

        if (strlen(line) <= 1) continue;  /* Skip blank lines */

        /* Parse the ID and check if it's in any fresh range */
        if (sscanf(line, "%lld", &ingredientID) == 1) {
            if (isInAnyRange(ingredientID, freshRanges, rangeCount)) {
                freshCount++;
            }
        }
    }

    /* ========== PART 2: COUNT ALL FRESH IDs IN RANGES ========== */

    printf("\n--- Part 2 ---\n");

    /* Step 1: Sort ranges by start value */
    qsort(freshRanges, rangeCount, sizeof(Range), compareRanges);

    /* Step 2: Merge overlapping ranges and count total IDs */
    long long totalFreshIDs = 0;
    long long currentStart = freshRanges[0].start;
    long long currentEnd = freshRanges[0].end;

    for (int i = 1; i < rangeCount; i++) {

        /* Check if this range overlaps or touches the current merged range */
        if (freshRanges[i].start <= currentEnd + 1) {
            /* Extend the current range if this one goes further */
            if (freshRanges[i].end > currentEnd) {
                currentEnd = freshRanges[i].end;
            }
        }
        else {
            /* No overlap - count the current range and start a new one */
            totalFreshIDs += (currentEnd - currentStart + 1);
            currentStart = freshRanges[i].start;
            currentEnd = freshRanges[i].end;
        }
    }

    /* ========== STEP 4: CLEANUP AND OUTPUT ========== */

    fclose(inputFile);  /* Always close files when done */

    /* Print the answer */
    printf("\n");
    printf("============================================\n");
    printf("ANSWER Part 1: %d ingredients are fresh\n", freshCount);
    printf("============================================\n");


    /* Don't forget to count the last merged range */
    totalFreshIDs += (currentEnd - currentStart + 1);

    printf("============================================\n");
    printf("ANSWER Part 2: %lld total fresh IDs\n", totalFreshIDs);
    printf("============================================\n");

    return 0;
}