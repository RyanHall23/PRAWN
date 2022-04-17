#include "DatabaseHelper.h"

CDatabaseHelper::CDatabaseHelper()
{

}

CDatabaseHelper::~CDatabaseHelper()
{

}

/// <summary>
/// Updates to database using selected directories using ODBC
/// https://docs.microsoft.com/en-us/previous-versions/office/developer/office-2007/cc811599(v=office.12)
/// 64-bit Install https://www.microsoft.com/en-gb/download/details.aspx?id=13255
/// 64-bit Installs MUST use a 64bit CPU build!!
/// Must be the only ODBC driver?? (Microsoft Server ODBC must not be installed) 2010 Only (Cannot run with 2016 installed also, more precise command?)
/// If 2016 is installed ontop of 2010 both must be uninstalled and 2010 (linked) ODBC driver must be re-installed
/// </summary>
/// <param name="strRegistrationPlate"></param>
/// <param name="strOffence"></param>
/// <param name="strLocation"></param>
/// <param name="strDBLocation"></param>
/// <returns></returns>
bool CDatabaseHelper::UpdateDatabase(std::string strRegistrationPlate, std::string strOffence, std::string strLocation, std::string strDBLocation)
{
    bool bRet = false;

    try
    {
        RETCODE rc;
        int     iConnStrLength2Ptr;
        WCHAR   szConnStrOut[256];
        HSTMT   hStmt;
        HENV    hEnv;
        HDBC    hDbc;
        std::string szDatabaseDriver = "Driver={Microsoft Access Driver (*.mdb, *.accdb)};DSN='';DBQ=" + strDBLocation + ";";

        /* Allocate an environment handle */
        rc = SQLAllocEnv(&hEnv);
        /* Allocate a connection handle */
        rc = SQLAllocConnect(hEnv, &hDbc);

        /* Connect to the database */
        rc = SQLDriverConnect(hDbc, NULL, (SQLCHAR*)szDatabaseDriver.c_str(), SQL_NTS, (SQLCHAR*)szConnStrOut, 255, (SQLSMALLINT*)&iConnStrLength2Ptr, SQL_DRIVER_NOPROMPT);
        if (SQL_SUCCEEDED(rc))
        {
            std::string prepSQL = ("INSERT INTO [tblOffences] (Registration, Location, Offence) VALUES ('" + strRegistrationPlate + "','" + strLocation + "','" + strOffence + "')");
            rc = SQLAllocStmt(hDbc, &hStmt);
            rc = SQLPrepare(hStmt, (SQLCHAR*)prepSQL.c_str(), SQL_NTS);

            /* Excecute the query */
            rc = SQLExecute(hStmt);
            if (SQL_SUCCEEDED(rc))
            {
                bRet = true;
                #ifdef _DEBUG
                OutputDebugString((LPCSTR)"Successfully connected to database. Data source name: ");
                OutputDebugString((LPCSTR)szConnStrOut);
                OutputDebugString("\n");
                OutputDebugString((LPCSTR)"SQL query: ");
                OutputDebugString((LPCSTR)prepSQL.c_str());
                OutputDebugString("\n");
                #endif
            }
            else    // Failed
            {
                bRet = false;
            }
        }
        else
        {
            #ifdef _DEBUG
            OutputDebugString((LPCSTR)"Couldn't connect to: ");   // Debug output device name
            OutputDebugString((LPCSTR)szDatabaseDriver.c_str());
            #endif

            bRet = false;
        }
        /* Disconnect and free up allocated handles */
        SQLDisconnect(hDbc);
        SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
        SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
    }
    catch (...)
    {
        bRet = false;
    }
    return bRet;
}

/// <summary>
/// Returns a bool for pass/fail result of database connection test
/// </summary>
/// <returns></returns>
bool CDatabaseHelper::TestDatabase()
{
    CMenuCLI pMenuCLI;
    bool bResult = UpdateDatabase("Test Vehicle", "Test Offence", pDevProp.m_strScannerLocation, pDevProp.m_strDatabaseDirectory);

    if (bResult)
    {
        pMenuCLI.UpdateDatabaseResultString("Test Passed");
    }
    else
    {
        pMenuCLI.UpdateDatabaseResultString("Test Failed");
    }

    return bResult;
}