@echo off

rem Create the build directory if it doesn't exist
if not exist ..\..\build (
    mkdir ..\..\build
)
rem Change the current directory to the build directory
pushd ..\..\build

rem Compile the application
cl -FC -Zi ..\mite\code\win32_maninthebox.cpp user32.lib gdi32.lib /I"z:\externallib\SDL2\include" /link /LIBPATH:"z:\externallib\SDL2\lib\x64" SDL2.lib SDL2main.lib /OUT:win32_maninthebox.exe

rem Ensure SDL2.dll is copied to the build directory
copy /Y "z:\externallib\SDL2\lib\x64\SDL2.dll" .

rem Copy the controller_mappings.txt to the build directory
copy /Y "..\mite\code\controller_mappings.txt" .

popd

