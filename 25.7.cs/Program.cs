namespace _25._7.cs;

internal class Program
{
    static void Main(string[] args)
    {
        Console.WriteLine("Hello, World! Day 7 here.\nPaste Input: ");

        ulong[] beams;
        ulong sum;
        int s, numSplits = 0;

        string? line;
        if (null != (line = Console.ReadLine()) && 0 <= (s = line.IndexOf('S')))
        {
            beams = new ulong[line.Length];
            beams[s] = 1;
            sum = 1;
        }
        else return;

        for (numSplits = 0; !string.IsNullOrEmpty(line = Console.ReadLine()); )
        {
            // step thru positions of splitters
            //foreach (int beam in beams)
            for (int i = 0; i < line.Length; i++)
                if (beams[i] > 0 && line[i] == '^')
                {
                    numSplits++; // for part 1

                    ulong beam = beams[i];
                    beams[i] = 0; // can't penetrate a splitter. TODO: test Side-by side splitters. But none in input.
                    sum -= beam; // bear with me now.

                    if (i > 0 && line[i+1] != '^')
                    {
                        beams[i + 1] += beam;
                        sum += beam;
                    }
                    if (i < (beams.Length - 1) && line[i - 1] != '^')
                    {
                        beams[i - 1] += beam;
                        sum += beam;
                    }
                }
        }

        Console.WriteLine($"Part 1 solution: {numSplits}");

        Console.WriteLine($"Part 2 solution: {sum}");
    }
}
