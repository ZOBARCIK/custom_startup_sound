#include <windows.h>
#include <iostream>
//#pragma comment(lib, "winmm.lib") 
//g++ soundplayer.cpp -o soundplayer -lwinmm -mwindows //terminal command to compile for windows
using namespace std;
int main()
{

    (!PlaySound(TEXT("intothefray.wav"), NULL, SND_FILENAME)) ;

    return 0;
}