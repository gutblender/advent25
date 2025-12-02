namespace _25._2.cs;

internal class Program
{
    const int MaxPowerTen = 19;
    static readonly ulong[] PowTen = new ulong[MaxPowerTen];
    static Program()
    {
        ulong num = 1;
        for (int i = 0; i < MaxPowerTen; i++, num *= 10)
            PowTen[i] = num;
    }
    static void Main(string[] args)
    {
        Console.WriteLine("Hello, World! Day 2 here.\nPaste input: ");

        var line = Console.ReadLine();
        if (line == null) return;

        ulong agg = 0;
        foreach (var range in line.Split(','))
        {
            var leftright = range.Split('-');
            if (ulong.TryParse(leftright[0], out ulong left)
                && ulong.TryParse(leftright[1], out ulong right))
            {
                int lo = OrderMag(left), ro = OrderMag(right);
                if (lo == ro && (lo % 2) != 0) // odd numbered orders won't do anything.
                    continue;

                ulong upperHalf;
                (upperHalf, _) = Split(left);
                ulong upperHalfRight;
                (upperHalfRight, _) = Split(right);
                // auto-graduate to the next order if need be.
                if ((lo % 2) != 0 && ro > lo)
                    upperHalf = PowTen[lo++/2]; // next power of 10 up.
                else if (ro > lo && (ro % 2) != 0) // demote right
                    upperHalfRight = PowTen[--ro/2] - 1;

                for (; upperHalf <= upperHalfRight; upperHalf++)
                {
                    // dupe the sequence...
                    ulong dupe = PowTen[lo / 2] * upperHalf + upperHalf;
                    if (dupe >= left && dupe <= right)
                        agg += dupe;
                }
            }
            // else???
        }

        Console.WriteLine($"I aggregated {agg}");
    }

    static int OrderMag(ulong n)
    {
        int order;
        for (order = 0; order < MaxPowerTen; order++)
            if (PowTen[order] > n)
                break;
        return order;
    }

    static (ulong, ulong) Split(ulong n)
    {
        int order = OrderMag(n);
        // get right half, round up
        ulong splitter = PowTen[(order + 1) / 2];
        ulong right = n % splitter;
        ulong left = (n - right) / splitter;
        return (left, right);
    }
}
