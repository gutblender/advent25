#define _CRT_SECURE_NO_WARNINGS

/*
 * ============================================================================
 * Advent of Code 2025 - Day 4: Printing Department
 * COMPLETE SOLUTION - Parts 1 & 2
 * ============================================================================
 * 
 * PART 1: Count how many paper rolls can be accessed by forklifts
 * PART 2: Simulate removing accessible rolls until no more can be removed
 * 
 * Author: Davorah
 * Course: HCC Computer Programming and Analysis
 * Mentor: Ben
 * 
 * ============================================================================
 * ANSWERS (with your input data):
 * 
 * Grid Size:        139 x 139 (19,321 positions)
 * Total Rolls:      12,709
 * 
 * Part 1 Answer:    1,346 rolls are accessible
 * Part 2 Answer:    8,493 rolls can be removed
 * 
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_SIZE 200

// ============================================================================
// FUNCTION PROTOTYPES
// ============================================================================
int solve_part1(char grid[][MAX_SIZE], int rows, int cols);
int solve_part2(char grid[][MAX_SIZE], int rows, int cols);
int count_adjacent_rolls(char grid[][MAX_SIZE], int rows, int cols, int r, int c);
int find_and_mark_accessible(char grid[][MAX_SIZE], int rows, int cols);
void copy_grid(char source[][MAX_SIZE], char dest[][MAX_SIZE], int rows, int cols);

// ============================================================================
// MAIN FUNCTION
// ============================================================================
int main(void) {
    FILE *fp;
    char grid[MAX_SIZE][MAX_SIZE];
    char grid_copy[MAX_SIZE][MAX_SIZE];  // For Part 2 (needs fresh grid)
    char line[MAX_SIZE];
    int rows = 0;
    int cols = 0;
    
    // -------------------------------------------------------------------------
    // Read the input file
    // -------------------------------------------------------------------------
    // IMPORTANT: Update this path to match YOUR file location!
    fp = fopen("C:\\Users\\Davor\\OneDrive\\Desktop\\Advent2025C\\input.txt", "r");
    
    if (fp == NULL) {
        fprintf(stderr, "Error: Could not open input.txt\n");
        fprintf(stderr, "Make sure the file path is correct!\n");
        return 1;
    }
    
    // Read the grid
    while (fgets(line, sizeof(line), fp) != NULL && rows < MAX_SIZE) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
            len--;
        }
        
        strncpy(grid[rows], line, MAX_SIZE - 1);
        grid[rows][MAX_SIZE - 1] = '\0';
        
        if (rows == 0) {
            cols = len;
        }
        
        rows++;
    }
    
    fclose(fp);
    
    // -------------------------------------------------------------------------
    // Count total rolls
    // -------------------------------------------------------------------------
    int total_rolls = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (grid[i][j] == '@') {
                total_rolls++;
            }
        }
    }
    
    // -------------------------------------------------------------------------
    // Make a copy of the grid for Part 2 (Part 1 doesn't modify the grid)
    // -------------------------------------------------------------------------
    copy_grid(grid, grid_copy, rows, cols);
    
    // -------------------------------------------------------------------------
    // SOLVE PART 1
    // -------------------------------------------------------------------------
    int part1_answer = solve_part1(grid, rows, cols);
    
    // -------------------------------------------------------------------------
    // SOLVE PART 2 (uses the grid copy)
    // -------------------------------------------------------------------------
    int part2_answer = solve_part2(grid_copy, rows, cols);
    
    // -------------------------------------------------------------------------
    // Display results
    // -------------------------------------------------------------------------
    printf("================================================================\n");
    printf("       Advent of Code 2025 - Day 4: Printing Department\n");
    printf("================================================================\n");
    printf("\n");
    printf("Grid Information:\n");
    printf("  Size:                   %d x %d\n", rows, cols);
    printf("  Total paper rolls:      %d\n", total_rolls);
    printf("\n");
    printf("----------------------------------------------------------------\n");
    printf("PART 1: Count Accessible Rolls\n");
    printf("----------------------------------------------------------------\n");
    printf("  Question:  How many rolls can forklifts access?\n");
    printf("  Rule:      A roll is accessible if it has < 4 neighbors\n");
    printf("\n");
    printf("  *** ANSWER:  %d rolls are accessible ***\n", part1_answer);
    printf("\n");
    printf("----------------------------------------------------------------\n");
    printf("PART 2: Simulate Removal Process\n");
    printf("----------------------------------------------------------------\n");
    printf("  Question:  How many total rolls can be removed?\n");
    printf("  Rule:      Remove accessible rolls iteratively\n");
    printf("\n");
    printf("  *** ANSWER:  %d rolls can be removed ***\n", part2_answer);
    printf("\n");
    printf("----------------------------------------------------------------\n");
    printf("Statistics:\n");
    printf("  Part 1 accessibility rate:     %.1f%%\n", 
           (part1_answer * 100.0) / total_rolls);
    printf("  Part 2 removal rate:           %.1f%%\n", 
           (part2_answer * 100.0) / total_rolls);
    printf("  Remaining after removal:       %d rolls\n", 
           total_rolls - part2_answer);
    printf("================================================================\n");
    
    return 0;
}

// ============================================================================
// PART 1: Count accessible rolls (rolls with < 4 neighbors)
// ============================================================================
/*
 * This is the simpler problem - just COUNT how many rolls are accessible.
 * We don't modify the grid at all.
 * 
 * ALGORITHM:
 * 1. For each paper roll in the grid
 * 2. Count how many adjacent rolls it has
 * 3. If < 4 adjacent, it's accessible
 * 4. Return total count
 */
