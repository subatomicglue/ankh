@echo "running environment"
call vc-env.bat

@echo "Making Ankh"
cd ..\src\ankh
make clean
