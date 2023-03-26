/* Interface file */
#pragma once

/*
cl /c .\Display.cpp
cl /LD .\Display.cpp /Fe:Display.dll
dumpbin.exe .\Display.dll

Dump of file .\Display.dll

File Type: DLL

  Summary

        2000 .data
        7000 .rdata
        1000 .reloc
       12000 .text

link .\Audio.obj .\Display.obj /DLL /implib:DisplayAudio.lib /out:DisplayAudio.dll

Getting .exe dependencies

dumpbin.exe .\Calc.exe /dependents

Dump of file .\Calc.exe

File Type: EXECUTABLE IMAGE

  Image has the following dependencies:

    DisplayAudio.dll
    KERNEL32.dll

  Summary

        2000 .data
        7000 .rdata
        1000 .reloc
       13000 .text

*/

/* Need to set which symbols will be exported from dll*/
__declspec(dllexport) void Display();
