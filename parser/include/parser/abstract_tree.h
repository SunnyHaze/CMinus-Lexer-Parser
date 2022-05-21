#pragma once
#include "TokenType.h"
#include <iostream>
#include <cstring>
class cmparser;
// 单个节点的最大子节点个数
const int MAXCHILDREN = 4;
// 区分节点类型的枚举类型
enum class Node_type{
    _expK, // 表达式类型
    _stmtK, // 语句类型
};
// 语句类型的多种节点
enum class StmtKind{
    // 注释中为“语法规则有，但是没有实现”的非终结符：
    _program,       // 程序入口
    _declaration_list, // 声明列表
    _declaration,   // 一次声明
    _var_declaration,// 变量声明
        // type_specifier       - 只有int和void 
    _fun_declaration, // 函数声明
        // params               - 只有param_list和void，直接用param代替
        // param_list           - 用多个param 的 sibiling代替
    _param,        // 由param组成params
    _param_array,  // 数组形参 - 是原来的表达式中没有的
    _compound_stmt, // {local_declarations statement_list}
        // local_declarations   - 用多个var_declaration或者nullptr代替
        // statement_list       - 用多个不同种类的statement直接代替
    _expression_stmt, //  表达式语句
    _selection_stmt,// 分支语句
    _iteration_stmt,// 循环语句
    _return_stmt,   // 返回语句
};
// 表达式类型节点
enum class ExpKind{
    _void,       // Void节点
    _int,        // int节点
    _assign,     // 赋值语句
    _term,       // 乘法项
    _factor,     // 因子
    _var,        // 变量
    _array_var,  // 数组变量
    _call,       // 函数调用
    _args,      // 函数形参列表
    _num,       //只含有数值的节点
    _id,        // 只含有ID的节点
    _opK,       // 二元运算符节点
    _empty,
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
        StmtKind stmt;
    } kind;
    // 如果是Exp
    union {
        operator_type op;
        keyword_type keyword;
        int num;
        char * id;
    } attr;
    // 用于保存当前节点的行号
    int lineno = 0;
    // 生成一个表达式节点并返回
    static TreeNode * newExpNode(ExpKind kind, int line){
        TreeNode * p = new TreeNode();
        p->_type = Node_type::_expK;
        for(auto &i : p->child) i = nullptr;
        p->sibling = nullptr;
        p->kind.exp = kind;
        p->lineno = line;
        return p;
    }
    // 生成一个语句节点并返回
    static TreeNode * newStmtNode(StmtKind kind, int line){
        TreeNode * p = new TreeNode();
        p->_type = Node_type::_stmtK;
        for(auto &i : p->child) i = nullptr;
        p->sibling = nullptr;
        p->kind.stmt = kind;
        p->lineno = line;
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
        case Node_type::_stmtK:
            std:: cout << "\033[1;34;40m<";
            break;
        case Node_type::_expK:
            std:: cout << "\033[1;35;40m<";
            break;
        }

        switch (_type)
        {
        case Node_type::_stmtK:
            switch (kind.stmt)
            {
            case StmtKind::_program:
                std::cout << "Program" ;
                break;
            case StmtKind::_declaration_list:
                std::cout << "Declaration_list" ;
            break;
            case StmtKind::_declaration:
                std::cout << "Declaration" ;
            break;
            case StmtKind::_var_declaration:
                std::cout << "Var_declaration" ;
            break;
            case StmtKind::_fun_declaration:
                std::cout << "Fun_declaration";
            break;
            case StmtKind::_param:
                std::cout << "Param" ;
            break;
            case StmtKind::_param_array:
                std::cout << "Param_array";
            break;
            case StmtKind::_compound_stmt:
                std::cout << "Compound_stmt";
            break;
            case StmtKind::_expression_stmt:
                std::cout << "Expression_stmt" ;
            case StmtKind::_selection_stmt:
                std::cout << "Selection_stmt" ;
            break;
            case StmtKind::_iteration_stmt:
                std::cout << "Iteration_stmt" ;
            break;
            case StmtKind::_return_stmt:
                std::cout << "Return_stmt";
            break;
            default:
                break;
            }
            break;
        
        case Node_type::_expK:
            switch (kind.exp)
            {
            case ExpKind::_void:
                std::cout << "Type_identifier : VOID";
            break;
            case ExpKind::_int:
                std::cout << "Type_identifier : INT";
            break;
            case ExpKind::_assign:
                std::cout << "Assign: = ";
            break;
            case ExpKind::_var:
                std::cout << "Var";
            break;

            case ExpKind::_array_var:
                std::cout << "Array_var : " << attr.id << "[]";
            break;
            case ExpKind::_call:
                std::cout << "Call";
            break;
            case ExpKind::_args:
                if(attr.op == operator_type::_com){
                    // 后续的形参列表需要标注 “逗号”
                    std::cout <<"Args: "<< operator2string(attr.op);
                }
                else{
                    // 第一个形参则直接输出即可
                    std::cout<< "Args";
                }

            break;
            case ExpKind::_num:
                std::cout << "NUM : " << attr.num;
            break;
            case ExpKind::_id:
                std::cout <<"Identifier : " <<attr.id;
            break;
            case ExpKind::_opK: // 中央节点存运算符，左右节点存下一部运算
                std::cout <<"Operator : " << operator2string(attr.op);
            break;
            case ExpKind::_empty:
                std::cout << "Empty";
            break;
            default:
            break;
            }
        break;
        default:
            break;
        }
        std::cout <<">\033[0m (" <<lineno  << ")" << std::endl;
    }
    void debug_dfs(int layer){
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
    void dfs(int layer){
        std::cout  << "";
        for(int i = 0 ; i < layer ; i++){
            std::cout << " ";
        }
        to_string();
        for(int i = 0 ; i < MAXCHILDREN ; i++){
            if(child[i] != nullptr){
                child[i]->dfs(layer + 1);
            }
        }
        if(sibling != nullptr){
            sibling->dfs(layer);
        }
    }

    void show(){
        // debug_dfs(1);
        dfs(0);
    }

};
