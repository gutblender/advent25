namespace _25._5.cs;

internal class Program
{
    static void Main(string[] args)
    {
        Console.WriteLine("Hello, World! Day 5 here.\nPaste Input: ");

        List<Range> ranges = [];
        for (string? line; !string.IsNullOrWhiteSpace(line = Console.ReadLine()); )
        { // read ranges
            var split = line.Split('-');
            if (!long.TryParse(split[0], out var left) || !long.TryParse(split[1], out var right))
                break;

            ranges.Add(new(left, right));
        }

        int numFresh = 0;
        for (string? line; !string.IsNullOrWhiteSpace(line = Console.ReadLine()); )
        { // read IDs
            if (long.TryParse(line, out long id))
                if (ranges.Any(range => range.Contains(id)))
                    numFresh++;
        }

        Console.WriteLine($"Part 1 solution: {numFresh}");

        // Part 2: reduce overlapped ranges
        for (bool cut = true; cut;)
        {
            cut = false; // gonna fail unless someone gets cut
            for (int left = 0; left < ranges.Count - 1; left++)
                for (int right = left + 1; right < ranges.Count; right++)
                    if (ranges[left].TryJoin(ranges[right], out Range newLeft))
                    {
                        ranges[left] = newLeft;
                        ranges.RemoveAt(right);
                        cut = true;
                    }
        }

        long totalFreshInTheory = ranges.Sum(range => range.Count);
        Console.WriteLine($"Part 2 solution: {totalFreshInTheory}");
    }

    sealed record Range(long left, long right)
    {
        public static Range Zero = new (0, 0);
        public bool Contains(long id) => id >= left && id <= right;
        public long Count => right - left + 1;
        public bool TryJoin(Range range, out Range output)
        {
            if (range.left > (right + 1) || range.right < (left - 1))
            {
                output = Zero;
                return false;
            }
            output = new(Math.Min(left, range.left), Math.Max(right, range.right));
            return true;
        }
    }
}
