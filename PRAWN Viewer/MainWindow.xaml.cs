using Microsoft.Win32;
using System.Data;
using System.Data.OleDb;
using System.Windows;

namespace PRAWN_Viewer
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private OleDbConnection con;
        private OleDbDataReader rd;

        public MainWindow()
        {
            InitializeComponent();
            WbMapViewer.Navigate("Https://www.google.com");
        }

        private void BtnOpenDatabase_Click(object sender, RoutedEventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Filter = "Database Files|*.mdb;*.accdb;";
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
                }
            }
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            if (!rd.IsClosed)
                rd.Close();
            if (con.State == ConnectionState.Open)
                con.Close();
        }
    }
}
