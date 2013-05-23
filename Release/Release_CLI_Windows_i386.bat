@rem Copyright (c) MediaArea.net SARL. All Rights Reserved.
@rem
@rem Use of this source code is governed by a BSD-style license that can
@rem be found in the License.html file in the root of the source tree.
@rem

@rem echo off

@rem --- Clean up ---
del   MediaInfo_CLI_Windows_i386.zip
rmdir MediaInfo_CLI_Windows_i386 /S /Q
mkdir MediaInfo_CLI_Windows_i386

rem --- Copying : Exe ---
xcopy ..\Project\MSVC2010\CLI\Win32\Release\MediaInfo.exe MediaInfo_CLI_Windows_i386\ /S
xcopy ..\..\MediaInfoLib\Project\MSVC2005\DLL\Win32\Release\MediaInfo.dll MediaInfo_CLI_Windows_i386\ /S

rem --- Copying : Plugins ---
xcopy ..\Source\Resource\Plugin\Custom\* MediaInfo_CLI_Windows_i386\Plugin\Custom\ /S

rem --- Copying : Information files ---
copy ..\License.* MediaInfo_CLI_Windows_i386\
copy ..\History_CLI.txt MediaInfo_CLI_Windows_i386\History.txt
copy Readme_CLI_Windows.txt MediaInfo_CLI_Windows_i386\ReadMe.txt

rem --- Copying : Contrib files ---
mkdir MediaInfo_CLI_Windows_i386\Contrib
copy ..\Contrib\CLI_Help.doc MediaInfo_CLI_Windows_i386\Contrib\

rem --- Compressing Archive ---
cd MediaInfo_CLI_Windows_i386\
..\..\..\Shared\Binary\Windows_i386\7-Zip\7z a -r -tzip ..\MediaInfo_CLI_Windows_i386.zip *
cd ..


rem --- Clean up ---
if "%1"=="SkipCleanUp" goto SkipCleanUp
rmdir MediaInfo_CLI_Windows_i386\ /S /Q
:SkipCleanUp
