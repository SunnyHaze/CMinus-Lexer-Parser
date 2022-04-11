@echo off
cd build
cmake .. -G "MinGW Makefiles"
echo ===================CMAKE DONE===========================
mingw32-make.exe
echo ====================MAKE DONE===========================
cd ..
echo ====================Run Code============================
.\CMinusLexer_binary.exe  ./test_cases/input.c ./test_cases/output.txt
