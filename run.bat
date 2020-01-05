@echo off
pushd %~dp0.\
call bin\windows-Release-x86_64\particles-simulator\particles-simulator.exe %*
popd
PAUSE