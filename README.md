# CMinus 编译器构建
- 本仓库为四川大学编译原理课程设计仓库
- 目标为完成CMinus词法分析器和语法分析器，本质上是C语言的一个子集，功能较为简单，便于本科生快速实现理解原理。其[具体词法语法规则参见此链接](documents/CMinus词法和语法规则.md)
- 仓库内容按模块分为`lexer`和`parser`的子文件夹，实现了对应的功能。
- 本项目**仍在开发中**...
  - 目前已经实现了`lexer`（词法分析器）的功能
  - 下个月会慢慢实现`parser`（语法分析器）的功能
## CMAKE编译指令
本项目采用`CMake`构建，是C/C++工程开发目前最常见的集成工具之一，如果你不熟悉CMAKE，可以参考如下两个仓库学习如何使用CMAKE编译C/C++工程文件：

[![Readme Card](https://github-readme-stats.vercel.app/api/pin/?username=SFUMECJF&repo=cmake-examples-Chinese)](https://github.com/SFUMECJF/cmake-examples-Chinese)

[![Readme Card](https://github-readme-stats.vercel.app/api/pin/?username=ttroy50&repo=cmake-examples)](https://github.com/ttroy50/cmake-examples)

> 跟着上面的教程走一遍，大概1个多小时能掌握基本的使用方法。而事实上，如果你不想这么麻烦，大致把头文件`*.h`和`*.cpp`文件适当组装到一个文件中也是能正常运行的，不过不便于debug和寻找模块对应代码。

--------
掌握CMAKE之后（或者只打算跑一下demo看看结果），请将路径切换到对应文件夹模块下来编译源代码：
### 直接编译方式：
在目录下提供了`make.bat`文件，在`Windows`环境下，安装过CMAKE之后，可以通过在命令行中批处理来自动化完成编译运行过程。
```powershell
./make.bat
```
### 手动编译方式
- 在build路径中执行如下指令以获得`MinGW Makefiles`(我这里使用minGW编译，如果你有其他的编译工具，请自行选择）：
```
cmake .. -G 'MinGW Makefiles'
```
- 随后在这个文件夹下运行·MinGW·即可开始编译：
```
mingw32-make.exe
``` 
- 在**命令行中运行**根目录下得到的`*.exe`文件即可。
```powershell
./CMinusLexer_binary.exe
```

--------

祝大家学有所成~ :)