#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_ROWS 150
#define MAX_COLS 150
#define MAX_QUEUE 100000

typedef struct {
	int row;
	int col;
} Position;

typedef struct {
	Position data[MAX_QUEUE];
	int front;
	int rear;
	int size;
} Queue;

void initQueue(Queue* q) {
	q->front = 0;
	q->rear = 0;
	q->size = 0;
}

void enqueue(Queue* q, int row, int col) {
	if (q->size < MAX_QUEUE) {
		q->data[q->rear].row = row;
		q->data[q->rear].col = col;
		q->rear = (q->rear + 1) % MAX_QUEUE;
		q->size++;
	}
}

Position dequeue(Queue* q) {
	Position pos = q->data[q->front];
	q->front = (q->front + 1) % MAX_QUEUE;
	q->size--;
	return pos;
}

bool isEmpty(Queue* q) {
	return q->size == 0;
}

// Memoization array: -1 = not calculated, 0+ = number of paths from this position
long long memo[MAX_ROWS][MAX_COLS];

// Count paths from position (r,c) to the bottom
long long countPaths(int r, int c, char grid[MAX_ROWS][MAX_COLS], int rows, int cols) {
	// Out of bounds on sides - no paths
	if (c < 0 || c >= cols) {
		return 0;
	}

	// Reached bottom - this is 1 path!
	if (r >= rows) {
		return 1;
	}

	// Out of bounds top
	if (r < 0) {
		return 0;
	}

	// Already calculated?
	if (memo[r][c] >= 0) {
		return memo[r][c];
	}

	char cell = grid[r][c];
	long long paths = 0;

	if (cell == '^') {
		// Splitter - sum paths from both branches
		paths = countPaths(r + 1, c - 1, grid, rows, cols) +
			countPaths(r + 1, c + 1, grid, rows, cols);
	}
	else if (cell == '.' || cell == 'S') {
		// Continue down
		paths = countPaths(r + 1, c, grid, rows, cols);
	}

	// Memoize and return
	memo[r][c] = paths;
	return paths;
}

int main() {
	char grid[MAX_ROWS][MAX_COLS];
	bool visited[MAX_ROWS][MAX_COLS];
	int rows = 0;
	int cols = 0;
	int start_row = -1;
	int start_col = -1;

	printf("===========================================\n");
	printf("  Advent of Code 2024 - Day 7 Solutions   \n");
	printf("===========================================\n\n");

	// Read input
	FILE* file = fopen("Input.txt", "r");
	if (file == NULL) {
		printf("Error: Could not open Input.txt\n");
		return 1;
	}

	char line[MAX_COLS];
	while (fgets(line, sizeof(line), file) != NULL && rows < MAX_ROWS) {
		int len = strlen(line);
		if (len > 0 && line[len - 1] == '\n') {
			line[len - 1] = '\0';
			len--;
		}
		strcpy(grid[rows], line);
		if (rows == 0) cols = len;
		for (int c = 0; c < len; c++) {
			if (line[c] == 'S') {
				start_row = rows;
				start_col = c;
			}
		}
		rows++;
	}
	fclose(file);

	printf("Grid loaded: %d rows x %d cols\n", rows, cols);
	printf("Start position: (%d, %d)\n\n", start_row, start_col);

	// ========================================
	// PART 1
	// ========================================
	printf("Solving Part 1...\n");

	Queue queue;
	initQueue(&queue);
	memset(visited, false, sizeof(visited));
	enqueue(&queue, start_row, start_col);

	int split_count = 0;

	while (!isEmpty(&queue)) {
		Position current = dequeue(&queue);
		int r = current.row;
		int c = current.col;

		if (visited[r][c]) continue;
		if (r < 0 || r >= rows || c < 0 || c >= cols) continue;

		visited[r][c] = true;
		char cell = grid[r][c];

		if (cell == '^') {
			split_count++;
			enqueue(&queue, r + 1, c - 1);
			enqueue(&queue, r + 1, c + 1);
		}
		else if (cell == '.' || cell == 'S') {
			enqueue(&queue, r + 1, c);
		}
	}

	// ========================================
	// PART 2 - Path counting with memoization
	// ========================================
	printf("Solving Part 2...\n");

	// Initialize memoization array
	for (int r = 0; r < MAX_ROWS; r++) {
		for (int c = 0; c < MAX_COLS; c++) {
			memo[r][c] = -1;  // -1 means not yet calculated
		}
	}

	// Count all paths from start to bottom
	long long timeline_count = countPaths(start_row, start_col, grid, rows, cols);

	printf("\n");

	// ========================================
	// RESULTS
	// ========================================
	printf("===========================================\n");
	printf("                 RESULTS                   \n");
	printf("===========================================\n");
	printf("Part 1 - Beam Splits:       %d\n", split_count);
	printf("Part 2 - Timeline Exits:    %lld\n", timeline_count);
	printf("===========================================\n");

	return 0;
}