// =============================================================================
// ADVENT OF CODE 2025 - DAY 6: TRASH COMPACTOR (COMPLETE SOLUTION)
// =============================================================================
// This program solves BOTH Part 1 and Part 2 of the puzzle.
//
// PART 1: Read numbers horizontally (each row contains one number per problem)
// PART 2: Read numbers vertically right-to-left (cephalopod style!)
// =============================================================================

// Disable Visual Studio security warnings for standard C functions
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 10000
#define MAX_NUMS 2000

// =============================================================================
// PART 1 FUNCTIONS
// =============================================================================

// Parse a line of space-separated numbers into an array
// Returns: number of values found
int parse_numbers(char* line, int numbers[]) {
    int count = 0;
    char* token = strtok(line, " \t\n");
    
    while (token != NULL) {
        numbers[count] = atoi(token);
        count++;
        token = strtok(NULL, " \t\n");
    }
    
    return count;
}

// Parse operators from a line (extract only * and +)
// Returns: number of operators found
int parse_operators(char* line, char operators[]) {
    int count = 0;
    
    for (int i = 0; line[i] != '\0' && line[i] != '\n'; i++) {
        if (line[i] == '*' || line[i] == '+') {
            operators[count] = line[i];
            count++;
        }
    }
    
    return count;
}

// Solve Part 1: Horizontal reading
long long solve_part1(char* line1, char* line2, char* line3, 
                      char* line4, char* line5) {
    
    printf("\n");
    printf("=====================================================\n");
    printf("PART 1: HORIZONTAL READING\n");
    printf("=====================================================\n\n");
    
    // Arrays to hold parsed values
    int row1[MAX_NUMS], row2[MAX_NUMS], row3[MAX_NUMS], row4[MAX_NUMS];
    char operators[MAX_NUMS];
    int count1, count2, count3, count4, count_ops;
    
    // Need to make copies because strtok modifies the string
    char temp1[MAX_LINE_LENGTH], temp2[MAX_LINE_LENGTH];
    char temp3[MAX_LINE_LENGTH], temp4[MAX_LINE_LENGTH];
    
    strcpy(temp1, line1);
    strcpy(temp2, line2);
    strcpy(temp3, line3);
    strcpy(temp4, line4);
    
    // Parse each line
    count1 = parse_numbers(temp1, row1);
    count2 = parse_numbers(temp2, row2);
    count3 = parse_numbers(temp3, row3);
    count4 = parse_numbers(temp4, row4);
    count_ops = parse_operators(line5, operators);
    
    printf("Parsed %d problems\n\n", count1);
    
    // Calculate grand total
    long long grand_total = 0;
    int num_problems = count1;
    
    for (int i = 0; i < num_problems; i++) {
        long long result;
        char op = operators[i];
        
        if (op == '*') {
            result = (long long)row1[i] * row2[i] * row3[i] * row4[i];
        } else {
            result = (long long)row1[i] + row2[i] + row3[i] + row4[i];
        }
        
        grand_total += result;
        
        // Print first 5 for debugging
        if (i < 5) {
            printf("Problem %d: %d %c %d %c %d %c %d = %lld\n", 
                   i+1, row1[i], op, row2[i], op, row3[i], op, row4[i], result);
        }
    }
    
    printf("\n--- PART 1 RESULT ---\n");
    printf("Total problems: %d\n", num_problems);
    printf("Grand Total: %lld\n", grand_total);
    printf("=====================================================\n");
    
    return grand_total;
}

// =============================================================================
// PART 2 FUNCTIONS
// =============================================================================

// Check if a column position contains only spaces (separator column)
int is_separator_column(char* line1, char* line2, char* line3, 
                       char* line4, char* line5, int col) {
    int len1 = strlen(line1);
    int len2 = strlen(line2);
    int len3 = strlen(line3);
    int len4 = strlen(line4);
    int len5 = strlen(line5);
    
    // Get character at this column from each line (or space if past end)
    char c1 = (col < len1) ? line1[col] : ' ';
    char c2 = (col < len2) ? line2[col] : ' ';
    char c3 = (col < len3) ? line3[col] : ' ';
    char c4 = (col < len4) ? line4[col] : ' ';
    char c5 = (col < len5) ? line5[col] : ' ';
    
    // A separator column has only spaces (or newlines/nulls)
    return (c1 == ' ' || c1 == '\n' || c1 == '\0') &&
           (c2 == ' ' || c2 == '\n' || c2 == '\0') &&
           (c3 == ' ' || c3 == '\n' || c3 == '\0') &&
           (c4 == ' ' || c4 == '\n' || c4 == '\0') &&
           (c5 == ' ' || c5 == '\n' || c5 == '\0');
}

// Safely get character from line (returns space if out of bounds)
char get_char(char* line, int col) {
    if (col >= strlen(line) || line[col] == '\n' || line[col] == '\0') {
        return ' ';
    }
    return line[col];
}

