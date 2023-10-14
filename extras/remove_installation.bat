@echo off

if exist "%LOCALAPPDATA%\Arduino15\package_nm180100_index.json" (
    del /Q "%LOCALAPPDATA%\Arduino15\package_nm180100_index.json"
)

if exist "%LOCALAPPDATA%\Arduino15\staging\packages\ArduinoCore*.tar.bz2" (
    del /Q "%LOCALAPPDATA%\Arduino15\staging\packages\ArduinoCore*.tar.bz2"
)

if exist "%LOCALAPPDATA%\Arduino15\packages\nmi" (
    rmdir /Q /S "%LOCALAPPDATA%\Arduino15\packages\nmi"
)
