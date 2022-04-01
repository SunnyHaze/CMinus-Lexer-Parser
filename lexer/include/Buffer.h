#pragma once
#include<iostream>
#include<fstream>
#include<cstring>
extern const int BUFFSIZE; // 最大缓冲区长度
extern int bufflen;
extern int lineno;         // 当前行号 
extern int linepos;        // 当前字符号 
extern char linebuf[];
extern int EOF_flag;       //文件尾


extern std::string inputPath, outputPath;
extern std::ifstream ifs;
extern std::ofstream ofs;

//设置存取路径
void setPath(const char * i, const char * o);
void setFileStream();
int getNextChar();
void ungetNextChar(void);