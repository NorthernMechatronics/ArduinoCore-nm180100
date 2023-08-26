@echo off
IF "%~1" == "" GOTO error
IF "%~2" == "" GOTO error
IF "%~3" == "" GOTO error


SET "DESTINATION=staging/ArduinoCore-%~1"
robocopy cores "%DESTINATION%/cores" *.h *.a /e
robocopy "libraries" "%DESTINATION%/libraries" *.h *.a /e
robocopy "variants/%~1" "%DESTINATION%/%~1" *.h *.a /e

copy boards.txt "%DESTINATION%"
copy platform.txt "%DESTINATION%"
copy programmers.txt "%DESTINATION%"

sed -i "s/\[VARIANT\]/%~1/g" "%DESTINATION%/platform.txt"
sed -i "s/\[VERSION\]/%~2/g" "%DESTINATION%/platform.txt"

SET "FILENAME=%DESTINATION%-%~2.tar.bz2"
SET "VERSION=%~2"
tar --exclude='*.git' --exclude='*.github' -cjhf "%FILENAME%" "%DESTINATION%"
for /f "delims= " %%i in ('sha256sum %FILENAME%') do set CHECKSUM=%%i
for /f "delims= " %%i in ('wc -c %FILENAME%') do set SIZE=%%i
for /f "delims= " %%i in ('basename %FILENAME%') do set BASENAME=%%i

SET "JSON_FILE=staging/package_%~1_%~2_index.json"
copy extras\package_index_template.json "%JSON_FILE%"
sed -i "s/%%%%VARIANT%%%%/%~1/g" "%JSON_FILE%"
sed -i "s/%%%%VERSION%%%%/%~2/g" "%JSON_FILE%"
sed -i "s/%%%%URL%%%%/%~3/g" "%JSON_FILE%"
sed -i "s/%%%%ARCHIVENAME%%%%/%BASENAME%/g" "%JSON_FILE%"
sed -i "s/%%%%CHECKSUM%%%%/%CHECKSUM%/g" "%JSON_FILE%"
sed -i "s/%%%%SIZE%%%%/%SIZE%/g" "%JSON_FILE%"

GOTO end

:error
echo usage: release_package variant version url
echo.
echo valid variants are:
echo     nm180100evb
echo.
echo.

:end
