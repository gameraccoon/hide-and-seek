@echo off
set scriptDir=%CD%
cd ..
mkdir vs_project
cd vs_project
cmake build .. -G "Visual Studio 15 2017"
cd %scriptDir%