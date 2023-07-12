
namespace WindowsFormsHelloWorldApp
{
    partial class Form1
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
            this._button_pressMe = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // _button_pressMe
            // 
            this._button_pressMe.Dock = System.Windows.Forms.DockStyle.Fill;
            this._button_pressMe.Location = new System.Drawing.Point(0, 0);
            this._button_pressMe.Name = "_button_pressMe";
            this._button_pressMe.Size = new System.Drawing.Size(251, 244);
            this._button_pressMe.TabIndex = 0;
            this._button_pressMe.Text = "Press me!";
            this._button_pressMe.UseVisualStyleBackColor = true;
            this._button_pressMe.Click += new System.EventHandler(this._button_pressMe_Click);
            // 
            // Form1
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.ClientSize = new System.Drawing.Size(251, 244);
            this.Controls.Add(this._button_pressMe);
            this.Font = new System.Drawing.Font("Calibri", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Form1";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button _button_pressMe;
    }
}

