set %REL_VERSION=0.0.2
set %REL_NAME=fieldCodeSrc


mkdir %REL_NAME%%REL_VERSION%

xcopy /E /I ..\bin %REL_NAME%%REL_VERSION%\bin

xcopy /E /I ..\build %REL_NAME%%REL_VERSION%\build
xcopy /E /I ..\docs %REL_NAME%%REL_VERSION%\docs
xcopy /E /I ..\src %REL_NAME%%REL_VERSION%\src

cd %REL_NAME%%REL_VERSION%
del /s *.ncb
del /s *.suo
del /s *.suo
del /s *.user
cd ..

7za.exe a -r %REL_NAME%%REL_VERSION%.zip %REL_NAME%%REL_VERSION%

rmdir /S /Q %REL_NAME%%REL_VERSION%

pause