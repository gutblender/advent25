namespace _25._1.cs
{
    internal class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Hello, World! Advent '25 Day 1 here.\nEnter input:");

            int where, cntZero = 0, cntCrossings = 0;
            const int NumPositions = 100;
            for (where = 50; ;)
            {
                string? line = Console.ReadLine();
                if (!string.IsNullOrWhiteSpace(line) && int.TryParse(line[1..], out int turn))
                {
                    bool left = (line[0] == 'L');
                    int distanceToCrossing = left ? where : (NumPositions - where);

                    // all other crossings, for big numbers.
                    cntCrossings += turn / NumPositions;
                    turn %= NumPositions;

                    if (distanceToCrossing > 0 && // bug: double-counting when starting at 0.
                        turn >= distanceToCrossing)
                    { // do the first crossing and get that out of the way.
                        where = 0;
                        turn -= distanceToCrossing;
                        cntCrossings++;
                    }

                    if (left) turn = -turn;
                    where = (where + turn + NumPositions) % NumPositions; // renorm.

                    if (0 == where) // only if ending up at 0 will we count zeros.
                        cntZero++;
                }
                else break;
            }
            
            Console.WriteLine($"Number of times ending at position 0: {cntZero}");
            Console.WriteLine($"Number of times crossing position 0: {cntCrossings}");
        }
    }
}
