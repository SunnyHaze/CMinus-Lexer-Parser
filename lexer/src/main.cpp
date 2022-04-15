#include <iostream>
#include <fstream>
#include "TokenType.h"
#include "Lexer.h"

// ==========全局参数定义============
std::string input = "";
// outpur路径为空，则输出到标准输出，如果不为空，则输出到文件
// std::string output = "./test_cases/output.txt";
std::string output = "";
cmlexer lex; // 初始化语法解析器类
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
        for(int i=0 ; i < str.size(); ){
            // printf("%c\n",str[i]);
            auto s = lex.read_next(str[i], next);
            // std::cout <<"Status: " <<(int) s << std::endl;
            if(!next){
                next = 1;
            }
            if(s == state::output){
                auto res = lex.get_result();
                std::cout <<'#'<< res->get_line() << "\t" <<res->get_pos() << '\t' << res->to_string() << std::endl;
                next = false;
                continue;
            }
            i++; // next为false（刚进行output后则加加）
        }
    }
    // // 带行数输出原始代码——测试buffer
    // void showInputFile(){
    //     char c;
    //     while((c = lex.getNextChar()) != EOF){
    //         std::cout << c;
    //     }
    // }
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
    // 测试运算符解析是否正确的部分
    void operatorLexer(){
        std::string buf;
        bool next = 1;
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
    lex.if_std_output = true; // 是否在标准输出展示结果
    // 根据输入数据的个数来判定操作
    switch (argc)
    {
    case 1: // 解析
        std::cout << "ERROR:缺少参数，请输入需要解析的文件路径！"<< std::endl;
        break;
    case 2: // 如果有一个参数，则从文件路径读入，并从标准输出输出结果
        input.assign(argv[1]);
        initPath();
        lex.lexing_file();
        break;
    case 3:
        input.assign(argv[1]);
        output.assign(argv[2]);
        initPath();
        lex.lexing_file();
        break;
    default:
        std::cout << "ERROR:参数过多，请确认是否只有'输入路径'、'输出路径'两个参数。"<< std::endl;
        break;
    }
    // keyword_type key;
    // key = string2keyword("if");
    // std::cout << keyword2string(key) << std::endl;
 
    // tests::showInputFile();
    // tests::showTokens();
    // tests::operatorLexer();
    // lex.lexing_file(); // 默认按照内部的ifs读取数据

    // 最后一总从信息中输出
    // for(auto i : lex.results){
    //     std::cout << '#' << i->get_line() << "\t" << i->get_pos() << "\t" << i->to_string() << std::endl;
    // }

    return 0;
}