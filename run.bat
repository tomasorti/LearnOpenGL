@echo off
echo Guess file to run...
dir /s /b *.exe > tmpfile
FOR /F %%i IN (tmpfile) DO set FILE=%%i

echo Guessed: %FILE%
del tmpfile

call %FILE%
