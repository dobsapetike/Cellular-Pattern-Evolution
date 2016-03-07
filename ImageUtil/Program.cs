using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;

namespace ImageUtil
{
    /// <summary>
    /// Simple utility app, which prints the generation number on the 
    /// generated CA simulation images.
    /// </summary>
    class Program
    {
        private const int FileSize = 500;
        private const string FilePath = "pics";
        private const string FilePrefix = "eval";
        
        private static readonly Font Font = new Font("Arial", 16);

        // Gets the directory name as argument
        static void Main(string[] args)
        {
            var path = Path.Combine(FilePath, args[0]);
            var files = Directory.GetFiles(path);
            foreach (var filepath in files)
            {
                var filename = Path.GetFileNameWithoutExtension(filepath);
                if (filename == null) continue;
                if (!filename.StartsWith(FilePrefix)) continue;

                // filename is of format '$prefix_$generationNumber'
                var parts = filename.Split('_');
                var geNum = Int32.Parse(parts[1]);

                var bitmap = new Bitmap(filepath);
                using (var g = Graphics.FromImage(bitmap))
                using (var memory = new MemoryStream())
                using (var fs = new FileStream(Path.Combine(path, parts[0] + ".png"), 
                    FileMode.Create, FileAccess.ReadWrite))
                {
                    g.DrawString(string.Format("Generation {0}", geNum), Font,
                        Brushes.Red, new PointF(10, FileSize - 30));
                    bitmap.Save(memory, ImageFormat.Png);
                    var bytes = memory.ToArray();
                    fs.Write(bytes, 0, bytes.Length);
                }
            }
        }
    }
}
