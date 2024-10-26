@echo off

mkdir ..\build
pushd ..\build
cl -Zi ..\src\win32_entry.cpp user32.lib
popd
