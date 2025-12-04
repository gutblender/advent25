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

        for (int progress; ; total += progress)
            if (0 >= (progress = Solution(map)))
                break;

        Console.WriteLine($"Part 2 solution: {total}");

        int Solution(List<char[]> map)
        {
            int total = 0;
            for (int y = 0; y < map.Count; y++)
            {
                for (int x = 0; x < map[y].Length; x++)
                {
                    if (map[y][x] != Roll)
                        continue; // not a roll, fuck you!

                    // at this position, look all around.
                    int cnt = 0; // are there less than TooManyRolls rolls around me?
                    bool ok = true;

                    for (int j = Math.Max(0, y - 1); ok && j <= Math.Min(y + 1, map.Count - 1); j++) // y lookaround
                    {
                        for (int i = Math.Max(0, x - 1); ok && i <= Math.Min(x + 1, map[j].Length - 1); i++) // x lookaround
                        {
                            if (i == x && j == y)
                                continue; // skip the roll I'm on.

                            if (map[j][i] == Roll && ++cnt >= TooManyRolls)
                                ok = false;
                        }
                    }

                    if (ok)
                    {
                        total++;
                        map[y][x] = 'x';
                    }
                }
            }
            return total;
        }
    }
}
