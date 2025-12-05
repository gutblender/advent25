/*
 * Advent of Code 2025 - Day 1: Circular Dial Lock
 * =================================================
 * BEGINNER-FRIENDLY C VERSION
 * 
 * Problem: A circular dial has positions 0-99. Starting at position 50,
 * we perform a series of rotations (L=left/counterclockwise, R=right/clockwise).
 * Count how many times the dial points at position 0 after any rotation.
 * 
 * Author: Davorah
 * Version: 1.0 (Beginner C Edition)
 */

#include <stdio.h>   // For printf, fopen, fclose, fgets
#include <stdlib.h>  // For atoi (string to integer conversion)
#include <string.h>  // For strlen, strcspn

/*
 * Function: solveDialLock
 * -----------------------
 * Reads rotation instructions from a file and counts how many times
 * the dial points at position 0.
 * 
 * Parameters:
 *   filename - path to the input file containing rotations
 * 
 * Returns:
 *   The number of times the dial pointed at position 0
 */
int solveDialLock(const char* filename) {
    // Declare variables
    FILE* file;              // File pointer to read the input file
    char line[20];           // Buffer to hold each line from the file (each line is short)
    int position = 50;       // Starting position on the dial
    int zeroCount = 0;       // Counter for how many times we hit position 0
    
    // Try to open the file for reading
    file = fopen(filename, "r");
    
    // Check if file opened successfully
    if (file == NULL) {
        printf("Error: Could not open file '%s'\n", filename);
        printf("Make sure the file exists and the path is correct!\n");
        return -1;  // Return -1 to indicate error
    }
    
    printf("Reading file... Processing rotations...\n\n");
    
    // Read the file line by line
    // fgets reads one line at a time into the 'line' buffer
    while (fgets(line, sizeof(line), file) != NULL) {
        char direction;      // Will hold 'L' or 'R'
        int distance;        // Will hold the number after L or R
        
        // Remove newline character if present
        // strcspn finds where the newline is, and we replace it with \0 (end of string)
        line[strcspn(line, "\n")] = '\0';
        
        // Skip empty lines
        if (strlen(line) == 0) {
            continue;  // Skip to next iteration
        }
        
        // Parse the line
        // First character is the direction (L or R)
        direction = line[0];
        
        // Rest of the string is the number
        // atoi converts string to integer (atoi = "ASCII to Integer")
        // &line[1] means "start from character at index 1" (skip the first character)
        distance = atoi(&line[1]);
        
        // Perform the rotation based on direction
        if (direction == 'L') {
            // Turn LEFT - subtract the distance
            position = position - distance;
            
            // Handle wraparound when going below 0
            // Keep adding 100 until we're back in valid range (0-99)
            while (position < 0) {
                position = position + 100;
            }
        }
        else if (direction == 'R') {
            // Turn RIGHT - add the distance
            position = position + distance;
            
            // Handle wraparound when going above 99
            // Keep subtracting 100 until we're back in valid range (0-99)
            while (position >= 100) {
                position = position - 100;
            }
        }
        
        // Check if we landed on position 0
        if (position == 0) {
            zeroCount++;  // Increment our counter
        }
    }
    
    // Close the file (important! Always close files when done)
    fclose(file);
    
    // Return the final count
    return zeroCount;
}

/*
 * Function: main
 * --------------
 * The main entry point of the program.
 * Calls solveDialLock and displays the results.
 */
int main() {
    // Path to the input file
    // Using relative path - both files should be in the same folder!
    // If you get "Could not open file" error, use the full path instead:
    // const char* inputFile = "C:/Users/Davor/OneDrive/Desktop/Advent2025c/Advent2025Day1InputData.txt";
    const char* inputFile = "Advent2025Day1InputData.txt";
    
    // Variables to hold our answer
    int answer;
    
    // Print header
    printf("============================================================\n");
    printf("  Advent of Code 2025 - Day 1 Solution (C Version)\n");
    printf("============================================================\n\n");
    
    // Solve the puzzle by calling our function
    answer = solveDialLock(inputFile);
    
    // Check if there was an error
    if (answer == -1) {
        printf("\nFailed to solve the puzzle due to file error.\n");
        return 1;  // Exit with error code
    }
    
    // Display the results
    printf("\n============================================================\n");
    printf("Answer: %d\n", answer);
    printf("\nThe dial pointed at position 0 exactly %d times!\n", answer);
    printf("============================================================\n");
    
    return 0;  // Exit successfully
}

