using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Media;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace GUIarmstrong
{
    public partial class Form1 : Form
    {
        string soundPath = null;
        SoundPlayer customsound = null;
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {

        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            
            switch (comboBox1.SelectedItem.ToString())
            {
                case "Marc Streitenfeld- Into the Fray":
                    soundPath = "D:\\thegrey.wav";
                    customsound = new SoundPlayer(@soundPath);
                    customsound.Play();
                    break;
                case "Orange Caramel- Catallena":
                    soundPath = "D:\\catallena.wav";
                    customsound = new SoundPlayer(@soundPath);
                    customsound.Play();
                    break;
                case "Choose a .wav file from my computer": // replace "Your Selection" with the actual item text
                    OpenFileDialog openFileDialog = new OpenFileDialog();
                    openFileDialog.Filter = "WAV files (*.wav)|*.wav";
                    if (openFileDialog.ShowDialog() == DialogResult.OK)
                    {
                        string soundPath = openFileDialog.FileName;
                        customsound = new SoundPlayer(@soundPath);
                        customsound.Play();
                    }
                    break;
                    // more cases
            }

            
        }

        private void button1_Click_1(object sender, EventArgs e)
        {
            //change the sound files name and copy it to the target directory
            if (soundPath != null)
            {
                string targetDirectory = "D:\\thegrey"; 
                string targetFileName = "CustomStartupSound.wav"; 

                string targetPath = Path.Combine(targetDirectory, targetFileName);
                File.Copy(soundPath, targetPath, true); //true for rewrite

                MessageBox.Show("Sound file has been selected.\n Restart your computer to see the effects.");
                Application.Exit();
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }
    }
}
