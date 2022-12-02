@echo off

set BuildDir=%~dp0build
set MSVC_CommonCompiler=-I%BuildDir%\..\shared_code -DAOC_BUILD_EXE -FC -Zo -Zi -GS- -W3 -WX -nologo -O2 -std:c11 ntdll.lib
echo Building from %BuildDir%
echo Compilation Line: %MSVC_CommonCompiler%

