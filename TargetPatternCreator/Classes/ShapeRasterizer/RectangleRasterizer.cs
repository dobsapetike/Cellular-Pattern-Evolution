using System;
using System.Collections.Generic;
using System.Drawing;

namespace TargetPatternCreator.Classes.ShapeRasterizer
{
    public class RectangleRasterizer : ShapeRasterizer
    {
        protected override IEnumerable<Point> ComputePoints(Point p1, Point p2, bool fill)
        {
            var result = new List<Point>();

            int minX = Math.Min(p1.X, p2.X), maxX = Math.Max(p1.X, p2.X);
            int minY = Math.Min(p1.Y, p2.Y), maxY = Math.Max(p1.Y, p2.Y);
            for (var y = minY; y <= maxY; ++y)
            {
                result.Add(new Point(minX, y));
                if (fill || y == minY || y == maxY)
                {
                    for (var x = minX + 1; x <= maxX - 1; ++x)
                    {
                        result.Add(new Point(x, y));
                    }
                }
                result.Add(new Point(maxX, y));
            }

            return result;
        }

        protected override void DrawImplicit(Graphics g, Point p1, Point p2, float pixelSize)
        {
            g.DrawRectangle(Pens.Red, pixelSize * Math.Min(p1.X, p2.X) + pixelSize/2,
                pixelSize * Math.Min(p1.Y, p2.Y) + pixelSize/2,
                Math.Abs(p1.X - p2.X) * pixelSize, Math.Abs(p1.Y - p2.Y) * pixelSize);
        }
    }
}
