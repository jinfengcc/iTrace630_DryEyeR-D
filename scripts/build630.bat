:: echo off
:: ============================================================================
:: Copyright(c) 2020 Tracey Technologies, INC. All Rights Reserved.
::
:: ============================================================================
setlocal

:: capture present working directory

set PWD=%cd%
set TMP_DIR=__build_tmp__6_30
set GIT=https://github.com/traceytech-code/iTrace6xx.git
set BRANCH=master
set SOLUTION=%PWD%\%TMP_DIR%\iTrace6xx.sln
set CSPROJ=%PWD%\%TMP_DIR%\tools\TraceyRzdcxActivation\TraceyRzdcxActivation.csproj

:: Visual studio
set VSE="C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\Common7\Tools\VsDevCmd.bat"

call %VSE%

:: Cleanup temp directory

if exist %PWD%\%TMP_DIR% (
	echo %TMP_DIR% exists, performing cleanup ...
   rd /S /Q %PWD%\%TMP_DIR%
)

:: clone folder

git clone %GIT% %TMP_DIR% --branch=%BRANCH% --depth=1

:: Build, etc
cd %TMP_DIR%

MSBuild "%CSPROJ%" -p:Configuration=Release
MSBuild "%SOLUTION%" -p:Configuration=Release -p:Platform=x86 -target:iTrace

:: Cleanup (remove .get folder)

cd %PWD%
rd /S /Q %PWD%\%TMP_DIR%\.git

endlocal