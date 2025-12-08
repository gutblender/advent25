using System.Numerics;

namespace _25._8.cs;

internal class Program
{
    //const int NumLinks = 1000;
    static void Main(string[] args)
    {
        Console.WriteLine("Hello, World! Day 8 here.\nPaste Input: ");

        List<Node> nodes = [];
        for (string? line; !string.IsNullOrEmpty(line = Console.ReadLine());)
        {
            if (line == null || (line.Split(',') is not string[] pieces)
                || !int.TryParse(pieces[0], out int x)
                || !int.TryParse(pieces[1], out int y)
                || !int.TryParse(pieces[2], out int z))
                break;
            nodes.Add(new(x, y, z));
        }
        List<Joiner> joiners = new((nodes.Count - 1) * (nodes.Count - 1));
        for (int i = 0; i < nodes.Count - 1; i++)
            for (int j = i + 1; j < nodes.Count; j++)
                joiners.Add( new(i, j, nodes[i].Distance(nodes[j])) );
        joiners.Sort((l, r) => l.Distance.CompareTo(r.Distance));

        int lastLeft = -1, lastRight = -1, n = 0;
        foreach(var join in joiners)
        {
            int left = join.Left;
            int right = join.Right;

            // first update circuit counts
            nodes[left].Join(nodes[right]);
            (lastLeft, lastRight) = (left, right);

            if (++n == nodes.Count)
            { // part 1 solution
                int product = 1;
                var circuits = new int[nodes.Count];
                HashSet<Node> counted = [];
                for (int c = 0, net = 0; c < nodes.Count; c++) // circuit id
                {
                    if (!counted.Contains(nodes[c]))
                    {
                        counted.UnionWith(nodes[c].Circuit);
                        circuits[net++] = nodes[c].Count;
                    }
                }
                foreach (var circuit in circuits.OrderDescending().Take(3))
                    product *= circuit;

                Console.WriteLine($"Part 1 solution: {product}");
            }

            if (nodes[left].Count >= nodes.Count)
                break; // they're all joined now.
        }

        ulong prod = (ulong)nodes[lastLeft].X * (ulong)nodes[lastRight].X;
        Console.WriteLine($"Part 2 solution: {prod}");
    }
}

sealed record Joiner(int l, int r, ulong dist) 
{
    public readonly int Left = l, Right = r; 
    public readonly ulong Distance = dist; 
}
class Node
{
    readonly public int X, Y, Z;

    HashSet<Node> DirectLinks = [];
    public HashSet<Node> Circuit { get; private set; }
    public bool IsLinked(Node node) => DirectLinks.Contains(node);
    public int Count => Circuit.Count;
    public Node(int x, int y, int z)
    {
        X = x;
        Y = y;
        Z = z;
        s_NextId++;
        Circuit = [];
        Circuit.Add(this);
    }

    public ulong Distance(Node other)
    { // dist squared
        ulong x = (ulong)Math.Abs(other.X - X);
        ulong y = (ulong)Math.Abs(other.Y - Y);
        ulong z = (ulong)Math.Abs(other.Z - Z);
        return x*x + y*y + z*z;
    }
    public void Join(Node other)
    {
        if (DirectLinks.Contains(other))
            return;

        // joined! do circuit ID and individual links
        if (!Circuit.Contains(other))
        {
            Circuit.UnionWith(other.Circuit);
            foreach (var node in Circuit)
                node.Circuit = Circuit; // collapse into my hashset
        }

        // process direct links
        this.DirectLinks.Add(other);
        other.DirectLinks.Add(this);
    }

    static int s_NextId = 0;
}
