@echo off
IF "%~1" == "" GOTO error

SET "DESTINATION=staging/ArduinoCore-%~1"
robocopy cores "%DESTINATION%/cores" *.h *.a /e
robocopy "libraries" "%DESTINATION%/libraries" *.h *.a /e
robocopy "variants/%~1" "%DESTINATION%/%~1" *.h *.a /e
GOTO end

:error
echo.
echo error: missing variant specification
echo.
echo usage: stage_content variant
echo.
echo valid variants are:
echo     nm180100evb
echo.
echo.

:end
