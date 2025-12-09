#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

// Structure to hold a junction box's 3D coordinates
typedef struct {
    int x, y, z;
} Point;

// Structure to represent an edge (connection) between two junction boxes
typedef struct {
    int box1;      // Index of first junction box
    int box2;      // Index of second junction box
    double distance;  // Distance between them
} Edge;

// Union-Find data structure for tracking connected circuits
int parent[1000];
int rank_array[1000];

// Function: Initialize Union-Find structure
void init_union_find(int n) {
    for (int i = 0; i < n; i++) {
        parent[i] = i;
        rank_array[i] = 0;
    }
}

// Function: Find the root parent of a junction box
int find(int x) {
    if (parent[x] != x) {
        parent[x] = find(parent[x]);
    }
    return parent[x];
}

// Function: Connect two circuits together
// Returns 1 if they were merged, 0 if already in same circuit
int union_sets(int x, int y) {
    int root_x = find(x);
    int root_y = find(y);

    if (root_x == root_y) {
        return 0;
    }

    if (rank_array[root_x] < rank_array[root_y]) {
        parent[root_x] = root_y;
    }
    else if (rank_array[root_x] > rank_array[root_y]) {
        parent[root_y] = root_x;
    }
    else {
        parent[root_y] = root_x;
        rank_array[root_x]++;
    }

    return 1;
}

// Manual square root function (no library needed)
// Uses Newton's method for approximation
double my_sqrt(double x) {
    if (x == 0) return 0;
    double guess = x / 2.0;
    double epsilon = 0.00001;
    while ((guess * guess - x) > epsilon || (x - guess * guess) > epsilon) {
        guess = (guess + x / guess) / 2.0;
    }
    return guess;
}

// Function: Calculate 3D Euclidean distance between two points
// Formula: sqrt((x2-x1)^2 + (y2-y1)^2 + (z2-z1)^2)
double calculate_distance(Point p1, Point p2) {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    double dz = p2.z - p1.z;
    return my_sqrt(dx * dx + dy * dy + dz * dz);
}

// Comparison function for qsort - sorts edges by distance (smallest first)
int compare_edges(const void* a, const void* b) {
    Edge* edge_a = (Edge*)a;
    Edge* edge_b = (Edge*)b;

    if (edge_a->distance < edge_b->distance) return -1;
    if (edge_a->distance > edge_b->distance) return 1;
    return 0;
}

// Comparison function for qsort - sorts integers in descending order
int compare_int_desc(const void* a, const void* b) {
    return (*(int*)b - *(int*)a);
}

// Function: Count how many separate circuits exist
int count_circuits(int num_boxes) {
    int circuit_count = 0;
    for (int i = 0; i < num_boxes; i++) {
        // If a box is its own parent, it's a root (separate circuit)
        if (find(i) == i) {
            circuit_count++;
        }
    }
    return circuit_count;
}

int main() {
    Point boxes[1000];
    int num_boxes = 0;

    // Step 1: Read the input file
    FILE* file = fopen("C:\\Users\\Davor\\OneDrive\\Desktop\\Advent2025C\\AdventDay8\\input.txt", "r");
    if (file == NULL) {
        printf("Error: Could not open input.txt file\n");
        return 1;
    }

    // Parse each line: X,Y,Z coordinates
    while (fscanf(file, "%d,%d,%d", &boxes[num_boxes].x,
        &boxes[num_boxes].y, &boxes[num_boxes].z) == 3) {
        num_boxes++;
    }
    fclose(file);

    printf("Read %d junction boxes\n\n", num_boxes);

    // Step 2: Calculate all possible distances between junction boxes
    int total_edges = (num_boxes * (num_boxes - 1)) / 2;
    Edge* edges = (Edge*)malloc(total_edges * sizeof(Edge));
    int edge_count = 0;

    // Generate all pairs of junction boxes and calculate their distances
    for (int i = 0; i < num_boxes; i++) {
        for (int j = i + 1; j < num_boxes; j++) {
            edges[edge_count].box1 = i;
            edges[edge_count].box2 = j;
            edges[edge_count].distance = calculate_distance(boxes[i], boxes[j]);
            edge_count++;
        }
    }

    // Step 3: Sort all edges by distance (shortest first)
    qsort(edges, edge_count, sizeof(Edge), compare_edges);

    // ===== PART 1: Make 1000 connections =====
    printf("========== PART 1 ==========\n");
    init_union_find(num_boxes);

    // Connect the 1000 shortest pairs
    for (int i = 0; i < 1000 && i < edge_count; i++) {
        union_sets(edges[i].box1, edges[i].box2);
    }

    // Count the size of each circuit
    int circuit_sizes[1000] = { 0 };
    for (int i = 0; i < num_boxes; i++) {
        int root = find(i);
        circuit_sizes[root]++;
    }

    // Find the three largest circuits
    int sizes[1000] = { 0 };
    int size_count = 0;
    for (int i = 0; i < num_boxes; i++) {
        if (circuit_sizes[i] > 0) {
            sizes[size_count++] = circuit_sizes[i];
        }
    }

    // Sort circuit sizes in descending order
    qsort(sizes, size_count, sizeof(int), compare_int_desc);

    printf("After 1000 connections:\n");
    printf("Number of circuits: %d\n", size_count);
    printf("Three largest circuits: %d, %d, %d\n", sizes[0], sizes[1], sizes[2]);

    long long part1_answer = (long long)sizes[0] * sizes[1] * sizes[2];
    printf("Part 1 Answer: %d * %d * %d = %lld\n\n",
        sizes[0], sizes[1], sizes[2], part1_answer);

    // ===== PART 2: Connect until all in one circuit =====
    printf("========== PART 2 ==========\n");
    init_union_find(num_boxes);  // Reset the union-find structure

    int last_box1 = -1, last_box2 = -1;
    int connections_made = 0;

    // Keep connecting until all boxes are in one circuit
    for (int i = 0; i < edge_count; i++) {
        if (union_sets(edges[i].box1, edges[i].box2)) {
            connections_made++;
            last_box1 = edges[i].box1;
            last_box2 = edges[i].box2;

            // Check if everything is now in one circuit
            if (count_circuits(num_boxes) == 1) {
                printf("All boxes connected after %d connections!\n", connections_made);
                printf("Last connection: Box %d (%d,%d,%d) to Box %d (%d,%d,%d)\n",
                    last_box1, boxes[last_box1].x, boxes[last_box1].y, boxes[last_box1].z,
                    last_box2, boxes[last_box2].x, boxes[last_box2].y, boxes[last_box2].z);
                printf("Distance: %.2f\n", edges[i].distance);
                break;
            }
        }
    }

    // Calculate Part 2 answer: multiply X coordinates
    long long part2_answer = (long long)boxes[last_box1].x * boxes[last_box2].x;
    printf("Part 2 Answer: %d * %d = %lld\n\n",
        boxes[last_box1].x, boxes[last_box2].x, part2_answer);

    // Clean up
    free(edges);

    printf("Press any key to continue...\n");
    getchar();

    return 0;
}