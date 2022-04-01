@echo off
cd build
cmake .. -G "MinGW Makefiles"
echo ===================CMAKE DONE===========================
mingw32-make.exe
echo ====================MAKE DONE===========================
cd ..
echo ====================Run Code============================
.\CMinusLexer_binary.exe
