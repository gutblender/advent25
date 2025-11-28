import java.util.*;
import java.io.*;

public class Day1Part2 {
    public static void main(String[] args) {
        try {
            // Create lists to store the numbers
            List<Integer> leftList = new ArrayList<>();
            List<Integer> rightList = new ArrayList<>();
            
            // Read the input file
            BufferedReader br = new BufferedReader(new FileReader("input.txt"));
            String line;
            
            // Parse each line
            while ((line = br.readLine()) != null) {
                String[] parts = line.trim().split("\\s+");
                leftList.add(Integer.parseInt(parts[0]));
                rightList.add(Integer.parseInt(parts[1]));
            }
            br.close();
            
            // Calculate similarity score
            int similarityScore = 0;
            
            for (int leftNum : leftList) {
                // Count how many times leftNum appears in rightList
                int count = 0;
                for (int rightNum : rightList) {
                    if (leftNum == rightNum) {
                        count++;
                    }
                }
                // Add to similarity score
                similarityScore += leftNum * count;
            }
            
            // Print the answer
            System.out.println("Similarity score: " + similarityScore);
            
        } catch (IOException e) {
            System.out.println("Error reading file: " + e.getMessage());
        }
    }
}