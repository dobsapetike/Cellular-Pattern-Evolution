using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;

namespace TargetPatternCreator.Classes.ShapeRasterizer
{
    /// <summary>
    /// Abstract class for rasterizing a 2D shape parametrizable with 2 points
    /// </summary>
    public abstract class ShapeRasterizer
    {
        /// <summary>
        /// Returns all the points occupied by the shape represented with points p1 and p2
        /// </summary>
        protected abstract IEnumerable<Point> ComputePoints(Point p1, Point p2, bool fill);
        /// <summary>
        /// Draws the implicit version of the shape represented with points p1 and p2 into 
        /// the given graphics instance
        /// </summary>
        protected abstract void DrawImplicit(Graphics g, Point p1, Point p2, float pixelSize);

        /// <summary>
        /// Draws the object represented with points p1 and p2 into the given Graphics instance
        /// </summary>
        public void DrawGraphics(Graphics g, Point p1, Point p2, Color color, float pixelSize, bool fill)
        {
            var points = ComputePoints(p1, p2, fill);
            foreach (var p in points)
            {
                g.FillRectangle(
                        new SolidBrush(Color.FromArgb(150, color)),
                        new Rectangle(Convert.ToInt32(p.X * pixelSize), Convert.ToInt32(p.Y * pixelSize),
                            Convert.ToInt32(pixelSize), Convert.ToInt32(pixelSize)));
            }
            DrawImplicit(g, p1, p2, pixelSize);
        }

        /// <summary>
        /// Draws the object represented with points p1 and p2 into the given ColorGrid
        /// </summary>
        public void DrawGraphics(ColorGrid grid, Point p1, Point p2, Color color, bool fill)
        {
            var points = ComputePoints(p1, p2, fill);
            foreach (var p in points
                .Where(p => p.X >= 0 && p.Y >= 0 && p.X < grid.Size && p.Y < grid.Size))
            {
                grid[p.X, p.Y] = color;
            }
        }
    }
}
