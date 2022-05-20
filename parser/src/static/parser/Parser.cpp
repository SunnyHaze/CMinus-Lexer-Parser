#include "parser/Parser.h"
// #include "abstract_tree.h"

token_base * cmparser::get_next_token(){
    current_token = lexer->get_next_token();
    if(current_token != nullptr){
        std::cout << current_token->to_string() << std::endl;
    }
    return current_token;
}

token_base * cmparser::get_current_token(){
    return current_token;
}
token_type cmparser::get_current_token_type(){
    return current_token->get_token_type();
}
token_keyword* cmparser::get_current_keyword(){
    if(current_token==nullptr)
        return nullptr;
    if(current_token->get_token_type() == token_type::_keyword){
        return (token_keyword *) current_token;
    }else{
        // SyntaxError("token类型不是预期的关键字类型!");
        return nullptr;
    }
}
token_number* cmparser::get_current_number(){
    if(current_token==nullptr)
        return nullptr;
    if(current_token->get_token_type() == token_type::_NUM){
        return (token_number *) current_token;
    }else{
        // SyntaxError("Token类型不是预期的Number类型！");
        return nullptr;
    }
}
token_operator* cmparser::get_current_operator(){
    if(current_token==nullptr)
        return nullptr;
    if(current_token->get_token_type() == token_type::_operator){
        return (token_operator *) current_token;
    }else{
        // SyntaxError("Token类型不是预期的运算符类型！");
        return nullptr;
    }
}
token_identifier* cmparser::get_current_id(){
    if(current_token==nullptr)
        return nullptr;
    if(current_token->get_token_type() == token_type::_ID){
        return (token_identifier *) current_token;
    }else{
        // SyntaxError("Token类型不是预期的标识符类型！");
        return nullptr;
    }
}    

void cmparser::SyntaxError(std::string error_string){
    std:: cout << ">>> ";
    std::cout << "SyntaxError:\033[1;31;40m" << error_string << "\033[0m at line: \033[1;32;40m" << current_token->get_line() << "\033[0m pos: \033[1;32;40m" << current_token->get_pos() << "\033[0m" << std::endl;
    error_state = 1; // 进入错误状态
    throw -1;
}

bool cmparser::match_keyword(keyword_type t){
    // 匹配成功，读入下一个字符
    if(get_current_token()->get_token_type() == token_type::_keyword && ((token_keyword*) current_token)->get_type() == t){
        get_next_token();
        return 1;
    }
    else{
        std::cout << ">>>Error: Unexpected token type -->  " << get_current_token()->to_string() << std::endl;
        error_state = 1;
        return 0;
    }
}
bool cmparser::match_operator(operator_type t){
    // 匹配成功，读入下一个字符
    if(get_current_token()->get_token_type() == token_type::_operator && ((token_operator*) current_token)->get_operator_type() == t){
        get_next_token();
        return 1;
    }
    else{
        std::cout << ">>>Error: Unexpected token type -->  " << get_current_token()->to_string() << std::endl;
        error_state = 1;
        return 0;
    }
}