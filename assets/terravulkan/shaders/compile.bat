@echo off
SETLOCAL ENABLEDELAYEDEXPANSION

FOR %%f IN (*.vert) DO (
    ECHO Compiling %%f
    glslangValidator -V "%%f" -o "%%~nf.spv"
)

FOR %%f IN (*.frag) DO (
    ECHO Compiling %%f
    glslangValidator -V "%%f" -o "%%~nf.spv"
)

ECHO Compilation complete.
ENDLOCAL

PAUSE