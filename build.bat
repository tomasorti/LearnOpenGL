@echo off
set FILE=%~f1
if "%~1"=="" ( goto find ) else goto compile

:find
echo Guess file to compile
dir /s /b *.cpp > tmpfile
FOR /F %%i IN (tmpfile) DO set FILE=%%i
del tmpfile
@echo on
echo Guessed: %FILE%

:compile
echo Compiling...       
@echo off
set ROOT=%userprofile%\LearnOpenGL

for /F "delims=" %%i in ("%FILE%") do set FILEPATH="%%~di%%~pi\"
for /F "delims=" %%i in ("%FILE%") do set NAME_EXT="%%~ni%%~xi"
for /F "delims=" %%i in ("%FILE%") do set NAME="%%~ni"

set OBJ=%FILEPATH:~1,-1%%NAME:~1,-1%.obj
set EXE=%FILEPATH:~1,-1%%NAME:~1,-1%.exe
@echo on

CL.exe /c /I%ROOT%\includes /Zi /nologo /W3 /WX- /diagnostics:column /Od /Ob0 /D WIN32 /D _WINDOWS /D _MBCS /Gm- /RTC1 /MDd /GS /fp:precise /Zc:wchar_t /Zc:forScope /Zc:inline /GR /Fo%FILEPATH% /Gd /TP /errorReport:prompt  /EHsc %FILE%

link.exe /ERRORREPORT:PROMPT /OUT:%EXE% /INCREMENTAL /NOLOGO /LIBPATH:%ROOT%/lib /LIBPATH:%FILEPATH% glfw3.lib opengl32.lib assimp.lib STB_IMAGE.lib GLAD.lib kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib /MANIFEST /MANIFESTUAC:"level='asInvoker' uiAccess='false'" /manifest:embed /SUBSYSTEM:CONSOLE /TLBID:1 /DYNAMICBASE /MACHINE:X64 %OBJ%
