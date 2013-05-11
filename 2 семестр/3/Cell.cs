using System;

namespace Life
{
    public class Cell
    {
        private static readonly Random Random = new Random();

        public Cell(int x, int y)
        {
            X = x;
            Y = y;
            IsAlive = Random.Next()%2 == 1;
        }

        public int X { get; private set; }
        public int Y { get; private set; }
        public bool IsAlive { get; set; }
    }
}