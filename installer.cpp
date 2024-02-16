#include <windows.h>
#include <iostream>
#include <thread>
/*winmm.lib library for sound playing*/
/*compile rc file with: windres installer_admin.rc -O coff -o installer_admin.res*/
/*g++ installer.cpp -o installer installer_admin.res -lwinmm -mwindows terminal command to compile with manifest file, with winmm.lib and no terminal window*/
using namespace std;

SERVICE_STATUS serviceStatus;
SERVICE_STATUS_HANDLE hStatus;


void  ServiceMain(int argc, char** argv); 
void  ControlHandler(DWORD request); 
char serviceName[] = "thegreyfray";


int main() {
    // handle to SCM database, opened connection to the service control manager
    SC_HANDLE schSCManager = OpenSCManager( 
        NULL,                    // local computer
        NULL,                    // ServicesActive database 
        SC_MANAGER_ALL_ACCESS);  // full access rights 

    if (NULL == schSCManager) { // if failed to open the handle
        printf("OpenSCManager failed (%d)\n", GetLastError());
        return 1;
    }

    // Creating the service in msconfig by calling CreateService
    // handle to SCM service creator
    SC_HANDLE schService = CreateService( 
        schSCManager,              // SCManager database 
        serviceName,               // Name of service 
        serviceName,               // Name of display
        SERVICE_ALL_ACCESS,        // Desired access 
        SERVICE_WIN32_OWN_PROCESS, // Service type 
        SERVICE_AUTO_START,        // Service start type 
        SERVICE_ERROR_NORMAL,      // Error control type 
        "D:\\thegrey\\intothefray.exe", // Service's binary 
        NULL,                      // No load ordering group 
        NULL,                      // No tag identifier 
        NULL,                      // Dependencies 
        NULL,                      // Service running account Null for local 
        NULL);                     // Password of the account 

    if (schService == NULL) {
        printf("CreateService failed (%d)\n", GetLastError()); 
        CloseServiceHandle(schSCManager);
        return 1;
    } else {
        printf("Service installed successfully\n"); 
    }

    // close handles
    CloseServiceHandle(schService); 
    CloseServiceHandle(schSCManager);

    //start the service
    SERVICE_TABLE_ENTRY ServiceTable[] = {
        {serviceName, (LPSERVICE_MAIN_FUNCTION)ServiceMain},
        {NULL, NULL}
    };

    // Start the control dispatcher thread for the service
    StartServiceCtrlDispatcher(ServiceTable);  
    return 0;
}

/*void WorkerThread() {

 (!PlaySound(TEXT("D:\\thegrey\\intothefray.wav"), NULL, SND_FILENAME)) ;
    return;
}*/

/// service main function
void ServiceMain(int argc, char** argv) {
    serviceStatus.dwServiceType        = SERVICE_WIN32; 
    serviceStatus.dwCurrentState       = SERVICE_START_PENDING; 
    serviceStatus.dwControlsAccepted   = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
    serviceStatus.dwWin32ExitCode      = 0; 
    serviceStatus.dwServiceSpecificExitCode = 0; 
    serviceStatus.dwCheckPoint         = 0; 
    serviceStatus.dwWaitHint           = 0; 

    hStatus = RegisterServiceCtrlHandler(serviceName, (LPHANDLER_FUNCTION)ControlHandler); 
    serviceStatus.dwCurrentState = SERVICE_RUNNING; 
    SetServiceStatus (hStatus, &serviceStatus);
}


/// handling SCM requests stop, shutdown etc.
void ControlHandler(DWORD request) { 
    switch(request) { 
        case SERVICE_CONTROL_STOP: 
             serviceStatus.dwWin32ExitCode = 0; 
             serviceStatus.dwCurrentState  = SERVICE_STOPPED; 
             SetServiceStatus (hStatus, &serviceStatus);
             return; 

        case SERVICE_CONTROL_SHUTDOWN: 
             serviceStatus.dwWin32ExitCode = 0; 
             serviceStatus.dwCurrentState  = SERVICE_STOPPED; 
             SetServiceStatus (hStatus, &serviceStatus);
             return; 
        
        default:
             break;
    } 
    SetServiceStatus (hStatus,  &serviceStatus);
    return; 
} 