/*
 * Advent of Code 2025 - Day 2: Gift Shop - Part 2
 * Author: Davorah Stetson
 * 
 * Problem: Find invalid product IDs that are made of a pattern repeated 
 * at least twice.
 * 
 * Examples: 
 *   111 = "1" repeated 3 times (invalid)
 *   1010 = "10" repeated 2 times (invalid)
 *   123 = no repeating pattern (valid)
 */

#include <stdio.h>
#include <string.h>

/*
 * Function: hasRepeatingPattern
 * -----------------------------
 * Simple approach: Extract the pattern and rebuild the string.
 * If rebuilt string matches original, it's a repeating pattern!
 * 
 * Example with 1010:
 * - Try pattern "10" (length 2)
 * - Rebuild: "10" + "10" = "1010"
 * - Compare with original: "1010" matches! ✓
 */
int hasRepeatingPattern(long long num) {
    char str[20];
    sprintf(str, "%lld", num);
    int len = strlen(str);
    
    // Try different pattern sizes
    for (int pattern_len = 1; pattern_len <= len / 2; pattern_len++) {
        
        // Check if length divides evenly
        if (len % pattern_len != 0) {
            continue;
        }
        
        // Extract the pattern (first pattern_len characters)
        char pattern[20];
        for (int i = 0; i < pattern_len; i++) {
            pattern[i] = str[i];
        }
        pattern[pattern_len] = '\0';  // End the string
        
        // Build what the full string SHOULD be if pattern repeats
        char rebuilt[20];
        int rebuilt_pos = 0;
        
        // How many times should the pattern repeat?
        int num_repeats = len / pattern_len;
        
        // Repeat the pattern that many times
        for (int repeat = 0; repeat < num_repeats; repeat++) {
            for (int i = 0; i < pattern_len; i++) {
                rebuilt[rebuilt_pos] = pattern[i];
                rebuilt_pos++;
            }
        }
        rebuilt[rebuilt_pos] = '\0';  // End the string
        
        // Does the rebuilt string match the original?
        if (strcmp(rebuilt, str) == 0) {
            return 1;  // Yes! It's a repeating pattern
        }
    }
    
    return 0;  // No repeating pattern found
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
    
    // Read each range from the file
    while (fscanf(fp, "%lld-%lld", &start, &end) == 2) {
        range_count++;
        
        // Check every number in this range
        for (long long id = start; id <= end; id++) {
            if (hasRepeatingPattern(id)) {
                sum += id;
            }
        }
        
        // Skip the comma between ranges
        char comma;
        fscanf(fp, "%c", &comma);
    }
    
    printf("Total ranges processed: %d\n", range_count);
    printf("Sum of invalid IDs: %lld\n", sum);
    
    fclose(fp);
    return 0;
}