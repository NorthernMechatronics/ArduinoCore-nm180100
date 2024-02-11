@echo off
IF "%~1" == "" GOTO error
IF "%~2" == "" GOTO error


SET "DESTINATION=staging/ArduinoCore-nm180100"
robocopy cores "%DESTINATION%/cores" stub.c *.h *.a /e
robocopy "libraries" "%DESTINATION%/libraries" library.properties *.h *.a *.ino /e
robocopy "variants" "%DESTINATION%/variants" AMA3B1KK.ld loadscript *.h *.c *.cpp *.a /e

copy boards.txt "%DESTINATION%"
copy platform.txt "%DESTINATION%"
copy programmers.txt "%DESTINATION%"

sed -i "s/\[VERSION\]/%~1/g" "%DESTINATION%/platform.txt"

SET "FILENAME=%DESTINATION%-%~1.tar.bz2"
for /f "delims= " %%i in ('basename %FILENAME%') do set BASENAME=%%i
for /f "delims= " %%i in ('basename %DESTINATION%') do set BASEFOLDER=%%i
SET "VERSION=%~1"

cd staging
tar --exclude='*.git' --exclude='*.github' -cjhf "%BASENAME%" "%BASEFOLDER%"
for /f "delims= " %%i in ('sha256sum %BASENAME%') do set CHECKSUM=%%i
for /f "delims= " %%i in ('wc -c %BASENAME%') do set SIZE=%%i
cd ..

SET "JSON_FILE=staging/package_nm180100_index.json"
copy extras\package_index_template.json "%JSON_FILE%"
sed -i "s/%%%%VERSION%%%%/%~1/g" "%JSON_FILE%"
sed -i "s#%%%%URL%%%%#%~2#g" "%JSON_FILE%"
sed -i "s/%%%%ARCHIVENAME%%%%/%BASENAME%/g" "%JSON_FILE%"
sed -i "s/%%%%CHECKSUM%%%%/%CHECKSUM%/g" "%JSON_FILE%"
sed -i "s/%%%%SIZE%%%%/%SIZE%/g" "%JSON_FILE%"

GOTO end

:error
echo usage: release_package version url
echo.
echo.

:end
