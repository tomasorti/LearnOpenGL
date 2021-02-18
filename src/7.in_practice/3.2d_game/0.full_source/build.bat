set ROOT=%userprofile%\Desktop\OpenGL\LearnOpenGL
set FILEPATH=%ROOT%\src\7.in_practice\3.2d_game\0.full_source

CL.exe /c /Fo:./obj/ /I%ROOT%\includes /Zi /Fdbin/breakout.pdb /nologo /W3 /WX- /diagnostics:column /Od /Ob0 /D WIN32 /D _WINDOWS /D _MBCS /Gm- /RTC1 /MDd /GS /fp:precise /Zc:wchar_t /Zc:forScope /Zc:inline /GR /Gd /TP /errorReport:prompt /EHsc *.cpp

rem 
link.exe /ERRORREPORT:PROMPT /OUT:bin/breakout.exe /INCREMENTAL:NO /NOLOGO /LIBPATH:%ROOT%/lib /LIBPATH:%FILEPATH% glfw3.lib opengl32.lib glad.lib kernel32.lib user32.lib gdi32.lib shell32.lib irrKlang.lib freetype.lib /NODEFAULTLIB:MSVCRT /SUBSYSTEM:CONSOLE obj/*.obj
