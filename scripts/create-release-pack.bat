@echo off
REM Requires MSBuild.exe and 7z.exe to be accessible in directories from PATH env variable
REM First cmd argument should be version code; it will be appended at the end of zip file name
pushd %~dp0\..\

MSBuild particles-simulator.sln /p:Configuration=Release;Platform=x64 /v:m /nologo
7z a particles-simulator-%1.zip bin/windows-Release-x86_64/**/*.exe particles-simulator/res/**/* run.bat

popd
PAUSE