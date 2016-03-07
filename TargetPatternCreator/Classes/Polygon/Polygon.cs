using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;

namespace TargetPatternCreator.Classes.Polygon
{
    public class Polygon
    {
        public bool Closed { get; private set; }
        public Color Color { get; private set; }
        private readonly List<Edge> edges = new List<Edge>();
        private Point prevPoint;
        private readonly Point firstPoint;

        private int ymin = int.MaxValue, ymax = int.MinValue;

        public Polygon(Point first, Color c)
        {
            Color = c;
            firstPoint = prevPoint = first;
        }

        public IReadOnlyCollection<Edge> Edges 
        {
            get
            {
                return edges.AsReadOnly();
            }
        }

        public void TryAddVertex(Point point)
        {
            if (Closed) return;

            edges.Add(new Edge(prevPoint, point, Color));
            prevPoint = point;
            if (point == firstPoint) 
                Closed = true;

            var y = Convert.ToInt32(point.Y);
            ymin = Math.Min(y, ymin);
            ymax = Math.Max(y, ymax);
        }

        private void ScanFill(ColorGrid grid)
        {
            if (!Closed) return;

            var edgeTable = new Dictionary<int, List<Edge>>();
            var xminTable = new Dictionary<Edge, double>();
            edges.ForEach(x => xminTable.Add(x, x.XMin));

            foreach (var edge in edges.Where(edge => !edge.IsHorizontal))
            {
                if (!edgeTable.ContainsKey(edge.YMax))
                {
                    edgeTable.Add(edge.YMax, new List<Edge>());
                }
                edgeTable[edge.YMax].Add(edge);
            }

            var activeEdgeTable = new List<Edge>();
            for (var y = ymax; y >= ymin; --y)
            {
                if (edgeTable.ContainsKey(y))
                {
                    activeEdgeTable.AddRange(edgeTable[y]);
                }
                activeEdgeTable.Sort((a, b) => xminTable[a].CompareTo(xminTable[b]));

                Edge temp = null;
                foreach (var e in activeEdgeTable)
                {
                    if (temp == null)
                    {
                        temp = e;
                        continue;
                    }
                    for (var x = Convert.ToInt32(xminTable[temp]); x <= xminTable[e]; ++x)
                        grid[x, y] = Color;
                    temp = null;
                }

                var toRem = new List<Edge>();
                foreach (var e in activeEdgeTable)
                {
                    if (e.YMin + 2 > y) toRem.Add(e);
                    else if (Math.Abs(e.Slope) > 0.0d) xminTable[e] -= 1 / e.Slope;
                }
                activeEdgeTable.RemoveAll(e => toRem.Contains(e));
            }
        }

        public void Draw(ColorGrid grid)
        {
            foreach (var e in edges)
                e.Draw(grid);
            if (Closed) ScanFill(grid);
        }
    }
}
