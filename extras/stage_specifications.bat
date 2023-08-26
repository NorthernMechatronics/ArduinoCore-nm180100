@echo off
IF "%~1" == "" GOTO error
IF "%~2" == "" GOTO error

SET "DESTINATION=staging/ArduinoCore-%~1"
copy boards.txt "%DESTINATION%"
copy platform.txt "%DESTINATION%"
copy programmers.txt "%DESTINATION%"

sed -i "s/\[VARIANT\]/%~1/g" "%DESTINATION%/platform.txt"
sed -i "s/\[VERSION\]/%~2/g" "%DESTINATION%/platform.txt"
GOTO end

:error
echo.
echo error: missing parameters
echo.
echo usage: stage_specifications variant version
echo.
echo valid variants are:
echo     nm180100evb
echo.
echo.

:end
