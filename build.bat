@echo off

set CommonCompilerFlags=-MT -nologo -Zi -FC -EHa -Od -Oi -W3 -std:c++17

set CommonLinkerFlags=

set dataDeskDir=%cd%\data_desk
set dataDeskCustomDir=%cd%\data_desk_custom
set buildDir=%cd%\build
set srcDir=%cd%\src


IF NOT EXIST %dataDeskDir% mkdir %dataDeskDir%
IF NOT EXIST %dataDeskCustomDir% mkdir %dataDeskCustomDir%
IF NOT EXIST  %srcDir% mkdir %srcDir%
IF NOT EXIST  %srcDir%\generated mkdir %srcDir%\generated
IF NOT EXIST  %buildDir% mkdir %buildDir%

echo ========Build Data Desk========
pushd %dataDeskDir%
call build_msvc.bat
popd

echo ========Build Data Desk Custom========
pushd %dataDeskCustomDir%
cl -nologo -FC -LD -TC .\data_desk_custom.c
popd

echo ========Run Data Desk========
pushd %srcDir%
call %dataDeskDir%\build\data_desk.exe --custom %dataDeskCustomDir%\data_desk_custom.dll .\ComponentEnums.ds
popd


pushd %buildDir%
echo ========Compile Code========
REM 64-bit build
cl %CommonCompilerFlags% -MP ..\src\TheGreatMachine.cpp ..\src\pge\*.cpp ..\src\imgui\*.cpp /link %CommonLinkerFlags%
popd