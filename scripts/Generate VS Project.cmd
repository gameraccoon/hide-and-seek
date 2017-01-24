@echo off
set scriptDir=%CD%
cd ..
mkdir vs_project
cd vs_project
cmake build .. -G "Visual Studio 14 2015 Win64" 
cd %scriptDir%