#include <Windows.h>
#include <cstdio>

using DisplayFuncPtr = void(*)();
using DisplayXFuncPtr = void(*)(int);

/*
Dump of file .\DisplayAudio.dll

File Type: DLL

  Section contains the following exports for DisplayAudio.dll

    00000000 characteristics
    FFFFFFFF time date stamp
        0.00 version
           1 ordinal base
           3 number of functions
           3 number of names

    ordinal hint RVA      name

          1    0 000010A0 Display
          2    1 000010C0 Display_x
          3    2 00001000 Play
*/

int main()
{
    /* code */
    printf("Loading DLL\n");
    HMODULE DisplayAudioModule = LoadLibraryA("DisplayAudio.dll");
    if(!DisplayAudioModule) 
    {
        printf("Could not load DLL\n");
        return 1;
    }

    DisplayFuncPtr DisplayFunc = (DisplayFuncPtr) GetProcAddress(DisplayAudioModule, "Display");
    DisplayXFuncPtr DisplayXFunc = (DisplayXFuncPtr) GetProcAddress(DisplayAudioModule, "Display_x");
    
    DisplayFunc();
    DisplayXFunc(42);

    printf("Freeing DLL\n");
    FreeLibrary(DisplayAudioModule);
}

