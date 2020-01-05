@echo off
REM Requires MSBuild.exe to be accessible in directories from PATH env variable
pushd %~dp0\..\

MSBuild particles-simulator.sln /p:Configuration=Release;Platform=x64 /v:n /nologo

popd
PAUSE