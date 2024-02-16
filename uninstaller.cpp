/* compile rc file with: windres uninstaller_admin.rc -O coff -o uninstaller_admin.res*/
/*g++ uninstaller.cpp -o uninstaller uninstaller_admin.res -lwinmm -mwindows terminal command to compile with manifest file, with winmm.lib and no terminal window*/


#include <windows.h>
#include <iostream>

    using std::cout;


int main() {
    // handle to SCM database, opened connection to the service control manager
    SC_HANDLE schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (NULL == schSCManager) {
        cout << "OpenSCManager failed (" << GetLastError() << ")\n";
        return 1;
    }

    // open the service
    SC_HANDLE schService = OpenService(schSCManager, "thegreyfray", SERVICE_ALL_ACCESS);
    if (schService == NULL) {
        cout << "OpenService failed (" << GetLastError() << ")\n";
        CloseServiceHandle(schSCManager);
        return 1;
    }

    // stop the service if running
    SERVICE_STATUS serviceStatus;
    if (ControlService(schService, SERVICE_CONTROL_STOP, &serviceStatus)) {
        cout << "Stopping service...\n";
        Sleep(1000);

        while (QueryServiceStatus(schService, &serviceStatus)) {
            if (serviceStatus.dwCurrentState == SERVICE_STOP_PENDING) {
                cout << "Waiting...\n";
                Sleep(1000);
            }
            else break;
        }

        if (serviceStatus.dwCurrentState == SERVICE_STOPPED)
            cout << "Service stopped successfully.\n";
        else
            cout << "Service stop failed (" << GetLastError() << ")\n";
    }

    // delete the service
    if (DeleteService(schService)) {
        cout << "Service uninstalled successfully\n";
    } else {
        cout << "DeleteService failed (" << GetLastError() << ")\n";
    }

    // close handles
    CloseServiceHandle(schService);
    CloseServiceHandle(schSCManager);

    return 0;
}