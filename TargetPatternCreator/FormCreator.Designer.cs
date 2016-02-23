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
            this.PanelBackgroundColor = new System.Windows.Forms.Panel();
            this.LabelBackgroundColor = new System.Windows.Forms.Label();
            this.ButExport = new System.Windows.Forms.Button();
            this.ButReset = new System.Windows.Forms.Button();
            this.PanelColor = new System.Windows.Forms.Panel();
            this.LabelColor = new System.Windows.Forms.Label();
            this.NumSize = new System.Windows.Forms.NumericUpDown();
            this.LblSize = new System.Windows.Forms.Label();
            this.ColorPicker = new System.Windows.Forms.ColorDialog();
            this.SaveDialog = new System.Windows.Forms.SaveFileDialog();
            this.BackgroundColorPicker = new System.Windows.Forms.ColorDialog();
            this.PanelControls.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.NumSize)).BeginInit();
            this.SuspendLayout();
            // 
            // PanelControls
            // 
            this.PanelControls.BackColor = System.Drawing.SystemColors.Control;
            this.PanelControls.Controls.Add(this.PanelBackgroundColor);
            this.PanelControls.Controls.Add(this.LabelBackgroundColor);
            this.PanelControls.Controls.Add(this.ButExport);
            this.PanelControls.Controls.Add(this.ButReset);
            this.PanelControls.Controls.Add(this.PanelColor);
            this.PanelControls.Controls.Add(this.LabelColor);
            this.PanelControls.Controls.Add(this.NumSize);
            this.PanelControls.Controls.Add(this.LblSize);
            this.PanelControls.Location = new System.Drawing.Point(600, 0);
            this.PanelControls.Name = "PanelControls";
            this.PanelControls.Size = new System.Drawing.Size(200, 600);
            this.PanelControls.TabIndex = 4;
            // 
            // PanelBackgroundColor
            // 
            this.PanelBackgroundColor.BackColor = System.Drawing.Color.White;
            this.PanelBackgroundColor.Location = new System.Drawing.Point(56, 216);
            this.PanelBackgroundColor.Name = "PanelBackgroundColor";
            this.PanelBackgroundColor.Size = new System.Drawing.Size(98, 60);
            this.PanelBackgroundColor.TabIndex = 4;
            this.PanelBackgroundColor.Click += new System.EventHandler(this.PanelBackgroundColor_Click);
            // 
            // LabelBackgroundColor
            // 
            this.LabelBackgroundColor.AutoSize = true;
            this.LabelBackgroundColor.Location = new System.Drawing.Point(14, 184);
            this.LabelBackgroundColor.Name = "LabelBackgroundColor";
            this.LabelBackgroundColor.Size = new System.Drawing.Size(123, 17);
            this.LabelBackgroundColor.TabIndex = 8;
            this.LabelBackgroundColor.Text = "Background color:";
            // 
            // ButExport
            // 
            this.ButExport.Location = new System.Drawing.Point(60, 368);
            this.ButExport.Name = "ButExport";
            this.ButExport.Size = new System.Drawing.Size(90, 30);
            this.ButExport.TabIndex = 7;
            this.ButExport.Text = "Export";
            this.ButExport.UseVisualStyleBackColor = true;
            this.ButExport.Click += new System.EventHandler(this.ButExport_Click);
            // 
            // ButReset
            // 
            this.ButReset.Location = new System.Drawing.Point(60, 319);
            this.ButReset.Name = "ButReset";
            this.ButReset.Size = new System.Drawing.Size(90, 29);
            this.ButReset.TabIndex = 6;
            this.ButReset.Text = "Reset";
            this.ButReset.UseVisualStyleBackColor = true;
            this.ButReset.Click += new System.EventHandler(this.ButReset_Click);
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
            this.LabelColor.Size = new System.Drawing.Size(79, 17);
            this.LabelColor.TabIndex = 2;
            this.LabelColor.Text = "Paint color:";
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
            8,
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
        private System.Windows.Forms.Button ButExport;
        private System.Windows.Forms.Button ButReset;
        private System.Windows.Forms.SaveFileDialog SaveDialog;
        private Panel PanelBackgroundColor;
        private Label LabelBackgroundColor;
        private ColorDialog BackgroundColorPicker;

    }
}

