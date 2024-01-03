#include "PreCompile.h"
#include "BuildConfg.h"
#include <iostream>
#include <Windows.h>
#include <sql.h>
#include <sqlext.h>
#include "Path.h"
#include "DBServer.h"

#include "ODBCConnector.h"
#include "StoredProcedure.h"
#include "ODBCConst.h"

#include "GoogleTest.h"

using namespace std;

bool TryDBMigration()
{
    char cwd[512];
    if (_getcwd(cwd, sizeof(cwd)) == NULL)
    {
        return false;
    }

    string migratorPath = cwd + MIGRATOR_PATH;

    int migratorResult = system(migratorPath.c_str());
    if (migratorResult != 0)
    {
        return false;
    }

    return true;
}

int main() 
{
    if (TryDBMigration() == false)
    {
        cout << "---------------------" << endl;
        cout << "Migration failed" << endl;
        cout << "---------------------" << endl << endl << endl;
        return 0;
    }
    cout << "---------------------" << endl;
    cout << "Migration successed" << endl;
    cout << "---------------------" << endl << endl << endl;

#if UNIT_TEST
    if (GTestHelper::StartTest() == false)
    {
        cout << "---------------------" << endl;
        cout << "GTest failed" << endl;
        cout << "---------------------" << endl << endl << endl;
        return 0;
    }
    cout << "---------------------" << endl;
    cout << "GTest successed" << endl;
    cout << "---------------------" << endl << endl << endl;
#endif

    ODBCConnector& connector = ODBCConnector::GetInst();
    do
    {
        if (connector.ConnectDB(L"DBServerOptionFile/DBConnectFile.txt") == false)
        {
            cout << "---------------------" << endl;
            cout << "ConnectDB() failed" << endl;
            cout << "---------------------" << endl << endl << endl;
            break;
        }

        if (connector.InitDB() == false)
        {
            cout << "---------------------" << endl;
            cout << "InitDB() failed" << endl;
            cout << "---------------------" << endl << endl << endl;
            break;
        }

        cout << "---------------------" << endl;
        cout << "InitDB() Success" << endl;
        cout << "---------------------" << endl << endl << endl;
    } while (false);

    DBServer dbServer(L"DBServerOptionFile/DBServerOptionFile.txt");

    while (true)
    {
        Sleep(1000);

        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
        {
            dbServer.StopServer();
            break;
        }
    }

    connector.DisconnectDB();

    return 0;
}
