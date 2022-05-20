#include <iostream>
#include <fstream>
#include "TokenType.h"
#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "parser/abstract_tree.h"
// ==========全局参数定义============
std::string input = "";
// outpur路径为空，则输出到标准输出，如果不为空，则输出到文件
// std::string output = "./test_cases/output.txt";
std::string output = "";
cmlexer lex; // 初始化语法解析器类
cmparser myparser(&lex);
// ====================================

// 初始化输入输出文件路径
void initPath(){
    lex.setPath(input,output);
}
int main(int argc, char* argv[]){
    input.assign("./test_cases/input.c");
    initPath();
    auto res = myparser.parse();
    if(res !=nullptr)   res->show();
    return 0;
}