// Solve Part 2: Vertical reading, right-to-left (each column is ONE number)
long long solve_part2(char* line1, char* line2, char* line3, 
                      char* line4, char* line5) {
    
    printf("\n");
    printf("=====================================================\n");
    printf("PART 2: VERTICAL READING (RIGHT-TO-LEFT)\n");
    printf("=====================================================\n\n");
    
    // Find maximum line length
    int max_len = strlen(line1);
    if (strlen(line2) > max_len) max_len = strlen(line2);
    if (strlen(line3) > max_len) max_len = strlen(line3);
    if (strlen(line4) > max_len) max_len = strlen(line4);
    if (strlen(line5) > max_len) max_len = strlen(line5);
    
    printf("Processing %d character positions...\n\n", max_len);
    
    long long grand_total = 0;
    int problem_count = 0;
    
    // Start from the rightmost character
    int col = max_len - 1;
    
    while (col >= 0) {
        // Skip separator columns (spaces between problems)
        while (col >= 0 && 
               is_separator_column(line1, line2, line3, line4, line5, col)) {
            col--;
        }
        
        if (col < 0) break;  // Reached the end
        
        // Found a problem! Collect numbers from right to left
        long long numbers[1000];
        int num_count = 0;
        char op = ' ';
        
        while (col >= 0 && 
               !is_separator_column(line1, line2, line3, line4, line5, col)) {
            // Each column builds ONE number by reading top-to-bottom
            char num_str[20] = "";
            int idx = 0;
            
            char c1 = get_char(line1, col);
            char c2 = get_char(line2, col);
            char c3 = get_char(line3, col);
            char c4 = get_char(line4, col);
            char c5 = get_char(line5, col);
            
            // Build number from vertical digits (top to bottom)
            if (isdigit(c1)) num_str[idx++] = c1;
            if (isdigit(c2)) num_str[idx++] = c2;
            if (isdigit(c3)) num_str[idx++] = c3;
            if (isdigit(c4)) num_str[idx++] = c4;
            num_str[idx] = '\0';
            
            long long num = (strlen(num_str) > 0) ? atoll(num_str) : 0;
            numbers[num_count++] = num;
            
            // Get operator if present
            if (c5 == '*' || c5 == '+') {
                op = c5;
            }
            
            col--;
        }
        
        // Calculate result
        if (num_count == 0) continue;
        
        long long result;
        if (op == '*') {
            result = 1;
            for (int i = 0; i < num_count; i++) {
                if (numbers[i] > 0) result *= numbers[i];
            }
        } else {  // op == '+'
            result = 0;
            for (int i = 0; i < num_count; i++) {
                result += numbers[i];
            }
        }
        
        grand_total += result;
        problem_count++;
        
        // Print first 5 for debugging
        if (problem_count <= 5) {
            printf("Problem %d: ", problem_count);
            for (int i = num_count - 1; i >= 0; i--) {
                printf("%lld ", numbers[i]);
                if (i > 0) printf("%c ", op);
            }
            printf("= %lld\n", result);
        }
    }
    
    printf("\n--- PART 2 RESULT ---\n");
    printf("Total problems: %d\n", problem_count);
    printf("Grand Total: %lld\n", grand_total);
    printf("=====================================================\n");
    
    return grand_total;
}

// =============================================================================
// MAIN FUNCTION
// =============================================================================

int main() {
    FILE* file;
    char line1[MAX_LINE_LENGTH];
    char line2[MAX_LINE_LENGTH];
    char line3[MAX_LINE_LENGTH];
    char line4[MAX_LINE_LENGTH];
    char line5[MAX_LINE_LENGTH];
    
    printf("=============================================================================\n");
    printf("ADVENT OF CODE 2025 - DAY 6: TRASH COMPACTOR\n");
    printf("=============================================================================\n");
    
    // ===== OPEN AND READ FILE =====
    file = fopen("Day6_Input.txt", "r");
    if (file == NULL) {
        printf("\nError: Cannot open Day6_Input.txt!\n");
        printf("Make sure the file is in the same folder as your executable.\n\n");
        return 1;
    }
    
    printf("\nReading input file...\n");
    
    // Read all 5 lines
    fgets(line1, MAX_LINE_LENGTH, file);
    fgets(line2, MAX_LINE_LENGTH, file);
    fgets(line3, MAX_LINE_LENGTH, file);
    fgets(line4, MAX_LINE_LENGTH, file);
    fgets(line5, MAX_LINE_LENGTH, file);
    
    fclose(file);
    
    printf("File loaded successfully!\n");
    
    // ===== SOLVE PART 1 =====
    // Make copies for Part 1 (since it modifies the strings)
    char p1_line1[MAX_LINE_LENGTH], p1_line2[MAX_LINE_LENGTH];
    char p1_line3[MAX_LINE_LENGTH], p1_line4[MAX_LINE_LENGTH];
    char p1_line5[MAX_LINE_LENGTH];
    
    strcpy(p1_line1, line1);
    strcpy(p1_line2, line2);
    strcpy(p1_line3, line3);
    strcpy(p1_line4, line4);
    strcpy(p1_line5, line5);
    
    long long answer1 = solve_part1(p1_line1, p1_line2, p1_line3, p1_line4, p1_line5);
    
    // ===== SOLVE PART 2 =====
    // Use the original unmodified lines
    long long answer2 = solve_part2(line1, line2, line3, line4, line5);
    
    // ===== PRINT FINAL SUMMARY =====
    printf("\n");
    printf("=============================================================================\n");
    printf("FINAL ANSWERS\n");
    printf("=============================================================================\n");
    printf("Part 1 (Horizontal Reading):    %lld\n", answer1);
    printf("Part 2 (Vertical R-to-L):       %lld\n", answer2);
    printf("=============================================================================\n");
    printf("\n");
    
    return 0;
}