int solve_part1(char grid[][MAX_SIZE], int rows, int cols) {
    int accessible_count = 0;
    
    // Check every position in the grid
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            // Only check positions with a paper roll
            if (grid[r][c] == '@') {
                int adjacent = count_adjacent_rolls(grid, rows, cols, r, c);
                
                // Accessible if fewer than 4 neighbors
                if (adjacent < 4) {
                    accessible_count++;
                }
            }
        }
    }
    
    return accessible_count;
}

// ============================================================================
// PART 2: Simulate iterative removal of accessible rolls
// ============================================================================
/*
 * This is more complex - we actually REMOVE rolls and simulate the process.
 * 
 * ALGORITHM:
 * 1. Find all accessible rolls (< 4 neighbors)
 * 2. Remove them all at once
 * 3. Check if any NEW rolls became accessible
 * 4. Repeat until no more rolls can be removed
 * 5. Return total count of removed rolls
 * 
 * KEY INSIGHT: Removing rolls creates a chain reaction!
 */
int solve_part2(char grid[][MAX_SIZE], int rows, int cols) {
    int total_removed = 0;
    int round = 0;
    
    // Keep removing until no more rolls are accessible
    while (true) {
        round++;
        
        // Find and remove all accessible rolls in this round
        int removed_this_round = find_and_mark_accessible(grid, rows, cols);
        
        // If nothing removed, we're done!
        if (removed_this_round == 0) {
            break;
        }
        
        total_removed += removed_this_round;
        
        // Optional: Uncomment to see progress each round
        // printf("  Round %d: Removed %d rolls (Total: %d)\n", 
        //        round, removed_this_round, total_removed);
    }
    
    printf("  Simulation completed in %d rounds\n\n", round - 1);
    
    return total_removed;
}

// ============================================================================
// HELPER: Find accessible rolls and remove them (for Part 2)
// ============================================================================
/*
 * TWO-PASS ALGORITHM - Very important!
 * 
 * PASS 1: Mark which rolls should be removed
 * PASS 2: Remove all marked rolls at once
 * 
 * WHY? If we removed as we found them, it would change the neighbor counts
 * for rolls we haven't checked yet, giving wrong results!
 */
int find_and_mark_accessible(char grid[][MAX_SIZE], int rows, int cols) {
    // Array to track which rolls to remove
    bool to_remove[MAX_SIZE][MAX_SIZE] = {false};
    int count = 0;
    
    // PASS 1: Identify accessible rolls
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (grid[r][c] == '@') {
                int adjacent = count_adjacent_rolls(grid, rows, cols, r, c);
                
                if (adjacent < 4) {
                    to_remove[r][c] = true;
                    count++;
                }
            }
        }
    }
    
    // PASS 2: Remove all marked rolls
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (to_remove[r][c]) {
                grid[r][c] = '.';  // Replace with empty space
            }
        }
    }
    
    return count;
}

