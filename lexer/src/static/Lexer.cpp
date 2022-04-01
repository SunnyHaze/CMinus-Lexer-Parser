#include "Lexer.h"

std::set<char> oper_start = {
    '+', '-', '*', '/', '<', '>', '=', '!', '=',';' , ',', '(', ')', '[', ']', '{', '}'
};
bool isoperator(char c){
    return oper_start.count(c);
}

// 设置输入输出的路径
void cmlexer::setPath(std::string i, std::string o){
    ifs.open(i);
    if(o != ""){
        output_redirect = true;
        ofs.open(o);
        std::cout.rdbuf(ofs.rdbuf());
    }
}
    // 从文件中获取下一个字符
int  cmlexer::getNextChar(){
    if(linepos >= bufflen){ // 读取新一行
        lineno++;
        if(output_redirect){
            ofs << "#" << lineno << "\t";
        }else{
            std::cout << "#" << lineno << "\t";
        }
        
        if(std::getline(ifs, buffer)){ // 能读取到新一行
            buffer += '\n';
            bufflen = buffer.size();
            linepos = 0;
        }else{       
            EOF_flag = 1;           // 读不到了，结束
            return EOF;
        }
    }
    return buffer[linepos++];
}
//Todo 功能重构


// 回退一个字符 (没有处理段首回退到上一行的问题，不过在此项目中不太影响)
void cmlexer::ungetNextChar(void){
    if (!EOF_flag) linepos-- ;
}

state cmlexer::read_next(char c, bool next){
    if(next){   //后移一位
        linepos++;  // 加位置
    }
    switch (_s){
    case state::start :{
        if(c == '\0') {
            return _s;
        }
        else if(c == '\n'){
            linepos = 0;
            lineno++;   // 加行数
            return _s;
        }
        else if(isspace(c)){
            return _s;
        }
        else if(c == '\r'){
            return _s;
        }
        else if(isdigit(c)){
            buffer += c;
            return _s = state::in_numb;
        }
        else if(isoperator(c)){
            buffer += c;
            return _s = state::in_oper;
        }
        else if(isalpha(c)){
            buffer += c;
            return _s = state::in_iden;
        }else if (c == EOF){
            return _s = state::output;
        }
    }
    case state::in_oper :{
        if(!isoperator(c)){ // 新读进来的不是运算符
            auto oper = string2operator(buffer);
            buffer.clear();
            if(oper == operator_type::_null){ // 不存在
                return _s = state::undefined_operator;
            }           
            else if(oper == operator_type::_comment){ // 如果是注释则进入注释状态
                return _s = state::in_comm;
            }else{  // 其他状态则正常输出
                results.emplace_back(new token_operator(oper, lineno, linepos - 1));
                return state::output;
            }
        }else{  // 新读进来的也是运算符 【x=-1】【y<=1】
            auto oper = string2operator(buffer);
            if(oper != operator_type::_null && string2operator(buffer + c) == operator_type::_null){
                buffer.clear();
                if(oper == operator_type::_comment){ //解决 【/***】这种注释的问题
                    return state::in_comm;
                }else{
                    results.emplace_back(new token_operator(oper, lineno, linepos - 1));
                }
            }
            buffer += c;
            return _s;      // 因为下一个还是运算符，保持运算符不变
        }
    }

    
    default:
        return state::unexpected_char; //todo
        break;
    }
}