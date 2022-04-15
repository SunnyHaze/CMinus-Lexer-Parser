#include "Lexer.h"

std::set<char> oper_start = {
    '+', '-', '*', '/', '<', '>', '=', '!', '=',';' , ',', '(', ')', '[', ']', '{', '}'
};
bool isoperator(char c){
    return oper_start.count(c);
}
std::unordered_map<state, std::string> error_map = {
    {state::unexpected_char, "未知的字符！"},
    {state::undefined_operator, "无法识别的运算符!"},
    {state::unexpected_state, "状态机异常，请联系作者debug！"}
};
std::string get_error_str(state s){
    if(error_map.count(s)){
        return error_map.at(s);
    }else{
        return "<invalid error>";
    }
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

state cmlexer::read_next(char c, bool next){
    if(next){   //后移一位
        linepos++;  // 加位置
    }
    switch (_s){ // todo :  innumb
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
        }else{
            return _s = state::unexpected_char;
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
                return _s = state::output;
            }
        }else{  // 新读进来的也是运算符 【x=-1】【y<=1】
            auto oper = string2operator(buffer);
            if(oper != operator_type::_null && string2operator(buffer + c) == operator_type::_null){
                buffer.clear();
                results.emplace_back(new token_operator(oper, lineno, linepos - 1));
                return _s = state::output;
            }else if (oper != operator_type::_null && string2operator( buffer + c) != operator_type::_null){
                if(operator_type::_comment == (string2operator(buffer + c))){
                    buffer.clear();
                    return _s = state::in_comm;
                }
            }
            buffer += c;
            return _s;      // 因为下一个还是运算符，保持运算符不变
        }

    }
    case state::in_comm:{   
        if(c == '\n'){
            linepos = 0;
            lineno++;
            return _s;
        }else if(c == '*'){
            return _s = state::ex_comm;
        }else{
            return _s = state::in_comm;
        }
    }
    case state::ex_comm:{
        if(c == '/'){
            return _s = state::start;
        }else if (c == '\n'){
            linepos = 0;
            lineno++;
        }
        return _s = state::in_comm;
    }
    case state::in_iden:{
        if(isalpha(c)){
            buffer += c;
            return _s;
        }else{ // 这里需要先检测是否是关键字
            auto key_type = string2keyword(buffer);
            if(key_type == keyword_type::_null){
                // 说明不是关键字，按照标识符处理     
                results.emplace_back(new token_identifier(buffer, lineno, linepos - 1));
            }
            else{
                // 按照关键字处理
                results.emplace_back(new token_keyword(key_type, lineno, linepos - 1));
            }
            buffer.clear();
            return _s = state::output;
          
        }
    }
    case state::in_numb:{
        if(isdigit(c)){
            buffer += c;
            return _s;
        }else{
            results.emplace_back(new token_number(buffer, lineno, linepos - 1));
            buffer.clear();
            return _s = state::output;
        }
    }
    default:
        return _s = state::unexpected_state;
    }
}
token_base * cmlexer::get_next_token(){
    return get_next_token(ifs);
}
token_base * cmlexer::get_next_token(std::ifstream &local_ifs){
    do{
        if(is_eof){
            return nullptr;
        }else{
            if(line_idx >= line_buff.size()){
                if(!std::getline(local_ifs, line_buff)){
                    is_eof = true;
                }
                line_idx = 0;
                line_buff += "\n";
                // std::cout << line_buff;
            }
            read_next(line_buff[line_idx], next);
            if(!next){
                next = 1;
            }
            if(error_state()){
                std::cout << "ERROR :" << get_error_str(_s) << std::endl;
                for (char &ch : line_buff) if (ch == '\t') ch = ' ';
				std::cout << ' ' << line_buff << std::flush;
				std::cout << ' ' << std::string(get_pos() - 1, ' ') << "^" << std::endl;
                std::cout << " " << "In the line " << get_lineno() << ", position " << get_pos() << std::endl << std::endl; reset_status();
                return nullptr;
            }
            if(_s == state::output){
                next = 0;
                return this->get_result();
            }
            line_idx++;
        }
    }while(1);
}

void cmlexer::lexing_file(){
    lexing_file(ifs);
}

void cmlexer::lexing_file(std::ifstream &local_ifs){
    token_base* res;
    while((res = get_next_token()) != nullptr){
        if(if_std_output)
            std::cout <<'#'<< res->get_line() << "\t" <<res->get_pos() << '\t' << res->to_string() << std::endl;
    }
}