using System;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.IO;
using System.Text;
using System.Windows.Forms;
using TargetPatternCreator.Classes;
using TargetPatternCreator.Classes.ShapeRasterizer;

namespace TargetPatternCreator
{
    /// <summary>
    /// GUI for creating and exporting a target pattern for Cellular Pattern Evolution
    /// </summary>
    public sealed partial class FrmCreator : Form
    {

        #region Private fields

        private float _pixelDelta;
        private const int CanvasSize = 600;

        private ShapeRasterizer _currentRasterizer;
        private readonly LineRasterizer _lineRasterizer = new LineRasterizer();
        private readonly RectangleRasterizer _rectRasterizer = new RectangleRasterizer();
        private readonly EllipseRasterizer _ellipseRasrizer = new EllipseRasterizer();

        private readonly ColorGrid _colorGrid = new ColorGrid();

        private int _x0, _y0;
        private int _x1, _y1;

        #endregion

        #region Click state

        private OperationEnum _op = OperationEnum.FirstPoint;

        private enum OperationEnum
        {
            FirstPoint,
            SecondPoint,
        }

        #endregion

        #region Constructor

        public FrmCreator()
        {
            InitializeComponent();

            NumSize.Value = 15;
            _currentRasterizer = _lineRasterizer;
        }

        #endregion

        #region OnPaint

        protected override void OnPaint(PaintEventArgs e)
        {
            var g = e.Graphics;
            g.SmoothingMode = SmoothingMode.AntiAlias;
            g.InterpolationMode = InterpolationMode.HighQualityBicubic;
            g.PixelOffsetMode = PixelOffsetMode.Half;

            for (var y = 0; y < NumSize.Value; ++y)
            {
                for (var x = 0; x < NumSize.Value; ++x)
                {
                    g.FillRectangle(
                        new SolidBrush(_colorGrid[x, y]),
                        new Rectangle(Convert.ToInt32(x*_pixelDelta), Convert.ToInt32(y*_pixelDelta),
                            Convert.ToInt32(_pixelDelta), Convert.ToInt32(_pixelDelta)));
                }
            }

            if (_op == OperationEnum.SecondPoint)
            {
                _currentRasterizer.DrawGraphics(g, new Point(_x0, _y0), new Point(_x1, _y1),
                    ColorPicker.Color, _pixelDelta, CBoxFill.Checked);
            }

            g.DrawRectangle(Pens.Black, 0, 0, CanvasSize - 1, CanvasSize - 1);
            for (var i = 0; i <= NumSize.Value + 1; i++)
            {
                var pos = Convert.ToInt32(i*_pixelDelta);
                g.DrawLine(Pens.Black, 0, pos, CanvasSize, pos);
                g.DrawLine(Pens.Black, pos, 0, pos, CanvasSize);
            }

            g.FillEllipse(Brushes.Red, _pixelDelta*_x0, _pixelDelta*_y0, _pixelDelta, _pixelDelta);
            if (_op == OperationEnum.SecondPoint)
            {
                g.FillEllipse(Brushes.Green, _pixelDelta*_x1, _pixelDelta*_y1, _pixelDelta, _pixelDelta);
            }
        }

        #endregion

        #region Methods

        private void Reset()
        {
            _colorGrid.Reset();
            Invalidate();
        }

        private string TargetToString()
        {
            var result = new StringBuilder();
            result.AppendLine(string.Format("{0} {0}", NumSize.Value));
            for (var y = 0; y < _colorGrid.Size; ++y)
            {
                for (var x = 0; x < _colorGrid.Size; ++x)
                {
                    var c = _colorGrid[x, y];
                    result.Append(string.Format("({0},{1},{2})", c.R, c.G, c.B));
                    if (x < _colorGrid.Size - 1)
                        result.Append(";");
                }
                if (y < _colorGrid.Size - 1)
                    result.AppendLine();
            }
            return result.ToString();
        }

        private void Export()
        {
            SaveDialog.FileName = "TargetPattern.target";
            SaveDialog.Filter = @"Target file (*.target)|*.target";
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
            _colorGrid.Resize(Convert.ToInt32(NumSize.Value));
            _pixelDelta = CanvasSize/(float) NumSize.Value;
            Invalidate();
        }

        #region Mouse

        protected override void OnMouseClick(MouseEventArgs e)
        {
            var x = Convert.ToInt32(Math.Floor(e.X/_pixelDelta));
            var y = Convert.ToInt32(Math.Floor(e.Y/_pixelDelta));

            if ((e.Button & MouseButtons.Left) == MouseButtons.Left)
            {
                switch (_op)
                {
                    case OperationEnum.FirstPoint:
                        _x0 = x;
                        _y0 = y;
                        _op = OperationEnum.SecondPoint;
                        break;
                    case OperationEnum.SecondPoint:
                        _currentRasterizer.DrawGraphics(_colorGrid,
                            new Point(_x0, _y0), new Point(_x1, _y1),
                            ColorPicker.Color, CBoxFill.Checked);

                        _x1 = x;
                        _y1 = y;
                        _op = OperationEnum.FirstPoint;
                        break;
                }
            }
            else if ((e.Button & MouseButtons.Right) == MouseButtons.Right)
            {
                _colorGrid.FloodFill(x, y, ColorPicker.Color);
            }

            Invalidate();
        }

        protected override void OnMouseMove(MouseEventArgs e)
        {
            var x = Convert.ToInt32(Math.Floor(e.X/_pixelDelta));
            var y = Convert.ToInt32(Math.Floor(e.Y/_pixelDelta));

            if (_op == OperationEnum.SecondPoint)
            {
                _x1 = x;
                _y1 = y;
            }
            else if (_op == OperationEnum.FirstPoint)
            {
                _x0 = x;
                _y0 = y;
            }

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

        #region Shape radiobutton

        private void RdbLine_CheckedChanged(object sender, EventArgs e)
        {
            _currentRasterizer = _lineRasterizer;
        }

        private void RdbRectangle_CheckedChanged(object sender, EventArgs e)
        {
            _currentRasterizer = _rectRasterizer;
        }

        private void RdbEllipse_CheckedChanged(object sender, EventArgs e)
        {
            _currentRasterizer = _ellipseRasrizer;
        }

        #endregion

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
