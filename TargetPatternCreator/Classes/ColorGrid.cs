using System;
using System.Collections.Generic;
using System.Drawing;

namespace TargetPatternCreator.Classes
{
    /// <summary>
    /// Structure representing a 2D array of RGB colored pixels
    /// </summary>
    public class ColorGrid
    {
        private Color[,] colorGrid;

        public Color BackgroundColor { get; set; }

        public ColorGrid()
        {
            BackgroundColor = Color.White;
        }

        /// <summary>
        /// Resizes the array, keeping the values with valid indices
        /// </summary>
        /// <param name="size"> length of the array in both directions </param>
        public void Resize(int size)
        {
            var newGrid = new Color[size, size];
            for (var y = 0; y < size; ++y)
            {
                for (var x = 0; x < size; ++x)
                {
                    newGrid[y, x] =
                        colorGrid != null && colorGrid.GetLength(0) > y && colorGrid.GetLength(0) > x
                            ? colorGrid[y, x]
                            : BackgroundColor;
                }
            }
            colorGrid = newGrid;
        }

        /// <summary>
        /// Reset the structure with the default value
        /// </summary>
        public void Reset()
        {
            for (var y = 0; y < Size; ++y)
            {
                for (var x = 0; x < Size; ++x)
                {
                    this[y, x] = BackgroundColor;
                }
            }
        }

        /// <summary>
        /// Recolors a continous block of eighbouring pixels with same color 
        /// </summary>
        /// <param name="x"> x coord of the starting position </param>
        /// <param name="y"> y coord of the starting position </param>
        /// <param name="c"> new color </param>
        public void FloodFill(int x, int y, Color c)
        {
            if (this[x, y] == c) return;

            // function returning the orthogonal neighbours of a point
            Func<Point, List<Point>> getNeighbours = p =>
            {
                var res = new List<Point>();
                if (p.X > 0) res.Add(new Point(p.X - 1, p.Y));
                if (p.X < Size - 1) res.Add(new Point(p.X + 1, p.Y));
                if (p.Y > 0) res.Add(new Point(p.X, p.Y - 1));
                if (p.Y < Size - 1) res.Add(new Point(p.X, p.Y + 1));
                return res;
            };

            // traverse the block using breadth-first search
            var startColor = this[x, y];
            var queue = new Queue<Point>();

            queue.Enqueue(new Point(x, y));
            while (queue.Count > 0)
            {
                var p = queue.Dequeue();
                // stop on already processed pixels and pixels with different colors 
                if (this[p.X, p.Y] != startColor
                    || this[p.X, p.Y] == c) continue;

                this[p.X, p.Y] = c;
                foreach (var neighbour in getNeighbours(p))
                {
                    queue.Enqueue(neighbour);
                }
            }
        }

        /// <summary>
        /// Size of the array
        /// </summary>
        public int Size
        {
            get { return colorGrid.GetLength(0); }
        }

        /// <summary>
        /// Getter and setter of the array cell color on given position
        /// </summary>
        public Color this[int x, int y]
        {
            get { return colorGrid[y, x]; }
            set { if (x < Size && y < Size && x >= 0 && y >= 0) colorGrid[y, x] = value; }
        }
    }
}
