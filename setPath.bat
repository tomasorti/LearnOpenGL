@echo off
echo %path% > tmppath
(findstr /IC:"LearnOpenGL" "tmppath" > Nul && (
         Echo LearnOpenGL path already set.
         ) || (
         Echo Adding LearnOpenGL to path.
         set "path=%path%;%cd%"
         )
)
del tmppath
