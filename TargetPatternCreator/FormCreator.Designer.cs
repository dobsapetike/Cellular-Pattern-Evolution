using System;
using System.Windows.Forms;

namespace TargetPatternCreator
{
    sealed partial class FrmCreator
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.PanelControls = new System.Windows.Forms.Panel();
            this.ButExport = new System.Windows.Forms.Button();
            this.ButReset = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.GroupBoxShapes = new System.Windows.Forms.GroupBox();
            this.CBoxFill = new System.Windows.Forms.CheckBox();
            this.RdbEllipse = new System.Windows.Forms.RadioButton();
            this.RdbRectangle = new System.Windows.Forms.RadioButton();
            this.RdbLine = new System.Windows.Forms.RadioButton();
            this.PanelColor = new System.Windows.Forms.Panel();
            this.LabelColor = new System.Windows.Forms.Label();
            this.NumSize = new System.Windows.Forms.NumericUpDown();
            this.LblSize = new System.Windows.Forms.Label();
            this.ColorPicker = new System.Windows.Forms.ColorDialog();
            this.SaveDialog = new System.Windows.Forms.SaveFileDialog();
            this.PanelControls.SuspendLayout();
            this.GroupBoxShapes.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.NumSize)).BeginInit();
            this.SuspendLayout();
            // 
            // PanelControls
            // 
            this.PanelControls.BackColor = System.Drawing.SystemColors.Control;
            this.PanelControls.Controls.Add(this.ButExport);
            this.PanelControls.Controls.Add(this.ButReset);
            this.PanelControls.Controls.Add(this.label1);
            this.PanelControls.Controls.Add(this.GroupBoxShapes);
            this.PanelControls.Controls.Add(this.PanelColor);
            this.PanelControls.Controls.Add(this.LabelColor);
            this.PanelControls.Controls.Add(this.NumSize);
            this.PanelControls.Controls.Add(this.LblSize);
            this.PanelControls.Location = new System.Drawing.Point(600, 0);
            this.PanelControls.Name = "PanelControls";
            this.PanelControls.Size = new System.Drawing.Size(200, 600);
            this.PanelControls.TabIndex = 4;
            this.PanelControls.MouseWheel += MouseWheelPanel;
            // 
            // ButExport
            // 
            this.ButExport.Location = new System.Drawing.Point(60, 529);
            this.ButExport.Name = "ButExport";
            this.ButExport.Size = new System.Drawing.Size(90, 30);
            this.ButExport.TabIndex = 7;
            this.ButExport.Text = "Export";
            this.ButExport.UseVisualStyleBackColor = true;
            this.ButExport.Click += new System.EventHandler(this.ButExport_Click);
            // 
            // ButReset
            // 
            this.ButReset.Location = new System.Drawing.Point(60, 479);
            this.ButReset.Name = "ButReset";
            this.ButReset.Size = new System.Drawing.Size(90, 30);
            this.ButReset.TabIndex = 6;
            this.ButReset.Text = "Reset";
            this.ButReset.UseVisualStyleBackColor = true;
            this.ButReset.Click += new System.EventHandler(this.ButReset_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(14, 424);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(136, 17);
            this.label1.TabIndex = 5;
            this.label1.Text = "Right click to floodfill";
            // 
            // GroupBoxShapes
            // 
            this.GroupBoxShapes.Controls.Add(this.CBoxFill);
            this.GroupBoxShapes.Controls.Add(this.RdbEllipse);
            this.GroupBoxShapes.Controls.Add(this.RdbRectangle);
            this.GroupBoxShapes.Controls.Add(this.RdbLine);
            this.GroupBoxShapes.Location = new System.Drawing.Point(17, 193);
            this.GroupBoxShapes.Name = "GroupBoxShapes";
            this.GroupBoxShapes.Size = new System.Drawing.Size(172, 213);
            this.GroupBoxShapes.TabIndex = 4;
            this.GroupBoxShapes.TabStop = false;
            this.GroupBoxShapes.Text = "Shape";
            // 
            // CBoxFill
            // 
            this.CBoxFill.AutoSize = true;
            this.CBoxFill.Location = new System.Drawing.Point(73, 173);
            this.CBoxFill.Name = "CBoxFill";
            this.CBoxFill.Size = new System.Drawing.Size(47, 21);
            this.CBoxFill.TabIndex = 3;
            this.CBoxFill.Text = "Fill";
            this.CBoxFill.UseVisualStyleBackColor = true;
            // 
            // RdbEllipse
            // 
            this.RdbEllipse.AutoSize = true;
            this.RdbEllipse.Location = new System.Drawing.Point(27, 130);
            this.RdbEllipse.Name = "RdbEllipse";
            this.RdbEllipse.Size = new System.Drawing.Size(70, 21);
            this.RdbEllipse.TabIndex = 2;
            this.RdbEllipse.Text = "Ellipse";
            this.RdbEllipse.UseVisualStyleBackColor = true;
            this.RdbEllipse.CheckedChanged += new System.EventHandler(this.RdbEllipse_CheckedChanged);
            // 
            // RdbRectangle
            // 
            this.RdbRectangle.AutoSize = true;
            this.RdbRectangle.Location = new System.Drawing.Point(27, 84);
            this.RdbRectangle.Name = "RdbRectangle";
            this.RdbRectangle.Size = new System.Drawing.Size(93, 21);
            this.RdbRectangle.TabIndex = 1;
            this.RdbRectangle.Text = "Rectangle";
            this.RdbRectangle.UseVisualStyleBackColor = true;
            this.RdbRectangle.CheckedChanged += new System.EventHandler(this.RdbRectangle_CheckedChanged);
            // 
            // RdbLine
            // 
            this.RdbLine.AutoSize = true;
            this.RdbLine.Checked = true;
            this.RdbLine.Location = new System.Drawing.Point(27, 35);
            this.RdbLine.Name = "RdbLine";
            this.RdbLine.Size = new System.Drawing.Size(56, 21);
            this.RdbLine.TabIndex = 0;
            this.RdbLine.TabStop = true;
            this.RdbLine.Text = "Line";
            this.RdbLine.UseVisualStyleBackColor = true;
            this.RdbLine.CheckedChanged += new System.EventHandler(this.RdbLine_CheckedChanged);
            // 
            // PanelColor
            // 
            this.PanelColor.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.PanelColor.Location = new System.Drawing.Point(56, 107);
            this.PanelColor.Name = "PanelColor";
            this.PanelColor.Size = new System.Drawing.Size(98, 60);
            this.PanelColor.TabIndex = 3;
            this.PanelColor.Click += new System.EventHandler(this.PanelColor_Click);
            // 
            // LabelColor
            // 
            this.LabelColor.AutoSize = true;
            this.LabelColor.Location = new System.Drawing.Point(14, 76);
            this.LabelColor.Name = "LabelColor";
            this.LabelColor.Size = new System.Drawing.Size(45, 17);
            this.LabelColor.TabIndex = 2;
            this.LabelColor.Text = "Color:";
            // 
            // NumSize
            // 
            this.NumSize.Location = new System.Drawing.Point(17, 38);
            this.NumSize.Maximum = new decimal(new int[] {
            200,
            0,
            0,
            0});
            this.NumSize.Minimum = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.NumSize.Name = "NumSize";
            this.NumSize.Size = new System.Drawing.Size(120, 22);
            this.NumSize.TabIndex = 1;
            this.NumSize.Value = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.NumSize.ValueChanged += new System.EventHandler(this.NumSize_ValueChanged);
            // 
            // LblSize
            // 
            this.LblSize.AutoSize = true;
            this.LblSize.Location = new System.Drawing.Point(14, 9);
            this.LblSize.Name = "LblSize";
            this.LblSize.Size = new System.Drawing.Size(39, 17);
            this.LblSize.TabIndex = 0;
            this.LblSize.Text = "Size:";
            // 
            // FrmCreator
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoValidate = System.Windows.Forms.AutoValidate.EnableAllowFocusChange;
            this.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.ClientSize = new System.Drawing.Size(800, 600);
            this.Controls.Add(this.PanelControls);
            this.DoubleBuffered = true;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Name = "FrmCreator";
            this.Text = "Target Pattern Creator";
            this.TopMost = true;
            this.PanelControls.ResumeLayout(false);
            this.PanelControls.PerformLayout();
            this.GroupBoxShapes.ResumeLayout(false);
            this.GroupBoxShapes.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.NumSize)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel PanelControls;
        private System.Windows.Forms.NumericUpDown NumSize;
        private System.Windows.Forms.Label LblSize;
        private System.Windows.Forms.ColorDialog ColorPicker;
        private System.Windows.Forms.Panel PanelColor;
        private System.Windows.Forms.Label LabelColor;
        private System.Windows.Forms.GroupBox GroupBoxShapes;
        private System.Windows.Forms.RadioButton RdbEllipse;
        private System.Windows.Forms.RadioButton RdbRectangle;
        private System.Windows.Forms.RadioButton RdbLine;
        private System.Windows.Forms.CheckBox CBoxFill;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button ButExport;
        private System.Windows.Forms.Button ButReset;
        private System.Windows.Forms.SaveFileDialog SaveDialog;

    }
}

