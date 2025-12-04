/*
 * Advent of Code 2025 - Day 3: Battery Joltage
 * 
 * PROBLEM:
 * Find the maximum joltage from each bank by selecting exactly two batteries.
 * Joltage = concatenation of two battery digits (e.g., 7 and 8 → 78 jolts)
 * 
 * INPUT: Each line is a bank of batteries (each digit is a battery rating)
 * OUTPUT: Sum of maximum joltages across all banks
 * 
 * APPROACH:
 * 1. Read each line (bank) from input file
 * 2. Try all pairs of batteries in the bank
 * 3. Calculate joltage by concatenating the two digits
 * 4. Track the maximum joltage for that bank
 * 5. Sum all maximums
 */

#include <stdio.h>
#include <string.h>

// Calculate joltage from two battery digits
// Example: (7, 8) → 7*10 + 8 = 78
int calculate_joltage(int digit1, int digit2) {
    return digit1 * 10 + digit2;
}

// Find maximum joltage possible from a bank of batteries
int find_max_joltage(const char *bank, int length) {
    int max_joltage = 0;
    
    // Try all pairs: first battery (i) with second battery (j)
    for (int i = 0; i < length - 1; i++) {
        for (int j = i + 1; j < length; j++) {
            // Convert char to int: '5' - '0' = 5
            int digit1 = bank[i] - '0';
            int digit2 = bank[j] - '0';
            
            int joltage = calculate_joltage(digit1, digit2);
            
            if (joltage > max_joltage) {
                max_joltage = joltage;
            }
        }
    }
    
    return max_joltage;
}

int main(void) {
    FILE *fp = fopen("input.txt", "r");
    if (!fp) {
        printf("Error: Cannot open input.txt\n");
        return 1;
    }
    
    long long total = 0;
    char line[1024];
    int bank_num = 1;
    
    // Read file line by line
    while (fgets(line, sizeof(line), fp)) {
        // Remove newline character if present
        int len = strlen(line);
        if (len > 0 && line[len-1] == '\n') {
            line[len-1] = '\0';
            len--;
        }
        
        // Skip empty lines
        if (len == 0) continue;
        
        // Find max joltage for this bank
        int max = find_max_joltage(line, len);
        total += max;
        
        printf("Bank %d: %d jolts\n", bank_num++, max);
    }
    
    fclose(fp);
    
    printf("\nTotal joltage: %lld\n", total);
    
    return 0;
}