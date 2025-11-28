import java.util.*;
import java.io.*;

public class Day1 {
    public static void main(String[] args) {
        try {
            // Create lists to store the numbers
            List<Integer> leftList = new ArrayList<>();
            List<Integer> rightList = new ArrayList<>();
            
            // Read the input file
            BufferedReader br = new BufferedReader(new FileReader("input.txt"));
            String line;
            
            // Parse each line and split into left and right numbers
            while ((line = br.readLine()) != null) {
                String[] parts = line.trim().split("\\s+");
                leftList.add(Integer.parseInt(parts[0]));
                rightList.add(Integer.parseInt(parts[1]));
            }
            br.close();
            
            // Sort both lists
            Collections.sort(leftList);
            Collections.sort(rightList);
            
            // Calculate the total distance
            int totalDistance = 0;
            for (int i = 0; i < leftList.size(); i++) {
                int distance = Math.abs(leftList.get(i) - rightList.get(i));
                totalDistance += distance;
            }
            
            // Print the final answer
            System.out.println("Total distance: " + totalDistance);
            
        } catch (IOException e) {
            System.out.println("Error reading file: " + e.getMessage());
        }
    }
}