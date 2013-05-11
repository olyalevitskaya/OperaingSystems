using System;

namespace Life
{
    public class Field
    {
        public Field(int size)
        {
            var cells = new Cell[size,size];
            for (int i = 0; i < size; i++)
                for (int j = 0; j < size; j++)
                    cells[i, j] = new Cell(i, j);

            for (int i = 0; i < size; i++)
            {
                for (int j = 0; j < size; j++)
                    Console.Out.Write("{0}", cells[i, j].IsAlive ? 1 : 0);
                Console.Out.WriteLine();
            }
        }
    }
}