/*
 * Advent of Code 2025 - Day 3 Part 2: Battery Joltage (12 batteries)
 * 
 * PROBLEM:
 * Select exactly 12 batteries from each bank to make the largest joltage.
 * Batteries must stay in original order (we skip some, don't rearrange).
 * 
 * APPROACH:
 * Greedy selection: At each position, skip if a better digit comes later.
 * Example: 987654321111111 → skip three 1s → keep 987654321111
 */

#include <stdio.h>
#include <string.h>

// Find largest 12-digit joltage by strategically skipping batteries
long long find_max_joltage_12(const char *bank, int length) {
    if (length < 12) return 0;
    
    int to_skip = length - 12;  // Number of batteries to skip
    int skipped = 0;
    char result[13];
    int result_idx = 0;
    
    for (int i = 0; i < length && result_idx < 12; i++) {
        int remaining = length - i;        // Batteries left (including current)
        int need = 12 - result_idx;        // How many more we need
        
        // Can we afford to skip this one?
        if (skipped < to_skip && remaining - 1 >= need) {
            // Look ahead: is there a better digit coming?
            char current = bank[i];
            int window = remaining - need + 1;  // How far ahead to check
            
            int found_better = 0;
            for (int j = i + 1; j < i + window && j < length; j++) {
                if (bank[j] > current) {
                    found_better = 1;
                    break;
                }
            }
            
            // Skip current if we found something better
            if (found_better) {
                skipped++;
                continue;
            }
        }
        
        // Keep this battery
        result[result_idx++] = bank[i];
    }
    result[result_idx] = '\0';
    
    // Convert to long long
    long long joltage = 0;
    for (int i = 0; i < 12; i++) {
        joltage = joltage * 10 + (result[i] - '0');
    }
    
    return joltage;
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
    
    while (fgets(line, sizeof(line), fp)) {
        // Remove newline
        int len = strlen(line);
        if (len > 0 && line[len-1] == '\n') {
            line[len-1] = '\0';
            len--;
        }
        
        if (len == 0) continue;
        
        long long joltage = find_max_joltage_12(line, len);
        total += joltage;
        
        printf("Bank %d: %12lld jolts\n", bank_num++, joltage);
    }
    
    fclose(fp);
    
    printf("\nTotal joltage: %lld\n", total);
    
    return 0;
}