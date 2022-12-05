@echo off
call build_core.bat
if not exist %BuildDir% mkdir %BuildDir%
pushd %BuildDir%
del *.exe
del *.pdb
del *.ilk
del *.obj
cl.exe %MSVC_CommonCompiler% %BuildDir%\..\shared_scratch.c

popd

