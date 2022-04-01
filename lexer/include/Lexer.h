#pragma once
#include<iostream>
#include<fstream>
#include<cstring>
#include<vector>
#include<cctype>
#include<set>
#include "TokenType.h"

extern std::set<char> oper_start;
bool isoperator(char c);

// 记录当前状态
enum class state{
    start,   // 普通状态
    output,  // 可以输出一个词法单元的状态
    in_oper, // 在操作符状态
    in_comm, // 在注释状态
    in_numb, // 在数值状态
    in_iden, // 在标识符状态
    unexpected_char,
    undefined_operator
};

enum class error_state{

};
// 定义用来判定是否是

class cmlexer{
    int bufflen = 0;
    int lineno = 0;         // 当前行号 
    int linepos = 0;        // 当前字符号 
    std::string buffer, last_buffer;
    int EOF_flag;       //文件尾

    state _s; //当前状态

    std::string inputPath, outputPath;

    // 按顺序保存结果的指针数组
    std::vector<token_base*> results;

public:
    std::ifstream ifs;
    std::ofstream ofs;
    bool output_redirect = false; // 是否重定向输出，如果没有则采用标准输入输出

    //设置存取路径
    void setPath(std::string i, std::string o);
    int getNextChar();
    void ungetNextChar();
    state read_next(char c, bool next);
    token_base* get_result(){
        _s = state::start;
        return results.back();
    }
};