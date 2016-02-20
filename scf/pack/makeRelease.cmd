set %REL_VERSION=0.0.2
set %REL_NAME=fieldCode
set %ARCH=win32


mkdir %REL_NAME%%REL_VERSION%%ARCH%

xcopy /E /I ..\bin %REL_NAME%%REL_VERSION%%ARCH%

del %REL_NAME%%REL_VERSION%%ARCH%\*.ilk
del %REL_NAME%%REL_VERSION%%ARCH%\*.pdb

7za.exe a -r %REL_NAME%%REL_VERSION%%ARCH%.zip %REL_NAME%%REL_VERSION%%ARCH%

rmdir /S /Q %REL_NAME%%REL_VERSION%%ARCH%

pause