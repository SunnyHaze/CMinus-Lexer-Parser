#include <iostream>
#include <fstream>
#include "TokenType.h"
#include "Lexer.h"

// ==========全局参数定义============
std::string input = "./test_cases/operator.cpp";
// 此路径为空，则输出到标准输出，如果不为空，则输出到文件
// std::string output = "./test_cases/output.txt";
std::string output = "";
cmlexer lex;
// ====================================

// 初始化输入输出文件路径
void initPath(){
    lex.setPath(input,output);
}



// 存放测试用的函数
namespace tests 
{
    // 利用简单字符串测试分析器的函数
    void testLexer(std::string str){
        bool next = true;
        for(int i=0 ; i < str.size(); i++){
            // printf("%c\n",str[i]);
            auto s = lex.read_next(str[i], next);
            // std::cout << (int) s;
            if(!next){
                next = 1;
            }
            if(s == state::output){
                auto res = lex.get_result();
                std::cout <<'#'<< res->get_line() << "\t" <<res->get_pos() << '\t' << res->to_string() << std::endl;
            }
        }
    }
    // 带行数输出原始代码——测试buffer
    void showInputFile(){
        char c;
        while((c = lex.getNextChar()) != EOF){
            std::cout << c;
        }
    }
    // 测试所有Token单元是否书写正确
    void showTokens(){
        token_keyword i(keyword_type::_else, 1,4);
        token_operator j(string2operator("+"), 2,3);
        token_identifier k("value", 4, 2);
        token_number l("123",2,4);
        std::cout << "\n";
        std::cout << i.to_string() << "\n";
        std::cout << j.to_string() << std::endl;
        std::cout << k.to_string() << std::endl;
        std::cout << l.to_string() << std::endl;
    }

    void operatorLexer(){
        std::string buf;
        bool next = 1;
        std::ifstream ifs;
        ifs.open("test_cases/operator.cpp");
        std::string a ;
        while(std::getline(lex.ifs, a)){
            a += '\n';
            // std::cout << a << std::endl;
            testLexer(a);
        }

    }
}
// 按行数显示所有的文本内容
int main(int argc, char* argv[]){
    initPath();
    // keyword_type key;
    // key = string2keyword("if");
    // std::cout << keyword2string(key) << std::endl;
 
    // tests::showInputFile();
    // tests::showTokens();

    tests::operatorLexer();
   
    return 0;
}