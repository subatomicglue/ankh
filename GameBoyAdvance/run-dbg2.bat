
set PATH=c:\cygwin\bin;%PATH%

cd ..\..\devkitadv-r5-beta-3
call cmd-rc.bat


pwd
cd ..\src\ankh
VisualBoyAdvance-SDL.exe -Gtcp -2 ankh.gba
