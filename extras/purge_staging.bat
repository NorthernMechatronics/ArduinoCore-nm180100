@echo off

if exist staging\ (
    for /D %%p in (staging\*.*) do rmdir /S /Q %%p
    del /S /Q staging\*.*
)