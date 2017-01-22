@echo off
cd ..
mkdir vs_project
cd vs_project
cmake build .. -G "Visual Studio 14 2015 Win64" 