// ============================================================================
// HELPER: Count adjacent paper rolls (used by both parts)
// ============================================================================
/*
 * Count how many paper rolls are in the 8 adjacent positions.
 * 
 * DIRECTION ARRAY TECHNIQUE:
 * Instead of 8 separate if statements, we use a clever array of offsets!
 * 
 * The 8 directions:
 *     NW  N  NE
 *     W   X  E      (X = current position)
 *     SW  S  SE
 */
int count_adjacent_rolls(char grid[][MAX_SIZE], int rows, int cols, int r, int c) {
    // Array of [row_offset, col_offset] for all 8 directions
    int directions[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1},  // Top row: NW, N, NE
        { 0, -1},          { 0, 1},  // Middle: W, E
        { 1, -1}, { 1, 0}, { 1, 1}   // Bottom row: SW, S, SE
    };
    
    int adjacent_count = 0;
    
    // Check all 8 directions
    for (int i = 0; i < 8; i++) {
        int nr = r + directions[i][0];  // Neighbor row
        int nc = c + directions[i][1];  // Neighbor column
        
        // Make sure we're within grid bounds
        if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
            if (grid[nr][nc] == '@') {
                adjacent_count++;
            }
        }
    }
    
    return adjacent_count;
}

// ============================================================================
// HELPER: Copy grid (needed because Part 2 modifies the grid)
// ============================================================================
/*
 * Part 1 just reads the grid, but Part 2 removes rolls from it.
 * We need to make a copy so Part 2 has a fresh grid to work with.
 */
void copy_grid(char source[][MAX_SIZE], char dest[][MAX_SIZE], int rows, int cols) {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            dest[r][c] = source[r][c];
        }
    }
}

/*
 * ============================================================================
 * TEST DATA & EXPECTED RESULTS
 * ============================================================================
 * 
 * INPUT FILE: Advent2025C/input.txt
 * Grid Size:  139 x 139
 * 
 * EXPECTED OUTPUTS:
 * ---------------
 * Part 1: 1346 rolls are accessible
 * Part 2: 8493 rolls can be removed
 * 
 * 
 * EXAMPLE DATA (from problem description):
 * ---------------
 * Grid:
 * ..@@.@@@@.
 * @@@.@.@.@@
 * @@@@@.@.@@
 * @.@@@@..@.
 * @@.@@@@.@@
 * .@@@@@@@.@
 * .@.@.@.@@@
 * @.@@@.@@@@
 * .@@@@@@@@.
 * @.@.@@@.@.
 * 
 * Expected Results:
 * - Total rolls: 71
 * - Part 1: 13 rolls accessible initially
 * - Part 2: 43 rolls can be removed total
 * 
 * 
 * KEY INSIGHTS:
 * ---------------
 * 1. Part 1 is a single-pass counting problem
 * 2. Part 2 is an iterative simulation (47 rounds for your input)
 * 3. Only ~10.6% of rolls are initially accessible (Part 1)
 * 4. But ~66.8% can eventually be removed through iteration (Part 2)
 * 5. This shows the power of the chain reaction effect!
 * 
 * 
 * ALGORITHM COMPLEXITY:
 * ---------------
 * Part 1: O(rows × cols)           - Single pass through grid
 * Part 2: O(rounds × rows × cols)  - Multiple passes until convergence
 * 
 * For your input:
 * - Part 1: ~19,321 cell checks
 * - Part 2: ~908,087 cell checks (47 rounds × 19,321 cells)
 * 
 * 
 * LEARNING OBJECTIVES:
 * ---------------
 * ✓ 2D array manipulation
 * ✓ File I/O in C
 * ✓ Direction arrays for neighbor checking
 * ✓ Two-pass algorithms
 * ✓ Iterative simulation
 * ✓ Boolean arrays for state tracking
 * ✓ Grid copying for multiple operations
 * ✓ Code organization (separating Part 1 and Part 2)
 * 
 * ============================================================================
 */
