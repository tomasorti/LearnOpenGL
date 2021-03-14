@REM Build for Visual Studio compiler. Run your copy of vcvars32.bat or vcvarsall.bat to setup command-line compiler.
@set OUT_DIR=Debug
@set OUT_EXE=main
@set DEFINES=/DIMGUI_IMPL_OPENGL_LOADER_GL3W /D
@set INCLUDES=/I. /Iimgui /Ilibs\glfw\include /Ilibs\gl3w
@set SOURCES=main.cpp imgui\imgui*.cpp libs\gl3w\GL\gl3w.c
@set LIBS=/LIBPATH:libs\glfw\lib-vc2010-64 glfw3.lib opengl32.lib gdi32.lib shell32.lib
mkdir %OUT_DIR%
cl /nologo /Zi /MD %INCLUDES% %SOURCES% /Fe%OUT_DIR%/%OUT_EXE%.exe /Fo%OUT_DIR%/ /link %LIBS%
