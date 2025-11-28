#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

int main() {
    // Vectors to store the two lists
    vector<int> leftList;
    vector<int> rightList;
    
    // Read input from file
    ifstream inputFile("input.txt");
    int left, right;
    
    while (inputFile >> left >> right) {
        leftList.push_back(left);
        rightList.push_back(right);
    }
    inputFile.close();
    
    // Sort both lists
    sort(leftList.begin(), leftList.end());
    sort(rightList.begin(), rightList.end());
    
    // Calculate total distance
    int totalDistance = 0;
    for (size_t i = 0; i < leftList.size(); i++) {
        totalDistance += abs(leftList[i] - rightList[i]);
    }
    
    cout << "Total distance: " << totalDistance << endl;
    
    return 0;
}