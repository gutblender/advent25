/*
 * Advent of Code 2025 - Day 1 PART 2: Circular Dial Lock
 * =======================================================
 * 
 * Problem: Same circular dial (positions 0-99, starting at 50), but now we need
 * to count EVERY TIME the dial points at 0 DURING a rotation, not just at the end!
 * 
 * Key Difference from Part 1:
 * - Part 1: Only counted if we LANDED on 0 after a rotation
 * - Part 2: Count EVERY TIME we PASS THROUGH 0 during a rotation
 * 
 * Author: Davorah
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Function: countZeroCrossings
 * -----------------------------
 * Counts how many times position 0 appears during a rotation.
 * Uses simple step-by-step counting for accuracy.
 */
int countZeroCrossings(int startPosition, char direction, int distance) {
    int crossings = 0;
    int currentPos = startPosition;
    int i;
    
    // Step through each position in the rotation
    for (i = 0; i < distance; i++) {
        if (direction == 'R') {
            currentPos++;
            if (currentPos >= 100) {
                currentPos = 0;
            }
        } else if (direction == 'L') {
            currentPos--;
            if (currentPos < 0) {
                currentPos = 99;
            }
        }
        
        // Check if we're at position 0
        if (currentPos == 0) {
            crossings++;
        }
    }
    
    return crossings;
}

/*
 * Function: solveDialLockPart2
 * ----------------------------
 * Reads rotations and counts all zero crossings.
 */
int solveDialLockPart2(const char* filename) {
    FILE* file;
    char line[20];
    int position = 50;
    int totalZeroCrossings = 0;
    int rotationNumber = 0;
    
    file = fopen(filename, "r");
    
    if (file == NULL) {
        printf("Error: Could not open file '%s'\n", filename);
        return -1;
    }
    
    printf("Reading file... Processing rotations...\n\n");
    
    while (fgets(line, sizeof(line), file) != NULL) {
        char direction;
        int distance;
        int crossings;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) == 0) {
            continue;
        }
        
        direction = line[0];
        distance = atoi(&line[1]);
        
        rotationNumber++;
        
        // Count crossings for this rotation
        crossings = countZeroCrossings(position, direction, distance);
        
        totalZeroCrossings += crossings;
        
        // Update position for next rotation
        if (direction == 'L') {
            position = position - distance;
            while (position < 0) {
                position = position + 100;
            }
        } else if (direction == 'R') {
            position = position + distance;
            while (position >= 100) {
                position = position - 100;
            }
        }
        
        // Debug output for rotations with crossings
        if (rotationNumber <= 15 && crossings > 0) {
            printf("Rotation %d: %c%d -> Crossed 0 %d time(s)\n", 
                   rotationNumber, direction, distance, crossings);
        }
    }
    
    fclose(file);
    
    printf("\nProcessed %d rotations total.\n", rotationNumber);
    
    return totalZeroCrossings;
}

int main() {
    const char* inputFile = "Advent2025Day1InputData.txt";
    int answer;
    
    printf("============================================================\n");
    printf("  Advent of Code 2025 - Day 1 PART 2 (C Version)\n");
    printf("  REVISED - Step-by-Step Counting\n");
    printf("============================================================\n\n");
    
    answer = solveDialLockPart2(inputFile);
    
    if (answer == -1) {
        printf("\nFailed to solve the puzzle due to file error.\n");
        return 1;
    }
    
    printf("\n============================================================\n");
    printf("Answer: %d\n", answer);
    printf("\nThe dial pointed at position 0 exactly %d times!\n", answer);
    printf("============================================================\n");
    
    return 0;
}