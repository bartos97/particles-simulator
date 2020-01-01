@echo off
pushd %~dp0\..\
del /s *.sln *.vcxproj*
rmdir bin\ /s
popd
PAUSE