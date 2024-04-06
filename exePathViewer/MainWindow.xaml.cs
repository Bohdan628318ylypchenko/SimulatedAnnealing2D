using Microsoft.Win32;
using System.IO;
using System.Runtime.CompilerServices;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace exePathViewer
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        private void ViewButton_Click(object sender, RoutedEventArgs e)
        {
            LogTextBox.Text = "";
            PathPlot.Plot.Clear();

            OpenFileDialog openFileDialog = new OpenFileDialog(); 
            if (openFileDialog.ShowDialog() == true)
            {
                String path = openFileDialog.FileName;
                PathTextBox.Text = path;

                try
                {
                    double[] x; double[] y;
                    using (FileStream fileStream = new FileStream(path, FileMode.Open, FileAccess.Read))
                    {
                        using(BinaryReader binaryReader = new BinaryReader(fileStream))
                        {
                            ulong pathCount = binaryReader.ReadUInt64();
                            x = new double[pathCount];
                            y = new double[pathCount];
                            for (ulong i = 0; i < pathCount; i++)
                            {
                                x[i] = binaryReader.ReadDouble();
                                y[i] = binaryReader.ReadDouble();
                            }
                        }
                    }
                    PathPlot.Plot.Add.Scatter(x, y);
                    PathPlot.Refresh();
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Error: " + ex.Message);
                }
            }
        }
    }
}