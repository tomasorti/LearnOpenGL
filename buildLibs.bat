@echo off
call setPath.bat

@echo on
rem Build stb_image
CL.exe /c /I.\build\configuration /I.\includes /Zi /nologo /W3 /WX- /diagnostics:column /Od /Ob0 /D WIN32 /D _WINDOWS /D _MBCS /Gm- /RTC1 /MDd /GS /fp:precise /Zc:wchar_t /Zc:forScope /Zc:inline /GR /Fo".\src\\" /Fd".\src\STB_IMAGE.pdb" /Gd /TP /errorReport:prompt  /EHsc .\src\stb_image.cpp

Lib.exe /OUT:".\lib\STB_IMAGE.lib" /NOLOGO /MACHINE:X64  /machine:x64 .\src\stb_image.obj

rem Build glad
CL.exe /c /I.\build\configuration /I.\includes /Zi /nologo /W3 /WX- /diagnostics:column /Od /Ob0 /D WIN32 /D _WINDOWS /D _MBCS /Gm- /RTC1 /MDd /GS /fp:precise /Zc:wchar_t /Zc:forScope /Zc:inline /Fo".\src\\" /Fd".\src\GLAD.pdb" /Gd /TC /errorReport:prompt .\src\glad.c

Lib.exe /OUT:".\lib\GLAD.lib" /NOLOGO /MACHINE:X64  /machine:x64 .\src\glad.obj
