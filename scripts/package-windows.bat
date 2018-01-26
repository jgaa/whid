
rem On my machine, I execute the build script from a script with these commands:
rem
rem SET QTDIR=C:\Qt\5.10.0\msvc2017_64
rem call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\amd64\vcvars64.bat"
rem PATH=%PATH%;C:\Program Files (x86)\Windows Kits\10\bin\10.0.16299.0\x64
rem cd "C:\Users\Jarle Aase\src\whid\scripts"
rem call .\package-windows

set DIST_DIR=%cd%\dist\windows
set BUILD_DIR=%DIST_DIR%\build
set SRC_DIR=%cd%\..
set OUT_DIR=%DIST_DIR%\whid

rmdir /S /Q "%DIST_DIR%"
mkdir "%DIST_DIR%"
mkdir "%BUILD_DIR%"
mkdir "%OUT_DIR%"

pushd "%BUILD_DIR%"

%QTDIR%\bin\qmake.exe ^
  -spec win32-msvc ^
  "CONFIG += release" ^
  "%SRC_DIR%\whid.pro"

nmake

%QTDIR%\bin\qmake.exe -spec win32-msvc "CONFIG += release" "%SRC_DIR%\whid.pro"

popd

copy "%BUILD_DIR%\release\whid.exe" "%OUT_DIR%"
copy "%SRC_DIR%\res\icons\whid.ico" "%OUT_DIR%"
%QTDIR%\bin\windeployqt --no-compiler-runtime --no-webkit2 --release "%OUT_DIR%\whid.exe"

echo "The prepared package is in: "%OUT_DIR%"
