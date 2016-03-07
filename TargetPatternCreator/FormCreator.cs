using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using TargetPatternCreator.Classes;
using TargetPatternCreator.Classes.Polygon;

namespace TargetPatternCreator
{
    /// <summary>
    /// GUI for creating and exporting a target pattern for Cellular Pattern Evolution
    /// </summary>
    public sealed partial class FrmCreator : Form
    {

        #region Private fields

        private float pixelDelta;
        private const int CanvasSize = 600;

        private Polygon currentPolygon;
        private readonly List<Polygon> polygons = new List<Polygon>(); 
        private readonly ColorGrid colorGrid = new ColorGrid();

        private Edge currentEdge;
        private Point mousePoint;
        private Point? edgeFirstPoint;

        #endregion

        #region Constructor

        public FrmCreator()
        {
            InitializeComponent();

            NumSize.Value = 15;
        }

        #endregion

        #region OnPaint

        protected override void OnPaint(PaintEventArgs e)
        {
            var g = e.Graphics;
            g.SmoothingMode = SmoothingMode.AntiAlias;
            g.InterpolationMode = InterpolationMode.HighQualityBicubic;
            g.PixelOffsetMode = PixelOffsetMode.Half;

            colorGrid.Reset();
            polygons.ForEach(x => x.Draw(colorGrid));
            if (currentPolygon != null) currentPolygon.Draw(colorGrid);
            if (currentEdge != null) currentEdge.Draw(colorGrid);

            for (var y = 0; y < NumSize.Value; ++y)
            {
                for (var x = 0; x < NumSize.Value; ++x)
                {
                    g.FillRectangle(
                        new SolidBrush(colorGrid[x, y]),
                        new Rectangle(Convert.ToInt32(x * pixelDelta), Convert.ToInt32(y * pixelDelta),
                            Convert.ToInt32(pixelDelta), Convert.ToInt32(pixelDelta)));
                }
            }

            g.DrawRectangle(Pens.Black, 0, 0, CanvasSize - 1, CanvasSize - 1);
            for (var i = 0; i <= NumSize.Value + 1; i++)
            {
                var pos = Convert.ToInt32(i * pixelDelta);
                g.DrawLine(Pens.Black, 0, pos, CanvasSize, pos);
                g.DrawLine(Pens.Black, pos, 0, pos, CanvasSize);
            }

            g.FillEllipse(Brushes.Red, pixelDelta * mousePoint.X, pixelDelta * mousePoint.Y, pixelDelta, pixelDelta);
        }

        #endregion

        #region Methods

        private void Reset()
        {
            colorGrid.Reset();
            polygons.Clear();
            currentEdge = null;
            currentPolygon = null;
            edgeFirstPoint = null;

            Invalidate();
        }

        private string TargetToString()
        {
            var result = new StringBuilder();

            var bcgCol = colorGrid.BackgroundColor;
            result.AppendLine(string.Format(
                "<svg height=\"{0}\" width=\"{0}\" fill=\"rgb({1},{2},{3})\">",
                NumSize.Value, bcgCol.R, bcgCol.G, bcgCol.B));
            foreach (var poly in polygons)
            {
                result.Append("<polygon points=\"");
                foreach (var edge in poly.Edges)
                {
                    result.Append(string.Format("{0} {1},", edge.Start.X, edge.Start.Y));
                }
                var first = poly.Edges.First().Start;
                result.Append(string.Format("{0} {1}", first.X, first.Y));
                result.Append("\" fill=\"");
                result.AppendLine(string.Format("rgb({0},{1},{2})\" />", poly.Color.R, poly.Color.G, poly.Color.B));
            }
            result.AppendLine("</svg>");
            return result.ToString();
        }

        private void Export()
        {
            SaveDialog.FileName = "TargetPattern.svg";
            SaveDialog.Filter = @"SVG file (*.svg)|*.svg";
            if (SaveDialog.ShowDialog() != DialogResult.OK) return;
            using (var writer = new StreamWriter(SaveDialog.FileName))
            {
                writer.Write(TargetToString());
            }
        }

        #endregion

        #region Control events

        private void NumSize_ValueChanged(object sender, EventArgs e)
        {
            colorGrid.Resize(Convert.ToInt32(NumSize.Value));
            pixelDelta = CanvasSize/(float) NumSize.Value;
            Invalidate();
        }

        #region Mouse

        protected override void OnMouseClick(MouseEventArgs e)
        {
            var x = Convert.ToInt32(Math.Floor(e.X/pixelDelta));
            var y = Convert.ToInt32(Math.Floor(e.Y/pixelDelta));
            var clickPoint = new Point(x, y);

            mousePoint = clickPoint;
            edgeFirstPoint = clickPoint;

            if (currentPolygon == null)
                currentPolygon = new Polygon(clickPoint, PanelColor.BackColor);
            else
            {
                currentPolygon.TryAddVertex(clickPoint);
                if (currentPolygon.Closed)
                {
                    polygons.Add(currentPolygon);
                    currentPolygon = null;
                    edgeFirstPoint = null;
                    currentEdge = null;
                }
            }

            Invalidate();
        }

        protected override void OnMouseMove(MouseEventArgs e)
        {
            var x = Convert.ToInt32(Math.Floor(e.X / pixelDelta));
            var y = Convert.ToInt32(Math.Floor(e.Y / pixelDelta));
            var clickPoint = new Point(x, y);

            if (edgeFirstPoint != null)
                currentEdge = new Edge(edgeFirstPoint.Value, clickPoint, 
                    Color.FromArgb(150, PanelColor.BackColor));

            mousePoint = clickPoint;

            Invalidate();
        }

        private void MouseWheelPanel(object sender, MouseEventArgs e)
        {
            if (e.Delta <= 0)
                NumSize.Value--;
            else 
                NumSize.Value++;
        }

        #endregion

        private void PanelColor_Click(object sender, EventArgs e)
        {
            if (ColorPicker.ShowDialog() == DialogResult.OK)
            {
                PanelColor.BackColor = ColorPicker.Color;
            }
        }

        private void PanelBackgroundColor_Click(object sender, EventArgs e)
        {
            if (BackgroundColorPicker.ShowDialog() == DialogResult.OK)
            {
                colorGrid.BackgroundColor = PanelBackgroundColor.BackColor = BackgroundColorPicker.Color;
                Invalidate();
            }
        }

        private void ButReset_Click(object sender, EventArgs e)
        {
            Reset();
        }

        private void ButExport_Click(object sender, EventArgs e)
        {
            Export();
        }

        #endregion

    }
}
