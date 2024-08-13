@echo off

mkdir ..\..\build
pushd ..\..\build
cl -FC -Zi ..\mite\code\win32_handmade2.cpp user32.lib gdi32.lib
popd

