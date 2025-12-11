using System.Text.RegularExpressions;

namespace _25._10.cs;

internal class Program
{
    static readonly Regex regLine = new(@"\[(?<indicators>[.#]+)\] (?<buttons>[0-9, ()]+) {(?<joltages>[0-9,]+)}");
    static void Main(string[] args)
    {
        Console.WriteLine("Hello, World! Day 10 here.\nPaste Input: ");

        List<uint> buttons = new(64); // reuse the same list.
        List<uint> joltages = new(64); // reuse the same list.
        uint part1sum = 0;
        for (string? line; !string.IsNullOrWhiteSpace(line = Console.ReadLine()); )
        {
            var match = regLine.Match(line);
            if (!match.Success)
                break;

            uint target = 0; // target state of the indicators
            {
                string ind = match.Groups["indicators"].Value;
                uint mask = 1;
                for (int i = 0; i < ind.Length; i++, mask <<= 1)
                    if (ind[i] != '.')
                        target |= mask;
            }

            buttons.Clear();
            foreach (var sButton in match.Groups["buttons"].Value.Split(' ', StringSplitOptions.RemoveEmptyEntries))
            {
                uint button = 0;
                var split = sButton.Trim('(', ')').Split(',');
                foreach (var str in split)
                {
                    int bit = int.Parse(str);
                    button |= 1U << bit;
                }
                buttons.Add(button);
            }

            joltages.Clear(); // for part 2: joltages
            foreach (var s in match.Groups["joltages"].Value.Trim('{', '}').Split(',', StringSplitOptions.RemoveEmptyEntries))
                joltages.Add(uint.Parse(s));

            uint bestCnt = uint.MaxValue; // of all the cunts in the world, you are the best.
            for (uint mask = 0, max = 1U << buttons.Count; mask < max; mask++)
            {
                uint indicators = 0, m = mask, cnt = 0;
                for (int bit = 0; m != 0; bit++, m >>= 1)
                    if (0 != (m & 1U))
                    {
                        indicators ^= buttons[bit];
                        cnt++; 
                        if (indicators == target)
                        {
                            if (bestCnt > cnt)
                                bestCnt = cnt;
                        }
                    }
            }
            part1sum += bestCnt;
        }
        Console.WriteLine($"Part 1 solution: {part1sum}");
    }

    uint CountBits(uint value)
    {
        uint cnt;
        for (cnt = 0; value != 0; value >>= 1)
            if (0 != (value & 1))
                cnt++;
        return cnt;
    }
}
