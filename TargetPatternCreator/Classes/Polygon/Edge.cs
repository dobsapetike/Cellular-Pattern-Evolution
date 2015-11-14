using System;
using System.Drawing;

namespace TargetPatternCreator.Classes.Polygon
{
    public class Edge
    {
        public int YMin { get; private set; }
        public int YMax { get; private set; }
        public double XMin { get; private set; }
        public Color Color { get; private set; }
        public bool IsHorizontal { get; private set; }
        public double Slope { get; private set; }

        public Point Start { get; private set; }
        public Point End { get; private set; }

        public Edge(Point p1, Point p2, Color c)
        {
            Start = p1; End = p2;
            Color = c;
            YMax = Convert.ToInt32(Math.Max(Start.Y, End.Y));
            YMin = Convert.ToInt32(Math.Min(Start.Y, End.Y));
            XMin = Start.Y > End.Y ? Start.X : End.X;
            IsHorizontal = Convert.ToInt32(Start.Y - End.Y) == 0;
            Slope = Convert.ToInt32(End.X - Start.X) == 0
                ? 0d
                : (double)(End.Y - Start.Y) / (End.X - Start.X);
        }

        public void Draw(ColorGrid grid)
        {
            var dx = End.X - Start.X;
            var dy = End.Y - Start.Y;
            grid[Start.X, Start.Y] = Color;
            if (Math.Abs(dx) >= Math.Abs(dy))
            {
                int x = Start.X, xinc = dx > 0 ? 1 : -1;
                float y = Start.Y, yinc = (float)dy / dx * xinc;
                while (x != End.X)
                {
                    x += xinc;
                    y += yinc;
                    grid[x, (int)Math.Round(y)] = Color;
                }
            }
            else
            {
                int y = Start.Y, yinc = dy > 0 ? 1 : -1;
                double x = Start.X, xinc = (double)dx / dy * yinc;
                while (y != End.Y)
                {
                    x += xinc;
                    y += yinc;
                    grid[(int)Math.Round(x), y] = Color;
                }
            }
        }
    }
}
