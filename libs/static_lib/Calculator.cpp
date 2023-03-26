#include <cstdio>

void Display();
void Play();

/* Lib file contains several .obj to make link easier */
/*
cl /c .\Audio.cpp .\Display.cpp .\Calculator.cpp
lib.exe .\Display.obj .\Audio.obj /out:DisplayAudio.lib
link .\Calculator.obj .\DisplayAudio.lib /out:Calc.exe
*/

int main(void)
{
    /* code */
    Display();
    printf("Starting calculator\n");
}
