using Microsoft.Win32;
using ScottPlot;
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
            openFileDialog.Filter = "output|*.path";
            if (openFileDialog.ShowDialog() == true)
            {
                String path = openFileDialog.FileName;
                PathTextBox.Text = path;

                try
                {
                    ulong pathCount;
                    double[] X; double[] Y;
                    using (FileStream fileStream = new FileStream(path, FileMode.Open, FileAccess.Read))
                    {
                        using(BinaryReader binaryReader = new BinaryReader(fileStream))
                        {
                            pathCount = binaryReader.ReadUInt64();
                            X = new double[pathCount];
                            Y = new double[pathCount];
                            for (ulong i = 0; i < pathCount; i++)
                            {
                                X[i] = binaryReader.ReadDouble();
                                Y[i] = binaryReader.ReadDouble();
                            }
                        }
                    }
                    double[,] data = new double[200, 200];
                    double xmin = -2; double xmax = 2;  double dx = (xmax - xmin) / 200.0;
                    double ymin = -1; double ymax = 3;  double dy = (ymax - ymin) / 200.0;
                    for (int i = 0; i < 200; i++)
                    {
                        double y = ymax - i * dy;
                        for (int j = 0; j < 200; j++)
                        {
                            double x = xmin + j * dx;
                            data[i,j] = (1 - x) * (1 - x) + 100 * (y - x * x) * (y - x * x);
                        }
                    }
                    double r = 25;
                    for (int i = 0; i < 200; i++)
                        for (int j = 0; j < 200; j++)
                            data[i, j] = data[i, j] % r;
                    var h = PathPlot.Plot.Add.Heatmap(data);
                    h.Position = new(new ScottPlot.Coordinates(-2.0, -1.0), new ScottPlot.Coordinates(2.0, 3.0));
                    h.Colormap = new ScottPlot.Colormaps.Turbo();
                    //PathPlot.Plot.Add.ColorBar(h);

                    var pathPlot = PathPlot.Plot.Add.Scatter(X, Y);
                    pathPlot.Color = ScottPlot.Colors.OrangeRed;

                    PathPlot.Plot.Add.HorizontalLine(0);
                    PathPlot.Plot.Add.VerticalLine(0);

                    var l1 = PathPlot.Plot.Add.Line(new ScottPlot.Coordinates(-10, 0.35), new ScottPlot.Coordinates(10, 0.35));
                    l1.LineWidth = 5;
                    var l2 = PathPlot.Plot.Add.Line(new ScottPlot.Coordinates(-0.5, -1), new ScottPlot.Coordinates(2, 2.5));
                    l2.LineWidth = 5;
                    var l3 = PathPlot.Plot.Add.Line(new ScottPlot.Coordinates(3, 0), new ScottPlot.Coordinates(0, 3));
                    l3.LineWidth = 5;

                    PathPlot.Refresh();

                    StringBuilder sb = new StringBuilder();
                    sb.Append($"""PathCount = {pathCount}""");
                    sb.Append('\n');
                    for (ulong i = 0; i < pathCount; i++)
                    {
                        sb.Append($"""x = {X[i].ToString("0.####")} | y = {Y[i].ToString("0.####")}""");
                        sb.Append('\n');
                    }
                    LogTextBox.Text = sb.ToString();
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Error: " + ex.Message);
                }
            }
        }
    }
}