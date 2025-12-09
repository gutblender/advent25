using System.Drawing;
using System.Text.RegularExpressions;

namespace _25._9.cs;

internal class Program
{
    static readonly Regex regVertex = new(@"(?<x>[0-9]+),(?<y>[0-9]+)");
    static void Main(string[] args)
    {
        Console.WriteLine("Hello, World! Day 9 here.\nPaste Input: ");

        List<Point> points = [];
        for (string? line; !string.IsNullOrEmpty(line = Console.ReadLine()); )
        {
            var match = regVertex.Match(line);
            if (!match.Success)
                break;

            int x = int.Parse(match.Groups["x"].Value);
            int y = int.Parse(match.Groups["y"].Value);
            points.Add(new (x, y));
        }

        ulong bestArea = 0;
        for (int i = 0; i < points.Count-1; i++)
            for (int j = i+1; j < points.Count; j++)
            {
                ulong area = Area(points[i], points[j]);
                if (bestArea < area)
                    bestArea = area;
            }

        Console.WriteLine($"Part 1 solution: {bestArea}");
    }

    static ulong Area(Point p1, Point p2)
    {
        ulong dx = (ulong)Math.Abs(p1.X - p2.X + 1);
        ulong dy = (ulong)Math.Abs(p1.Y - p2.Y + 1);
        return dx * dy;
    }
}
