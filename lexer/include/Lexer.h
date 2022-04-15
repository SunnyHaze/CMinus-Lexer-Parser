#pragma once
#include<iostream>
#include<fstream>
#include<cstring>
#include<vector>
#include<cctype>
#include<unordered_map>
#include<set>
#include "TokenType.h"
// 所有可能的运算符起始符号
extern std::set<char> oper_start;
bool isoperator(char c);

// 词法分析器的状态机的状态
enum class state{
    start,   // 普通状态 0
    output,  // 可以输出一个词法单元的状态 1
    in_oper, // 在操作符状态 2
    in_comm, // 在注释状态 3
    in_numb, // 在数值状态 4
    in_iden, // 在标识符状态 5
    ex_comm, // 即将退出注释 6
    unexpected_char, // 7 未知的字符
    undefined_operator, // 8 未知的运算符
    unexpected_state    // 异常状态转换
};

// 用来管理错误类型的
extern std::unordered_map<state, std::string> error_map;
std::string get_error_str(state s);

// 主要的词法分析器类
class cmlexer{
    int bufflen = 0;
    int lineno = 1;         // 当前行号 
    int linepos = 0;        // 当前字符号
    
    std::string buffer;     // 当前token的缓冲区
    std::string line_buff;  // 当前行缓冲区
    size_t line_idx = 0;   // 当前行索引
    bool is_eof = false;
    // 在output状态之后，需要通过这个状态量来确保能重新读入上一个仅仅用来判断“状态”但没读入缓冲区的变量
    bool next = 1;          
    state _s; //当前状态

    std::string inputPath, outputPath;
public:
    // 按顺序保存结果的指针数组
    std::vector<token_base*> results;
    std::ifstream ifs;
    std::ofstream ofs;
    bool if_std_output = true;    // 是否在标准输出输出词法分析结果
    bool output_redirect = false; // 是否重定向输出，如果没有则采用标准输入输出

    //设置存取路径
    void setPath(std::string i, std::string o);
    int getNextChar();
    void ungetNextChar();
    // 
    state read_next(char c, bool next);

    //获取下一个token元素
    token_base * get_next_token();
    token_base * get_next_token(std::ifstream &local_ifs);

    // 从输入流解析整个文件的函数，无参数为默认按照ifs读取
    void lexing_file(std::ifstream &ifstream); 
    void lexing_file();
    // 获得当前最新的头部token
    token_base* get_result(){
        _s = state::start;
        return results.back();
    }
    std::vector<token_base*>* get_results(){
        return &results;
    }
    inline std::string get_buffer(){
        return buffer;
    }
    inline int get_lineno(){
        return lineno;
    }
    inline int get_pos(){
        return linepos;
    }
    // 恢复初始状态
    void reset_status(){
        _s = state::start;
        buffer.clear();
        is_eof = 0;
    }
    // 查看是否错误
    bool error_state(){
        if(_s >= state::unexpected_char)
            return 1;
        else
            return 0;
    }
};