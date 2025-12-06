namespace _25._6.cs;

using Num = UInt128;
internal class Program
{
    static readonly Num[] PowTen; // starts with 10, index is 1-1 based
    static Program()
    {
        List<Num> powTenList = [];
        for (Num num = 10, last = 1; num / 10 >= last; num *= 10)
        {
            powTenList.Add(last = num);
        }
        PowTen = powTenList.ToArray();

        // test 
        //Num testnum = 6;
        //for (int numDigits = 1; numDigits <= PowTen.Length; numDigits++, testnum *= 10)
        //{
        //    int order = NumDigits(testnum);
        //    if (numDigits != order)
        //        throw new();
        //}
    }
    static int NumDigits(Num num)
    {
        int exp;
        for (exp = 0; exp < PowTen.Length; exp++)
        {
            if (PowTen[exp] > num)
                break;
        }
        return 1 + exp;
    }
    static void Main(string[] args)
    {
        Console.WriteLine("Hello, World! Day 6 here.\nPaste Input: ");

        List<Num[]> input = [];
        List<Operator> ops = [];
        List<Num> thisLine = [];
        List<string> lines = [];
        string lineOps = string.Empty;
        for (string? line; !string.IsNullOrWhiteSpace(line = Console.ReadLine()); )
        {
            var list = line.Split(' ', StringSplitOptions.RemoveEmptyEntries | StringSplitOptions.TrimEntries)
                .ToList();

            // use this loop instead of linq in case it doesn't work
            // which it won't for the last line
            thisLine.Clear();
            for (int i = 0; i < list.Count && Num.TryParse(list[i], out Num num); i++)
                thisLine.Add(num);

            if (thisLine.Count > 0)
            {
                input.Add(thisLine.ToArray());
                lines.Add(line);
                continue; // the general case, it's numbers
            }

            for (int i = 0; i < list.Count && OperatorTryParse(list[i], out Operator o); i++)
                ops.Add(o);
            lineOps = line;
            break; // that's all folks
        }

        int numCols = ops.Count;
        Num part1 = 0, agg;
        for (int col = 0; col < numCols; col++)
        {
            var o = ops[col];
            agg = input[0][col];
            for (int row = 1; row < input.Count; row++)
                agg = Do(agg, input[row][col], o);
            part1 += agg;
        }

        Console.WriteLine($"Part 1 sum: {part1}");

        // for part 2, remake the "input" map according to the RTL columns
        // this can be done then recalculated per column singly.
        // for each column, what's the max num of decimal digits?
        Num part2 = 0;
        numCols = lines[0].Length;

        //int ixOp;
        var oper = (Operator)'\0'; // yeah yeah. we'll load in the first one.
        agg = 0;

        // step across every char
        for (int col = 0; col < numCols; col++)
        {
            Operator nextOp = (Operator)lineOps[col];
            if ((char)nextOp != ' ')// && oper != nextOp)
            {
                // add up what's collected so far.
                part2 += agg;

                oper = nextOp;
                agg = Identity(oper); // additive / multiplicative identity
            }

            Num number = 0; // construct a new number for each digitIndex in this column.
            bool foundAnyDigit = false;
            for (int row = 0; row < lines.Count; row++)
            {
                char c = lines[row][col];
                if (c < '0' || c > '9')
                    continue; // no place value for this guy. Don't touch the number with leading or trailing zeros.

                foundAnyDigit = true; // nice.
                Num digit = (Num)(c - '0');

                number *= 10;
                number += digit;
            }
            if (foundAnyDigit)
                // now accumulate for this column
                agg = Do(agg, number, oper);
        }
        part2 += agg;

        Console.WriteLine($"Part 2 sum: {part2}");
    }
    enum Operator
    {
        Add = '+', Mul = '*'
    }
    static bool OperatorTryParse(string value, out Operator op)
    {
        op = (Operator)value.Trim().FirstOrDefault('\0');
        return op switch
        {
            Operator.Add or Operator.Mul => true,
            _ => false
        };
    }
    static Num Identity(Operator op)
    {
        return op switch
        {
            Operator.Add => 0,
            Operator.Mul => 1,
            _ => throw new()
        };
    }
    static Num Do(Num left, Num right, Operator op)
    {
        return op switch
        {
            Operator.Add => left + right,
            Operator.Mul => left * right,
            _ => throw new()
        };
    }
}
