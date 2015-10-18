using System;
using System.Collections.Generic;
using System.Drawing;

namespace TargetPatternCreator.Classes.ShapeRasterizer
{
    public class LineRasterizer : ShapeRasterizer
    {
        protected override IEnumerable<Point> ComputePoints(Point p1, Point p2, bool fill)
        {
            var result = new List<Point>();

            // Bresenham's line algorithm
            var dx = p2.X - p1.X;
            var dy = p2.Y - p1.Y;
            result.Add(new Point(p1.X, p1.Y));

            if (Math.Abs(dx) >= Math.Abs(dy))
            {
                int x = p1.X, xinc = dx > 0 ? 1 : -1;
                float y = p1.Y, yinc = (float)dy / dx * xinc;
                while (x != p2.X)
                {
                    x += xinc;
                    y += yinc;
                    result.Add(new Point(x, (int)Math.Round(y)));
                }
            }
            else
            {
                int y = p1.Y, yinc = dy > 0 ? 1 : -1;
                double x = p1.X, xinc = (double)dx / dy * yinc;
                while (y != p2.Y)
                {
                    x += xinc;
                    y += yinc;
                    result.Add(new Point((int)Math.Round(x), y));
                }
            }

            return result;
        }

        protected override void DrawImplicit(Graphics g, Point p1, Point p2, float pixelSize)
        {
            g.DrawLine(Pens.Red, pixelSize * p1.X + pixelSize / 2, pixelSize * p1.Y + pixelSize / 2,
                pixelSize * p2.X + pixelSize / 2, pixelSize * p2.Y + pixelSize / 2);
        }
    }
}
