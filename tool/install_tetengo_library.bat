@echo off
rem Installs tetengo library
rem Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/

setlocal
set INSTALL_PATH=%1

:loop
if not ""%INSTALL_PATH%""=="""" (
    echo Installing tetengo...
    
    xcopy /s /i /f /y "%~dp0"..\library\json\c\include\tetengo\json   %INSTALL_PATH%\include\tetengo\json\
    xcopy /s /i /f /y "%~dp0"..\library\json\cpp\include\tetengo\json %INSTALL_PATH%\include.cpp\tetengo\json\
    
    xcopy /s /i /f /y "%~dp0"..\library\lattice\c\include\tetengo\lattice   %INSTALL_PATH%\include\tetengo\lattice\
    xcopy /s /i /f /y "%~dp0"..\library\lattice\cpp\include\tetengo\lattice %INSTALL_PATH%\include.cpp\tetengo\lattice\
    
    xcopy /s /i /f /y "%~dp0"..\library\platform_dependent\cpp\include\tetengo\platform_dependent %INSTALL_PATH%\include.cpp\tetengo\platform_dependent\
    
    xcopy /s /i /f /y "%~dp0"..\library\text\c\include\tetengo\text   %INSTALL_PATH%\include\tetengo\text\
    xcopy /s /i /f /y "%~dp0"..\library\text\cpp\include\tetengo\text %INSTALL_PATH%\include.cpp\tetengo\text\
    
    xcopy /s /i /f /y "%~dp0"..\library\trie\c\include\tetengo\trie   %INSTALL_PATH%\include\tetengo\trie\
    xcopy /s /i /f /y "%~dp0"..\library\trie\cpp\include\tetengo\trie %INSTALL_PATH%\include.cpp\tetengo\trie\
    
    if exist "%~dp0"..\bin\Release.Win32 (
        xcopy /i /f /y "%~dp0"..\bin\Release.Win32\tetengo.*.lib %INSTALL_PATH%\lib.Release.Win32\
        xcopy /i /f /y "%~dp0"..\bin\Release.Win32\tetengo.*.dll %INSTALL_PATH%\lib.Release.Win32\
        xcopy /i /f /y "%~dp0"..\bin\Release.Win32\tetengo.*.pdb %INSTALL_PATH%\lib.Release.Win32\
        
        xcopy /i /f /y "%~dp0"..\bin\Release.Win32\json2timetable.exe %INSTALL_PATH%\sample\json2timetable.Win32\
        xcopy /i /f /y "%~dp0"..\bin\Release.Win32\tetengo.json.dll   %INSTALL_PATH%\sample\json2timetable.Win32\
        xcopy /i /f /y "%~dp0"..\bin\Release.Win32\tetengo.text.dll   %INSTALL_PATH%\sample\json2timetable.Win32\
        
        xcopy /i /f /y "%~dp0"..\bin\Release.Win32\make_dict.exe %INSTALL_PATH%\sample\make_dict.Win32\
        
        xcopy /i /f /y "%~dp0"..\bin\Release.Win32\search_dict.exe %INSTALL_PATH%\sample\search_dict.Win32\
        
        xcopy /i /f /y "%~dp0"..\bin\Release.Win32\search_dict_c.exe %INSTALL_PATH%\sample\search_dict_c.Win32\
        xcopy /i /f /y "%~dp0"..\bin\Release.Win32\tetengo.trie.dll  %INSTALL_PATH%\sample\search_dict_c.Win32\
        
        xcopy /i /f /y "%~dp0"..\bin\Release.Win32\transfer_trains.exe %INSTALL_PATH%\sample\transfer_trains.Win32\
    )
    if exist "%~dp0"..\bin\Debug.Win32 (
        xcopy /i /f /y "%~dp0"..\bin\Debug.Win32\tetengo.*.lib %INSTALL_PATH%\lib.Debug.Win32\
        xcopy /i /f /y "%~dp0"..\bin\Debug.Win32\tetengo.*.dll %INSTALL_PATH%\lib.Debug.Win32\
        xcopy /i /f /y "%~dp0"..\bin\Debug.Win32\tetengo.*.pdb %INSTALL_PATH%\lib.Debug.Win32\
    )
    if exist "%~dp0"..\bin\Release.x64 (
        xcopy /i /f /y "%~dp0"..\bin\Release.x64\tetengo.*.lib %INSTALL_PATH%\lib.Release.x64\
        xcopy /i /f /y "%~dp0"..\bin\Release.x64\tetengo.*.dll %INSTALL_PATH%\lib.Release.x64\
        xcopy /i /f /y "%~dp0"..\bin\Release.x64\tetengo.*.pdb %INSTALL_PATH%\lib.Release.x64\
        
        xcopy /i /f /y "%~dp0"..\bin\Release.x64\json2timetable.exe  %INSTALL_PATH%\sample\json2timetable.x64\
        xcopy /i /f /y "%~dp0"..\bin\Release.x64\tetengo.json.dll    %INSTALL_PATH%\sample\json2timetable.x64\
        xcopy /i /f /y "%~dp0"..\bin\Release.x64\tetengo.text.dll    %INSTALL_PATH%\sample\json2timetable.x64\
        
        xcopy /i /f /y "%~dp0"..\bin\Release.x64\make_dict.exe %INSTALL_PATH%\sample\make_dict.x64\
        
        xcopy /i /f /y "%~dp0"..\bin\Release.x64\search_dict.exe %INSTALL_PATH%\sample\search_dict.x64\
        
        xcopy /i /f /y "%~dp0"..\bin\Release.x64\search_dict_c.exe %INSTALL_PATH%\sample\search_dict_c.x64\
        xcopy /i /f /y "%~dp0"..\bin\Release.x64\tetengo.trie.dll  %INSTALL_PATH%\sample\search_dict_c.x64\
        
        xcopy /i /f /y "%~dp0"..\bin\Release.x64\transfer_trains.exe %INSTALL_PATH%\sample\transfer_trains.x64\
    )
    if exist "%~dp0"..\bin\Debug.x64 (
        xcopy /i /f /y "%~dp0"..\bin\Debug.x64\tetengo.*.lib %INSTALL_PATH%\lib.Debug.x64\
        xcopy /i /f /y "%~dp0"..\bin\Debug.x64\tetengo.*.dll %INSTALL_PATH%\lib.Debug.x64\
        xcopy /i /f /y "%~dp0"..\bin\Debug.x64\tetengo.*.pdb %INSTALL_PATH%\lib.Debug.x64\
    )
    
    xcopy /i /f /y "%~dp0"..\sample\json2timetable\data\hohi_down.json %INSTALL_PATH%\sample\json2timetable.Win32\
    xcopy /i /f /y "%~dp0"..\sample\json2timetable\data\hohi_up.json   %INSTALL_PATH%\sample\json2timetable.Win32\
    xcopy /i /f /y "%~dp0"..\sample\json2timetable\README.md           %INSTALL_PATH%\sample\json2timetable.Win32\
    xcopy /i /f /y "%~dp0"..\sample\json2timetable\data\hohi_down.json %INSTALL_PATH%\sample\json2timetable.x64\
    xcopy /i /f /y "%~dp0"..\sample\json2timetable\data\hohi_up.json   %INSTALL_PATH%\sample\json2timetable.x64\
    xcopy /i /f /y "%~dp0"..\sample\json2timetable\README.md           %INSTALL_PATH%\sample\json2timetable.x64\
    
    xcopy /i /f /y "%~dp0"..\sample\make_dict\README.md %INSTALL_PATH%\sample\make_dict.Win32\
    xcopy /i /f /y "%~dp0"..\sample\make_dict\README.md %INSTALL_PATH%\sample\make_dict.x64\
    
    xcopy /i /f /y "%~dp0"..\sample\search_dict\README.md %INSTALL_PATH%\sample\search_dict.Win32\
    xcopy /i /f /y "%~dp0"..\sample\search_dict\README.md %INSTALL_PATH%\sample\search_dict.x64\
    
    xcopy /i /f /y "%~dp0"..\sample\search_dict_c\README.md %INSTALL_PATH%\sample\search_dict_c.Win32\
    xcopy /i /f /y "%~dp0"..\sample\search_dict_c\README.md %INSTALL_PATH%\sample\search_dict_c.x64\
    
    xcopy /i /f /y "%~dp0"..\sample\transfer_trains\data\kagoshima_down.txt %INSTALL_PATH%\sample\transfer_trains.Win32\
    xcopy /i /f /y "%~dp0"..\sample\transfer_trains\data\kotoku_up.txt      %INSTALL_PATH%\sample\transfer_trains.Win32\
    xcopy /i /f /y "%~dp0"..\sample\transfer_trains\README.md               %INSTALL_PATH%\sample\transfer_trains.Win32\
    xcopy /i /f /y "%~dp0"..\sample\transfer_trains\data\kagoshima_down.txt %INSTALL_PATH%\sample\transfer_trains.x64\
    xcopy /i /f /y "%~dp0"..\sample\transfer_trains\data\kotoku_up.txt      %INSTALL_PATH%\sample\transfer_trains.x64\
    xcopy /i /f /y "%~dp0"..\sample\transfer_trains\README.md               %INSTALL_PATH%\sample\transfer_trains.x64\
    
    xcopy /f /y "%~dp0"..\README.md %INSTALL_PATH%\
    xcopy /f /y "%~dp0"..\COPYING   %INSTALL_PATH%\
    
    echo Done.
    if "%PAUSE_ON_EXIT%"=="1" pause
    exit /b 0
)

set /p INSTALL_PATH=Enter the path where to install: 
if not "%INSTALL_PATH%"=="" set INSTALL_PATH="%INSTALL_PATH%"
set PAUSE_ON_EXIT=1
goto :loop
