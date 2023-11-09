@echo off

cd src

rem Build the program using the Makefile
mingw32-make

rem Check if the build process was successful
if %errorlevel% neq 0 (
    echo Build failed
    pause
    exit /b %errorlevel%
)

rem Run the program
.\Paredao

rem After the program exits, run 'make clean'
mingw32-make clean