/*
 * =============================================================================
 * DETAILED EXPLANATION FOR BEGINNERS:
 * =============================================================================
 * 
 * THE PROBLEM:
 * ------------
 * Imagine you have a circular dial with numbers 0 through 99 (100 positions total).
 * You start at position 50.
 * 
 * You get instructions like:
 *   "L25" - Turn LEFT 25 positions (subtract 25)
 *   "R17" - Turn RIGHT 17 positions (add 17)
 * 
 * THE TRICKY PART - WRAPAROUND:
 * ------------------------------
 * The dial is circular, so:
 * - If you go past 99, you wrap back to 0
 * - If you go below 0, you wrap to 99
 * 
 * Think of Pac-Man going off one edge and appearing on the other!
 * 
 * EXAMPLES OF WRAPAROUND:
 * -----------------------
 * 
 * Example 1: Going too far RIGHT
 *   Start at position: 95
 *   Instruction: R10 (turn right 10)
 *   Calculation: 95 + 10 = 105
 *   But 105 > 99, so we wrap around
 *   While loop: 105 - 100 = 5
 *   Final position: 5
 * 
 * Example 2: Going too far LEFT
 *   Start at position: 5
 *   Instruction: L10 (turn left 10)
 *   Calculation: 5 - 10 = -5
 *   But -5 < 0, so we wrap around
 *   While loop: -5 + 100 = 95
 *   Final position: 95
 * 
 * Example 3: Multiple wraps
 *   Start at position: 10
 *   Instruction: L250 (turn left 250)
 *   Calculation: 10 - 250 = -240
 *   While loop iteration 1: -240 + 100 = -140 (still negative)
 *   While loop iteration 2: -140 + 100 = -40 (still negative)
 *   While loop iteration 3: -40 + 100 = 60 (done!)
 *   Final position: 60
 * 
 * YOUR GOAL:
 * ----------
 * Count how many times you land EXACTLY on position 0!
 * 
 * With 4,446 rotation instructions, the answer is: 1100 times
 * 
 * =============================================================================
 * HOW TO COMPILE AND RUN THIS PROGRAM:
 * =============================================================================
 * 
 * METHOD 1: Using GCC (most common C compiler)
 *   In your terminal/command prompt:
 *   
 *   gcc AdventDay1_Beginner_Solution.c -o advent
 *   ./advent
 * 
 *   On Windows:
 *   gcc AdventDay1_Beginner_Solution.c -o advent.exe
 *   advent.exe
 * 
 * METHOD 2: Using Visual Studio / Visual Studio Code
 *   1. Open the .c file
 *   2. Click "Run" or press F5
 * 
 * METHOD 3: Using an online C compiler
 *   1. Copy this code to onlinegdb.com or repl.it
 *   2. Upload your input file
 *   3. Update the filename in the code
 *   4. Click "Run"
 * 
 * =============================================================================
 * COMMON ERRORS AND HOW TO FIX THEM:
 * =============================================================================
 * 
 * Error: "Could not open file"
 *   Solution: Check that your file path is correct
 *   Windows paths need either:
 *   - Forward slashes: "C:/Users/Davor/Desktop/file.txt"
 *   - Double backslashes: "C:\\Users\\Davor\\Desktop\\file.txt"
 * 
 * Error: Compiler says "undefined reference to fopen"
 *   Solution: Make sure you have #include <stdio.h> at the top
 * 
 * Error: Program outputs 0 or wrong answer
 *   Solution: Check that your input file has all 4,446 lines of data
 * 
 * =============================================================================
 */