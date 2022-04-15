#include "TokenType.h"
#include <unordered_map>
#include <string>
const int MAX_TOKEN_SIZE = 256;
char TokenString[MAX_TOKEN_SIZE + 1];
int TokenIndex = 0;
// 定义转换表类型模板
template<typename _kT, typename _vT> using map_t = std::unordered_map<_kT, _vT>;
// 定义散列表，便于快速转换
map_t<std::string, keyword_type> keyword_map = {
    {"if",keyword_type::_if},
    {"else", keyword_type::_else},
    {"int", keyword_type::_int},
    {"return", keyword_type::_return},
    {"void", keyword_type::_void},
    {"while", keyword_type::_while}
};

map_t<std::string, operator_type> operator_map = {
    {"/*", operator_type::_comment},
    {"+", operator_type::_add},
    {"-", operator_type::_sub},
    {"*", operator_type::_mul},
    {"/", operator_type::_div},
    {"<", operator_type::_les},
    {"<=", operator_type::_leq},
    {">", operator_type::_gre},
    {">=", operator_type::_geq},
    {"==", operator_type::_equ},
    {"!=", operator_type::_neq},
    {"=", operator_type::_asi},
    {";", operator_type::_sem},
    {",", operator_type::_com},
    {"(", operator_type::_slb},
    {")", operator_type::_srb},
    {"[", operator_type::_mlb},
    {"]", operator_type::_mrb},
    {"{", operator_type::_llb},
    {"}", operator_type::_lrb}
};
// keyword子类型的转换函数
keyword_type string2keyword(std::string str){
    if(keyword_map.count(str)){
        return keyword_map.at(str);
    }
    else{
        return keyword_type::_null;
    }
}
std::string keyword2string(keyword_type t){
    switch (t)
    {
    case keyword_type::_if:
        return "if";
    case keyword_type::_else:
        return "else";
    case keyword_type::_int:
        return "int";
    case keyword_type::_return:
        return "return";
    case keyword_type::_void:
        return "void";
    case keyword_type::_while:
        return "while";
    default:
        return "<invalid token>";
        break;
    }
}

// operator子类型的转换函数
operator_type string2operator(std::string str){
    if(operator_map.count(str)){
        return operator_map.at(str);
    }
    else{
        return operator_type::_null;
    }
}
std::string operator2string(operator_type o){
    switch (o)
    {
    case operator_type::_add:
        return "+";
    case operator_type::_sub:
        return "-";
    case operator_type::_mul:
        return "*";
    case operator_type::_div:
        return "/";
    case operator_type::_les:
        return "<";
    case operator_type::_leq:
        return "<=";
    case operator_type::_gre:
        return ">";
    case operator_type::_geq:
        return ">=";
    case operator_type::_equ:
        return "==";
    case operator_type::_neq:
        return "!=";
    case operator_type::_asi:
        return "=";
    case operator_type::_sem:
        return ";";
    case operator_type::_com:
        return ",";
    case operator_type::_slb:
        return "(";
    case operator_type::_srb:
        return ")";
    case operator_type::_mlb:
        return "[";
    case operator_type::_mrb:
        return "]";
    case operator_type::_llb:
        return "{";
    case operator_type::_lrb:
        return "}";   
    default:
        return "<invalid token>";
        break;
    }
}

