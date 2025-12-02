using System.Diagnostics.CodeAnalysis;

namespace _25._2.cs;

internal class Program
{
    const int MaxPowerTen = 19;
    static readonly ulong[] PowTen = new ulong[MaxPowerTen];
    static Program()
    {
        ulong num = 1;
        for (int i = 0; i < MaxPowerTen; i++, num *= 10)
        {
            PowTen[i] = num;
        }
    }
    static void Main(string[] args)
    {
        Console.WriteLine("Hello, World! Day 2 here.\nPaste input: ");

        var line = Console.ReadLine();
        if (line == null) return;

        ulong agg = 0, agg2 = 0;
        HashSet<ulong> set = new();
        foreach (var range in line.Split(','))
        {
            var leftright = range.Split('-');
            if (ulong.TryParse(leftright[0], out ulong left)
                && ulong.TryParse(leftright[1], out ulong right))
            {
                int lo = OrderMag(left), ro = OrderMag(right);

                // COMMENTED: PART 1 SOLUTION ONLY. Part 2 solution supercedes this.
                //if (lo == ro && (lo % 2) != 0) // odd numbered orders won't do anything - for PART 1 - comment for part 2.
                //    continue;

                //ulong upperHalf;
                //(upperHalf, _) = Split(left);
                //ulong upperHalfRight;
                //(upperHalfRight, _) = Split(right);
                //// auto-graduate to the next order if need be.
                //if ((lo % 2) != 0 && ro > lo)
                //    upperHalf = PowTen[lo++/2]; // next power of 10 up.
                //else if (ro > lo && (ro % 2) != 0) // demote right
                //    upperHalfRight = PowTen[(ro - 1)/2] - 1;

                //for (; upperHalf <= upperHalfRight; upperHalf++)
                //{
                //    // dupe the sequence...
                //    ulong dupe = PowTen[lo / 2] * upperHalf + upperHalf;
                //    if (dupe >= left && dupe <= right)
                //        agg += dupe;
                //}

                // part 2 solution
                for (int seqLength = 1; seqLength <= (ro / 2); seqLength++)
                {
                    // consider the orders... could a sequence of this length ever work? 
                    // there must be a target of order that is evenly divisible by seqLength.
                    // but don't just check lo and ro, check in between too.
                    {
                        bool ok = false;
                        for (int order = lo; !ok && order <= ro; order++)
                            if (0 == (order % seqLength))
                                ok = true;
                        if (!ok)
                            continue;
                    }

                    for (int numRpt = Math.Max(2, (lo + seqLength - 1) / seqLength), numHi = (ro + seqLength - 1) / seqLength;
                        numRpt <= numHi && numRpt * seqLength <= ro; numRpt++)
                    {
                        // build an increment like 001 001 001 
                        ulong increment = 1;
                        for (int numOnes = 1; numOnes < numRpt; numOnes++)
                            increment = increment * PowTen[seqLength] + 1;

                        // auto-graduate the num to avoid leading zero.
                        // for example, 001001 ain't it. Good increment, but start at 100100. 
                        ulong num = increment * PowTen[seqLength - 1];
                        ulong numLoops = PowTen[seqLength] - PowTen[seqLength - 1]; // get 1000 - 100 = 900

                        //int orderNum = OrderMag(num);
                        //if (orderNum < lo || orderNum > ro)
                        //    continue; // anti-bug, not what's happening.

                        if (num < left)
                        {
                            var skip = (left - num + increment - 1) / increment;
                            num += skip * increment;
                            numLoops -= skip;
                        }

                        for (; num <= right && numLoops > 0; num += increment, numLoops--)
                        {
                            //if (OrderMag(num) != orderNum)
                            //    break; // anti-bug, not what's happening.
                            if (num >= left && !set.Contains(num))
                            {
                                Console.WriteLine($"{num}");
                                if (0 == (numRpt % 2))
                                    agg += num; // solution 1 by itself
                                else 
                                    agg2 += num; // solution 2 additionally
                                set.Add(num);
                            }
                        }
                    }
                }
                set.Clear();
            }
            else break; // anti-bug, not what's happening.
        }

        Console.WriteLine($"Part 1: aggregated {agg}");
        Console.WriteLine($"Part 2: aggregated {agg + agg2}");
    }

    // dataset: Part 1 22062284697, Part 2 46666175279 got it.
    // 26803-38596,161-351,37-56,9945663-10044587,350019-413817,5252508299-5252534634,38145069-38162596,1747127-1881019,609816-640411,207466-230638,18904-25781,131637-190261,438347308-438525264,5124157617-5124298820,68670991-68710448,8282798062-8282867198,2942-5251,659813-676399,57-99,5857600742-5857691960,9898925025-9899040061,745821-835116,2056-2782,686588904-686792094,5487438-5622255,325224-347154,352-630,244657-315699,459409-500499,639-918,78943-106934,3260856-3442902,3-20,887467-1022885,975-1863,5897-13354,43667065-43786338

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
