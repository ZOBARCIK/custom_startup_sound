#include <windows.h>
#include <iostream>
#include <thread>
/*#pragma comment(lib, "winmm.lib") */
/*g++ armstrong.cpp -o armstrong -lwinmm -mwindows terminal command to compile for windows*/
using namespace std;

SERVICE_STATUS serviceStatus;
SERVICE_STATUS_HANDLE hStatus;

void  ServiceMain(int argc, char** argv); 
void  ControlHandler(DWORD request); 
char serviceName[] = "thegreyfray";
int main() {
        // Open a handle to the SCM database. 
    SC_HANDLE schSCManager = OpenSCManager( 
        NULL,                    // local computer
        NULL,                    // ServicesActive database 
        SC_MANAGER_ALL_ACCESS);  // full access rights 

    if (NULL == schSCManager) {
        printf("OpenSCManager failed (%d)\n", GetLastError());
        return 1;
    }

    // Create the service
    SC_HANDLE schService = CreateService( 
        schSCManager,              // SCM database 
        "thegreyfray",               // name of service 
        "thegreyfray",               // service name to display 
        SERVICE_ALL_ACCESS,        // desired access 
        SERVICE_WIN32_OWN_PROCESS, // service type 
        SERVICE_AUTO_START,        // start type 
        SERVICE_ERROR_NORMAL,      // error control type 
        "D:\\thegrey\\intothefray.exe", // path to service's binary 
        NULL,                      // no load ordering group 
        NULL,                      // no tag identifier 
        NULL,                      // no dependencies 
        NULL,                      // LocalSystem account 
        NULL);                     // no password 

    if (schService == NULL) {
        printf("CreateService failed (%d)\n", GetLastError()); 
        CloseServiceHandle(schSCManager);
        return 1;
    } else {
        printf("Service installed successfully\n"); 
    }

    CloseServiceHandle(schService); 
    CloseServiceHandle(schSCManager);
    SERVICE_TABLE_ENTRY ServiceTable[] = {
        {serviceName, (LPSERVICE_MAIN_FUNCTION)ServiceMain},
        {NULL, NULL}
    };

    // Start the control dispatcher thread for our service
    StartServiceCtrlDispatcher(ServiceTable);  
    return 0;
}

/*void WorkerThread() {

 (!PlaySound(TEXT("D:\\thegrey\\intothefray.wav"), NULL, SND_FILENAME)) ;
    return;
}*/

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