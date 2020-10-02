
set PATH=c:\cygwin\bin;%PATH%

cd ..\..\devkitadv-r5-beta-3
call cmd-rc.bat


pwd
cd ..\src\ankh
arm-agb-elf-insight.exe ankh.elf



