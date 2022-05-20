#pragma once
#include "TokenType.h"
#include <iostream>
#include <cstring>
class cmparser;

const int MAXCHILDREN = 3;
enum class Node_type{
    _expK,
    _var,
    _var_declaration,
    _expression,
    _simple_expression,
    _additive_expression,
};
// 区分表达式的类型的枚举类型
// _opK 运算符表达式   
// _constK, 常量表达式    
// _idK, 标识符
enum class ExpKind{
    _assign,     // 赋值语句
    _simple_exp, // 简单表达式
    _add_exp,    // 加法表达式
    _term,       // 乘法项
    _factor,     // 因子
    _var,        // 变量
    _array_var,
    _call,       // 函数调用
    _args,      // 函数形参列表
    _num,       //只含有数值的节点
    _id,        // 只含有ID的节点
    _opK,
    _constK,
    _idK,

};
//抽象语法树的节点类
class TreeNode{
public:
    TreeNode() = default;
    ~TreeNode() = default;
    // 确定是什么类型，表达式 or stmt？
    Node_type _type;
    TreeNode * child[MAXCHILDREN];
    TreeNode * sibling;
    // 具体到节点内的类型、什么表达式或什么stmt
    union {
        ExpKind exp;
    } kind;
    // 如果是Exp
    union {
        operator_type op;
        keyword_type keyword;
        int num;
        char * id;
    } attr;
    // 返回一个表达式节点
    static TreeNode * newExpNode(ExpKind kind){
        TreeNode * p = new TreeNode();
        if(p == nullptr){
            std::cout << "Error: Out of memory!" << std::endl;
            return nullptr;
        }
        p->_type = Node_type::_expK;
        for(auto &i : p->child) i = nullptr;
        p->sibling = nullptr;
        p->kind.exp = kind;
        return p;
    }
    // 设置运算符表达式中的运算符类型
    void set_operator(operator_type oper){
        if(_type != Node_type::_expK){
            throw "Error, 尝试对于非表达式节点赋值！";
        }
        kind.exp = ExpKind::_opK;
        attr.op = oper;
    }

    //keyword不需要单独存储，从表达式类型即可反向构建

    //设置当前树节点 数值 并设置节点类型
    void set_number(std::string num_string){
        if(_type != Node_type::_expK){
            throw "Error, 尝试对于非表达式节点赋值！";
        }
        kind.exp = ExpKind::_num;
        attr.num = atoi(num_string.c_str());
    }
    
    // 设置当前节点的 字符值,并设置节点类型
    void set_id(std::string id_string){
        if(_type != Node_type::_expK){
            throw "Error, 尝试对于非表达式节点赋值！";
        }
        kind.exp = ExpKind::_id;
        attr.id = (char*) malloc(strlen(id_string.c_str()) + 1);
        strcpy(attr.id, id_string.c_str());
    }
    void to_string(){
        switch (_type)
        {
        case Node_type::_expK:
            switch (kind.exp)
            {
            case ExpKind::_opK :
                std::cout << operator2string(attr.op);
            break;
            
            case ExpKind::_id:
                std::cout << attr.id;
            break;
            case ExpKind::_num:
                std::cout << attr.num;
            break;
            case ExpKind::_var:
                std::cout << attr.id;
            break;
            case ExpKind::_array_var:
                std::cout << attr.id << "[]";
            break;
            case ExpKind::_assign:
                std::cout << "=";
            break;
            case ExpKind::_call:
                std::cout << attr.id << "()";
            break;
            case ExpKind::_args:
                std::cout << operator2string(attr.op);
            break;
            case ExpKind::_simple_exp:
                std::cout <<"simple_expression: " << operator2string(attr.op);
            break;
            default:
            break;
            }
        break;
        
        
        default:
            break;
        }
    }
    void dfs(int layer){
        for(int i = 0 ; i < MAXCHILDREN ; i++){
            if(ExpKind::_args != kind.exp){
                if(i == 1){
                    std::cout  << "└";
                    for(int i = 0 ; i < layer ; i++){
                        std::cout << "-";
                    }
                    to_string();
                    std::cout << std::endl;
                }
            }
           
            if(child[i] != nullptr){
                child[i]->dfs(layer + 1);
            }
        }
        if(sibling != nullptr){
            std::cout << "--sibling start--" << std::endl;
            sibling->dfs(layer);
            std::cout << "--sibling   end--" << std::endl;
        }
    }
    void show(){
        dfs(1);
    }
};
