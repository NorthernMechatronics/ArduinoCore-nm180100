@echo off
IF "%~1" == "" GOTO error
IF "%~2" == "" GOTO error

SET "DESTINATION=staging/ArduinoCore-%~1"
SET "FILENAME=%DESTINATION%-%~2.tar.bz2"
SET "VERSION=%~2"
REM tar --exclude='*.git' --exclude='*.github' -cjhf "%FILENAME%" "%DESTINATION%"
for /f "delims= " %%i in ('sha256sum %FILENAME%') do set CHECKSUM=%%i
for /f "delims= " %%i in ('wc -c %FILENAME%') do set SIZE=%%i
for /f "delims= " %%i in ('basename %FILENAME%') do set BASENAME=%%i

SET "JSON_FILE=staging/package_%~1_%~2_index.json"
copy extras\package_index_template.json "%JSON_FILE%"
sed -i "s/%%%%VERSION%%%%/%~2/g" "%JSON_FILE%"
sed -i "s/%%%%ARCHIVENAME%%%%/%BASENAME%/g" "%JSON_FILE%"
sed -i "s/%%%%CHECKSUM%%%%/%CHECKSUM%/g" "%JSON_FILE%"
sed -i "s/%%%%SIZE%%%%/%SIZE%/g" "%JSON_FILE%"

GOTO end

:error
echo.
echo error: missing parameters
echo.
echo usage: package variant version
echo.
echo valid variants are:
echo     nm180100evb
echo.
echo.

:end
