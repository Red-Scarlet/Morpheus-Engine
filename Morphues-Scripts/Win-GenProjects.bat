@echo off
pushd ..\
call Morphues-Vendor\premake\premake5.exe vs2019
popd

IF %ERRORLEVEL% NEQ 0 (
	PAUSE
)