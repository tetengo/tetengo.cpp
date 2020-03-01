@echo off
rem Installs tetengo library
rem Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/

setlocal
set INSTALL_PATH=%1

:loop
if not ""%INSTALL_PATH%""=="""" (
    if not exist "%~dp0"..\bin\Release.Win32 (
        echo The 32-bit library files does not look built yet. Build them first.
        if "%PAUSE_ON_EXIT%"=="1" pause
        exit /b 1
    )
    if not exist "%~dp0"..\bin\Release.x64 (
        echo The 64-bit library files does not look built yet. Build them first.
        if "%PAUSE_ON_EXIT%"=="1" pause
        exit /b 1
    )
    
    echo Installing tetengo...
    
    xcopy /s /i /f /y "%~dp0"..\library\trie\c\include\tetengo\trie   %INSTALL_PATH%\include\tetengo\trie
    xcopy /s /i /f /y "%~dp0"..\library\trie\cpp\include\tetengo\trie %INSTALL_PATH%\include.cpp\tetengo\trie
    
    xcopy /i /f /y "%~dp0"..\bin\Release.Win32\*.lib %INSTALL_PATH%\lib.Win32
    xcopy /i /f /y "%~dp0"..\bin\Release.Win32\*.dll %INSTALL_PATH%\lib.Win32
    xcopy /i /f /y "%~dp0"..\bin\Release.x64\*.lib   %INSTALL_PATH%\lib.x64
    xcopy /i /f /y "%~dp0"..\bin\Release.x64\*.dll   %INSTALL_PATH%\lib.x64
    
    xcopy /f /y "%~dp0"..\README.md %INSTALL_PATH%
    xcopy /f /y "%~dp0"..\COPYING   %INSTALL_PATH%
    
    echo Done.
    if "%PAUSE_ON_EXIT%"=="1" pause
    exit /b 0
)

set /p INSTALL_PATH=Enter the path where to install: 
if not "%INSTALL_PATH%"=="" set INSTALL_PATH="%INSTALL_PATH%"
set PAUSE_ON_EXIT=1
goto :loop
