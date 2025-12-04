/*
 * Advent of Code 2025 - Day 2: Gift Shop
 * Author: Davorah Stetson
 * 
 * Problem: Find all invalid product IDs in given ranges.
 * An invalid ID is one that is made ONLY of some sequence of digits repeated twice.
 * For example: 55 (5+5), 6464 (64+64), 123123 (123+123)
 * 
 * Solution: Check if each ID can be split into two equal halves that are identical.
 */

#include <stdio.h>
#include <string.h>

/*
 * Function: isRepeatedPattern
 * ---------------------------
 * Checks if a number is made entirely of a pattern repeated twice
 * 
 * num: the number to check
 * 
 * returns: 1 if the number is a repeated pattern, 0 otherwise
 * 
 * Examples:
 *   55 -> "5" + "5" -> returns 1 (invalid)
 *   6464 -> "64" + "64" -> returns 1 (invalid)
 *   101 -> can't split evenly -> returns 0 (valid)
 *   1010 -> "10" + "10" -> returns 1 (invalid)
 */
int isRepeatedPattern(long long num) {
    char str[20];
    sprintf(str, "%lld", num);
    int len = strlen(str);
    
    // The length must be even to be split into two equal halves
    if (len % 2 != 0) {
        return 0;
    }
    
    int half = len / 2;
    
    // Check if first half equals second half
    for (int i = 0; i < half; i++) {
        if (str[i] != str[half + i]) {
            return 0;  // Halves don't match
        }
    }
    
    return 1;  // The entire ID is a repeated pattern
}

int main() {
    // Open the input file
    FILE *fp = fopen("input.txt", "r");
    if (fp == NULL) {
        printf("Error opening file\n");
        return 1;
    }
    
    long long start, end;
    long long sum = 0;
    int range_count = 0;
    
    // Read ranges in format: start-end,start-end,...
    // fscanf automatically parses the numbers and skips the dash
    while (fscanf(fp, "%lld-%lld", &start, &end) == 2) {
        range_count++;
        
        // Check each ID in the range
        for (long long id = start; id <= end; id++) {
            if (isRepeatedPattern(id)) {
                sum += id;
            }
        }
        
        // Read the comma separator (or newline at end)
        char comma;
        fscanf(fp, "%c", &comma);
    }
    
    // Output results
    printf("Total ranges processed: %d\n", range_count);
    printf("Sum of invalid IDs: %lld\n", sum);
    
    // Clean up
    fclose(fp);
    return 0;
}