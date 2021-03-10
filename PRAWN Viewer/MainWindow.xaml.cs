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
                string connectstring = string.Format("Provider=Microsoft.ACE.OLEDB.12.0;Data Source=" + dlg.FileName);
                con = new OleDbConnection(connectstring);
                con.Open();

                OleDbCommand cmd = new OleDbCommand
                {
                    CommandText = "select* from [tblOffences]",
                    Connection = con
                };
                rd = cmd.ExecuteReader();
                while (rd.Read())
                {
                    dg_Offences.ItemsSource = rd;
                    dg_Offences.Items.Refresh();
                    tabNav.SelectedItem = LogsTab;    // Move to logs tab
                    CreateTimer();
                }
            }
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            if (rd != null && !rd.IsClosed)
                rd.Close();
            if (con != null && con.State == ConnectionState.Open)
                con.Close();
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
            dispatcherTimer.Tick += new EventHandler(dispatcherTimer_Tick);
            dispatcherTimer.Interval = new TimeSpan(0, 1, 0);   // Update every minute
            dispatcherTimer.Start();
        }

        private void dispatcherTimer_Tick(object sender, EventArgs e)
        {
            OleDbCommand cmd = new OleDbCommand
            {
                CommandText = "select* from [tblOffences]",
                Connection = con
            };
            rd = cmd.ExecuteReader();
            while (rd.Read())
            {
                dg_Offences.ItemsSource = rd;
                dg_Offences.Items.Refresh();
            }
        }
    }
}
