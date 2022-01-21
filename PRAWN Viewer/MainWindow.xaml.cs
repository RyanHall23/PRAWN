using Microsoft.Win32;
using System;
using System.Data;
using System.Data.OleDb;
using System.Windows;
using System.Windows.Controls;
using Microsoft.Web.WebView2.Core;

namespace PRAWN_Viewer
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private OleDbConnection con;
        private OleDbDataReader rd;
        private readonly System.Windows.Threading.DispatcherTimer dispatcherTimer = new System.Windows.Threading.DispatcherTimer();

        private const string strSQLSelectAllSatement = "select* from [tblOffences]";
        private const string strConnectionStringEngine = "Provider=Microsoft.ACE.OLEDB.12.0;Data Source=";

        public MainWindow()
        {
            InitializeComponent();
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
                tabNav.SelectedItem = LogsTab;    // Move to logs tab, displaying table
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

        private void CreateTimer()
        {
            if(!dispatcherTimer.IsEnabled)
            {
                dispatcherTimer.Tick += new EventHandler(Timer_OnTick);
                dispatcherTimer.Interval = new TimeSpan(0, 0, 10);   // Update every minute
                dispatcherTimer.Start();
            }
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
                CreateTimer();
            }
        }

        private void UpdateMap()
        {
            if (webView?.CoreWebView2 != null && dg_Offences.Items.Count > 0)
            {
                IDataRecord rec = (IDataRecord)dg_Offences.Items[dg_Offences.Items.Count - 1];
                webView.CoreWebView2.Navigate("https://www.google.com/maps/place/" + rec.GetString(2));
                // Future Work: string location = rec.GetString(rec.GetOrdinal("Location"); for column name.
            }
        }
    }
}
