using System.Drawing;

namespace _25._4.cs;

internal class Program
{
    const int TooManyRolls = 4;
    const char Roll = '@';
    static void Main(string[] args)
    {
        Console.WriteLine("Hello, World! Day 4 here.\nPaste Input: ");

        List<char[]> map = [];

        for (string? line; !string.IsNullOrWhiteSpace(line = Console.ReadLine()); )
            map.Add(line.ToArray());

        int total = Solution(map);
        Console.WriteLine($"Part 1 solution: {total}");

        total = 0; // restart progress and start marking up the map
        for (int progress; ; total += progress)
            if (0 >= (progress = Solution(map, 'x')))
                break;

        Console.WriteLine($"Part 2 solution: {total}");

        int Solution(List<char[]> map, char changeFoundTo = Roll)
        {
            int total = 0;
            for (int y = 0; y < map.Count; y++)
                for (int x = 0, cnt = 0; x < map[y].Length; x++, cnt = 0)
                { // at this position, look all around.
                    if (map[y][x] != Roll)
                        continue;
                    for (int j = Math.Max(0, y - 1); cnt < TooManyRolls && j <= Math.Min(y + 1, map.Count - 1); j++) // y lookaround
                        for (int i = Math.Max(0, x - 1); cnt < TooManyRolls && i <= Math.Min(x + 1, map[j].Length - 1); i++) // x lookaround
                            if (i == x && j == y)
                                continue; // skip the roll I'm on.
                            else if (map[j][i] == Roll) 
                                ++cnt;
                    if (cnt < TooManyRolls)
                    {
                        total++;
                        map[y][x] = changeFoundTo;
                    }
                }
            return total;
        }
    }
}
