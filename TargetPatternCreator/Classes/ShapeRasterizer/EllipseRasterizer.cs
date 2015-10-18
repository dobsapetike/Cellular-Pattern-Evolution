using System;
using System.Collections.Generic;
using System.Drawing;

namespace TargetPatternCreator.Classes.ShapeRasterizer
{
    public class EllipseRasterizer : ShapeRasterizer
    {
        private class Pair<T1, T2>
        {
            public T1 Item1 { get; set; }
            public T2 Item2 { get; set; }

            public Pair(T1 item1, T2 item2)
            {
                Item1 = item1;
                Item2 = item2;
            }
        }

        private void InsertRelativePoints(int cX, int cY, int x, int y, ICollection<Point> p)
        {
            p.Add(new Point(cX + x, cY + y));
            p.Add(new Point(cX - x, cY + y));
            p.Add(new Point(cX - x, cY - y));
            p.Add(new Point(cX + x, cY - y));
        }

        protected override IEnumerable<Point> ComputePoints(Point p1, Point p2, bool fill)
        {
            var result = new HashSet<Point>();

            // Bresenham's ellipse algorithm
            int cX = p1.X, cY = p1.Y;
            int dx = Math.Abs(p2.X - p1.X), dy = Math.Abs(p2.Y - p1.Y);

            if (dx == 0 && dy == 0)
            {
                result.Add(new Point(p1.X, p1.Y));
                return result;
            }

            int x = -dx, y = 0;
            int e2 = dy * dy, err = x * (2 * e2 + x) + e2;
            do
            {
                InsertRelativePoints(cX, cY, x, y, result);
                e2 = 2 * err;
                if (e2 >= (x * 2 + 1) * dy * dy)
                    err += (++x * 2 + 1) * dy * dy;
                if (e2 <= (y * 2 + 1) * dx * dx)
                    err += (++y * 2 + 1) * dx * dx;
            } while (x <= 0);

            if (fill)
            {
                // find out the border pixels on each row and fill the space between
                var pmap = new Dictionary<int, Pair<int, int>>();
                foreach (var p in result)
                {
                    if (!pmap.ContainsKey(p.Y))
                        pmap.Add(p.Y, new Pair<int, int>(int.MaxValue, int.MinValue));
                    pmap[p.Y].Item1 = Math.Min(pmap[p.Y].Item1, p.X);
                    pmap[p.Y].Item2 = Math.Max(pmap[p.Y].Item2, p.X);
                }
                foreach (var kv in pmap)
                {
                    for (var px = kv.Value.Item1 + 1; px < kv.Value.Item2; ++px)
                        result.Add(new Point(px, kv.Key));
                }
            }

            return result;
        }

        protected override void DrawImplicit(Graphics g, Point p1, Point p2, float pixelSize)
        {
            int dx = Math.Abs(p2.X - p1.X), dy = Math.Abs(p2.Y - p1.Y);
            g.DrawEllipse(Pens.Red, pixelSize * p1.X - dx * pixelSize + pixelSize / 2,
                pixelSize * p1.Y - dy * pixelSize + pixelSize / 2, dx * 2 * pixelSize, dy * 2 * pixelSize);
        }
    }
}
