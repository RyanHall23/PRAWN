using Microsoft.Win32;
using System;
using System.Data;
using System.Data.OleDb;
using System.Windows;
using System.Windows.Controls;

namespace PRAWN_Viewer
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private OleDbConnection con;
        private OleDbDataReader rd;
        private System.Windows.Threading.DispatcherTimer dispatcherTimer = new System.Windows.Threading.DispatcherTimer();

        private const string strSQLSelectAllSatement = "select* from [tblOffences]";
        private const string strConnectionStringEngine = "Provider=Microsoft.ACE.OLEDB.12.0;Data Source=";

        public MainWindow()
        {
            InitializeComponent();
            WbMapViewer.Navigate("https://www.google.com");
        }

        private void BtnOpenDatabase_Click(object sender, RoutedEventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog
            {
                Filter = "Database Files|*.mdb;*.accdb;"
            };

            if (dlg.ShowDialog() == true)
            {
                string connectstring = string.Format(strConnectionStringEngine + dlg.FileName);
                con = new OleDbConnection(connectstring);
                con.Open();

                UpdateDataGrid();
            }
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            if (rd != null && !rd.IsClosed)
            {
                rd.Close();
            }
            if (con != null && con.State == ConnectionState.Open)
            {
                con.Close();
            }
        }

        private void TabControl_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (MapTab.IsSelected)
            {
                // Update web stuff
            }
        }

        private void CreateTimer()
        {
            dispatcherTimer.Tick += new EventHandler(Timer_OnTick);
            dispatcherTimer.Interval = new TimeSpan(0, 1, 0);   // Update every minute
            dispatcherTimer.Start();
        }

        private void Timer_OnTick(object sender, EventArgs e)
        {
            UpdateDataGrid();
            UpdateMap();
        }

        private void UpdateDataGrid()
        {
            OleDbCommand cmd = new OleDbCommand
            {
                CommandText = strSQLSelectAllSatement,
                Connection = con
            };
            rd = cmd.ExecuteReader();
            while (rd.Read())
            {
                dg_Offences.ItemsSource = rd;
                dg_Offences.Items.Refresh();
                tabNav.SelectedItem = LogsTab;    // Move to logs tab, displaying table
                CreateTimer();
            }
        }

        private void UpdateMap()
        {
            // Get bottom most row and location column value
            // Append value on to Google Maps link
            // Run in WebView using google maps link
            // Manual update?
        }
    }
}
