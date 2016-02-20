set %REL_VERSION=0.0.1
set %REL_NAME=townScan
set %ARCH=

mkdir %REL_NAME%%REL_VERSION%%ARCH%

xcopy /E /I ..\bin %REL_NAME%%REL_VERSION%%ARCH%\bin
xcopy /E /I ..\src %REL_NAME%%REL_VERSION%%ARCH%\src

7za.exe a -r %REL_NAME%%REL_VERSION%%ARCH%.zip %REL_NAME%%REL_VERSION%%ARCH%

rmdir /S /Q %REL_NAME%%REL_VERSION%%ARCH%

pause