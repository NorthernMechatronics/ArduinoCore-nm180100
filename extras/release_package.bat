@echo off
IF "%~1" == "" GOTO error
IF "%~2" == "" GOTO error
IF "%~3" == "" GOTO error


SET "DESTINATION=staging/ArduinoCore-%~1"
robocopy cores "%DESTINATION%/cores" stub.c *.h *.a /e
robocopy "libraries" "%DESTINATION%/libraries" library.properties *.h *.a /e
robocopy "variants" "%DESTINATION%/variants" AMA3B1KK.ld loadscript *.h *.c *.cpp *.a /e

copy boards.txt "%DESTINATION%"
copy platform.txt "%DESTINATION%"
copy programmers.txt "%DESTINATION%"

sed -i "s/\[VERSION\]/%~2/g" "%DESTINATION%/platform.txt"

SET "FILENAME=%DESTINATION%-%~2.tar.bz2"
for /f "delims= " %%i in ('basename %FILENAME%') do set BASENAME=%%i
for /f "delims= " %%i in ('basename %DESTINATION%') do set BASEFOLDER=%%i
SET "VERSION=%~2"

cd staging
tar --exclude='*.git' --exclude='*.github' -cjhf "%BASENAME%" "%BASEFOLDER%"
for /f "delims= " %%i in ('sha256sum %BASENAME%') do set CHECKSUM=%%i
for /f "delims= " %%i in ('wc -c %BASENAME%') do set SIZE=%%i
cd ..

SET "JSON_FILE=staging/package_%~1_index.json"
copy extras\package_index_template.json "%JSON_FILE%"
sed -i "s/%%%%VARIANT%%%%/%~1/g" "%JSON_FILE%"
sed -i "s/%%%%VERSION%%%%/%~2/g" "%JSON_FILE%"
sed -i "s#%%%%URL%%%%#%~3#g" "%JSON_FILE%"
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
