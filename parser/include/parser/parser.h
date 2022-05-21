# pragma once
#include "lexer/Lexer.h"
#include "TokenType.h"
#include "parser/abstract_tree.h"
class TreeNode;
class cmparser{
    cmlexer *lexer; // 获取lexer指针，以操作lexer.results进行语法分析
    token_base * current_token;
    size_t idx_token;
    bool error_state = 0;
    // 抽象语法树的根节点
    TreeNode * AST;     
public:
    token_base * get_next_token();
    token_base * get_current_token();
    token_type get_current_token_type();
    token_keyword* get_current_keyword();
    token_number* get_current_number();
    token_operator* get_current_operator();
    token_identifier* get_current_id();
    //发生语法错误的报错，传入的字符串作为报错信息
    void SyntaxError(std::string str);
    // Match下一个字符
    bool match_keyword(keyword_type t);
    bool match_operator(operator_type t);
public:
    cmparser() = default;
    cmparser(cmlexer * lex): lexer(lex) {}
    ~cmparser() = default;
    void reset_status(){
        idx_token = 0;
    }
    TreeNode * parse(){
        get_next_token();
        auto res = program();
        if(current_token != nullptr){
            SyntaxError("Unexpected Exit!");
            return nullptr;
        }else{
            return res;
        }
    }
private: 
    TreeNode * program(){
        std::cout << "program" << std::endl;
        auto t = TreeNode::newStmtNode(StmtKind::_program, current_token->get_line());
        t->child[0] =  declaration_list();
        std::cout << "FINISH PARSING" << std::endl;
        return t;
    }
    TreeNode * declaration_list(){
        auto f = TreeNode::newStmtNode(StmtKind::_declaration_list,current_token->get_line());
        f->child[0] = declaration();
        auto p = f->child[0];
        // 如果符合进一步的FIRST集合
        if(current_token != nullptr && get_current_token_type() == token_type::_ID || (get_current_token_type() == token_type::_keyword && (get_current_keyword()->get_keyword_type() == keyword_type::_int || get_current_keyword()->get_keyword_type() == keyword_type::_void) ) ){
            p->sibling = declaration();
            p = p->sibling;
        }
        return f;
    }
    TreeNode * declaration(){
        std::cout << "declaration" << std::endl;
        auto t = TreeNode::newStmtNode(StmtKind::_declaration,current_token->get_line());
        TreeNode * local_type_specifier;
        TreeNode * local_ID;
        if(current_token != nullptr && get_current_token_type() == token_type::_keyword){
            switch (get_current_keyword()->get_keyword_type())
            {
            case keyword_type::_void :
                local_type_specifier = TreeNode::newExpNode(ExpKind::_void,current_token->get_line());
                match_keyword(keyword_type::_void);
                break;
            case keyword_type::_int:
                local_type_specifier = TreeNode::newExpNode(ExpKind::_int,current_token->get_line());
                match_keyword(keyword_type::_int);
                break;
            default:
                SyntaxError("不合法的声明！");
                get_next_token();
                break;
            }
            if(current_token != nullptr && get_current_token_type() == token_type::_ID){
                local_ID = TreeNode::newExpNode(ExpKind::_id,current_token->get_line());
                local_ID->set_id(get_current_id()->get_ID());
                get_next_token();
            }else{
                SyntaxError("不合法的声明！");
                get_next_token();
            }
        }
        if(current_token != nullptr && get_current_token_type() == token_type::_operator){
            switch(get_current_operator()->get_operator_type()){
                case operator_type::_mlb:
                    t->child[0] = var_declaration(local_type_specifier, local_ID);
                    break;
                case operator_type::_slb:
                    t->child[0] = fun_declaration(local_type_specifier, local_ID);
                break;
                default:
                    t->child[0] = var_declaration(local_type_specifier, local_ID);
                    break;
            }
        }
        else{
            t->child[0] = var_declaration(local_type_specifier, local_ID);
        }
        return t;
    }

    TreeNode * fun_declaration(TreeNode * type_specifier, TreeNode * ID){
        std::cout << "fun_declaration" << std::endl;
        auto t = TreeNode::newStmtNode(StmtKind::_fun_declaration,current_token->get_line());
        if(type_specifier == nullptr){
            if(current_token != nullptr && get_current_token_type() == token_type::_keyword){
                TreeNode * k;
                switch (get_current_keyword()->get_keyword_type())
                {
                case keyword_type::_void :
                    k = TreeNode::newExpNode(ExpKind::_void,current_token->get_line());
                    t->child[0] = k;
                    match_keyword(keyword_type::_void);
                    break;
                case keyword_type::_int:
                    k = TreeNode::newExpNode(ExpKind::_int,current_token->get_line());
                    t->child[0] = k;
                    match_keyword(keyword_type::_int);
                    break;
                default:
                    SyntaxError("不合法的函数声明！");
                    get_next_token();
                    break;
                }
                if(current_token != nullptr && get_current_token_type() != token_type::_ID){
                    auto k = TreeNode::newExpNode(ExpKind::_id,current_token->get_line());
                    k->set_id(get_current_id()->get_ID());
                    t->child[1] = k;
                    get_next_token();
                }else{
                    SyntaxError("不合法的函数声明！");
                    delete(t);
                    get_next_token();
                }
            }
        }else{
            t->child[0] = type_specifier;
            t->child[1] = ID;
        }
        match_operator(operator_type::_slb);
        auto j = params();
        t->child[2] = j;
        match_operator(operator_type::_srb);
        j = compound_stmt();
        t->child[3] = j;
        return t;
    }
    // params -> paramlist | VOID
    TreeNode * params(){
        std::cout << "params" << std::endl;
        if(current_token != nullptr && get_current_token_type() == token_type::_keyword && get_current_keyword()->get_keyword_type() == keyword_type::_void){
            auto k = TreeNode::newExpNode(ExpKind::_void,current_token->get_line());
            match_keyword(keyword_type::_void);
            if(current_token != nullptr && get_current_token_type() != token_type::_ID){
                k->kind.stmt = StmtKind::_param;
                return k;
            }else{
                return param_list(k);
            }
        }else{
            return param_list(nullptr);
        }
    }
    // param_list -> param {, param}
    TreeNode * param_list(TreeNode * pass_node){
        std::cout << "param_list" << std::endl;
        auto t = param(pass_node);
        auto p = t;
        while (current_token != nullptr && get_current_token_type() == token_type::_operator && get_current_operator()->get_operator_type() == operator_type::_com)
        {
            match_operator(operator_type::_com);
            auto k = param(nullptr);
            p->sibling = k;
            p = k;
        }
        return t;
    }
    // param -> type_specifier ID [ \[\] ] //用转义符表达真实匹配的信息
    TreeNode * param(TreeNode * pass_node){
        std::cout << "param" << std::endl;
        auto t = TreeNode::newStmtNode(StmtKind::_param,current_token->get_line());
        // 匹配开头的 type_specifier
        if(current_token != nullptr &&get_current_token_type() == token_type::_keyword){
            // 如果上面没有传下来消除左公共因子的参数
            if(pass_node == nullptr){
                if(get_current_keyword()->get_keyword_type() == keyword_type::_void){
                    auto k = TreeNode::newExpNode(ExpKind::_void,current_token->get_line());
                    t->child[0] = k;
                    match_keyword(keyword_type::_void);
                }
                else if(get_current_keyword()->get_keyword_type() == keyword_type::_int){
                    auto k = TreeNode::newExpNode(ExpKind::_int,current_token->get_line());
                    t->child[0] = k;
                    match_keyword(keyword_type::_int);
                }
            }else{// 如果上面为了消除左公共因子的参数传下来了了
                t->child[0] = pass_node;
            }
            // 进一步匹配ID
            if(current_token != nullptr && get_current_token_type() == token_type::_ID){
                auto k = TreeNode::newExpNode(ExpKind::_id,current_token->get_line());
                k->set_id(get_current_id()->get_ID());
                t->child[1] = k;
                get_next_token();
            }
            if(current_token != nullptr && get_current_token_type() == token_type::_operator && get_current_operator()->get_operator_type() == operator_type::_mlb){
                t->kind.stmt = StmtKind::_param_array; // 转换为数组保存
                match_operator(operator_type::_mlb);
                match_operator(operator_type::_mrb);
            }
            return t;
        }else{
            SyntaxError("不符合形参列表的规范！");
            return nullptr;
        }
    }


    TreeNode * compound_stmt(){
        std::cout << "compound_stmt" << std::endl;
        auto t = TreeNode::newStmtNode(StmtKind::_compound_stmt,current_token->get_line());
        match_operator(operator_type::_llb);
        t->child[0] = local_declarations();
        t->child[1] = statement_list();
        match_operator(operator_type::_lrb);
        return t;
    }
    TreeNode * local_declarations(){
        std::cout << "local_declarations" << std::endl;
        TreeNode * t = var_declaration(nullptr, nullptr);
        TreeNode * p = nullptr;
        while (current_token != nullptr && get_current_token_type() == token_type::_keyword && (get_current_keyword()->get_keyword_type() == keyword_type::_int || get_current_keyword() ->get_keyword_type() == keyword_type::_void))
        {
            p = var_declaration(nullptr, nullptr);
            t->sibling = p;
            if(p != nullptr){
                p = p->sibling;
            }
        }
        return t;
    }



    // var_declaration -> type_specifier ID; | type_specifier ID [ NUM ] ; 
    // 有声明列表则返回对应节点，否则返回nullptr
    // 因为存在“左公共因子” ，所以通过传参数来区别上层传下来的内容
    TreeNode * var_declaration(TreeNode * type_specifier, TreeNode * ID){
        std::cout << "var_declaration" << std::endl;
        TreeNode * t = nullptr;
        // 如果没有从上面传下来提前读的TypeID和specifier
        if(type_specifier == nullptr){
            if(current_token != nullptr && get_current_token_type() == token_type::_keyword){
                t = TreeNode::newStmtNode(StmtKind::_var_declaration,current_token->get_line());
                // p匹配 void 或 int
                if(get_current_keyword()->get_keyword_type() == keyword_type::_void ){
                    auto n = TreeNode::newExpNode(ExpKind::_void,current_token->get_line());
                    t->child[0] = n;
                    match_keyword(keyword_type::_void);
                }else if(get_current_keyword()->get_keyword_type() == keyword_type::_int){
                    auto n = TreeNode::newExpNode(ExpKind::_int,current_token->get_line());
                    t->child[0] = n;
                    match_keyword(keyword_type::_int);
                }else{
                    delete(t);
                    return nullptr;
                }
                // 匹配ID
                if(current_token != nullptr && get_current_token_type() == token_type::_ID){
                    auto n = TreeNode::newExpNode(ExpKind::_id,current_token->get_line());
                    n->set_id(get_current_id()->get_ID());
                    t->child[1] = n;
                    get_next_token();
                }else{
                    delete(t);
                    return nullptr;
                }
            }else{
                return nullptr;
            }
        }else {// 如果从上面传下来了已经读好的 type_specifier 和ID
            t = TreeNode::newStmtNode(StmtKind::_var_declaration,current_token->get_line());
            t->child[0] = type_specifier;
            t->child[1] = ID;
        }
        // 如果存在方括号
        if(current_token != nullptr && get_current_token_type() == token_type::_operator && get_current_operator()->get_operator_type() == operator_type::_mlb){
                match_operator(operator_type::_mlb);
                TreeNode * q3 = TreeNode::newExpNode(ExpKind::_num,current_token->get_line());
                q3->set_number(get_current_number()->get_number());
                t->child[2] = q3;
                get_next_token();
                match_operator(operator_type::_mrb);
        }
        match_operator(operator_type::_sem);
        return t;        
    }

    TreeNode * statement_list(){
        std::cout << "statement_list" << std::endl;
        auto t = statement();
        auto p = t;
        if(t == nullptr){
            return nullptr;
        }else{
            while(current_token != nullptr &&
            // keyword
            (
            get_current_token_type() == token_type::_keyword && (
                get_current_keyword()->get_keyword_type() == keyword_type::_if || 
                get_current_keyword()->get_keyword_type() == keyword_type::_while ||
                get_current_keyword()->get_keyword_type() == keyword_type::_return )
            )||(// operator
            get_current_token_type() == token_type::_operator &&(
                get_current_operator()->get_operator_type() == operator_type::_llb ||
                get_current_operator()->get_operator_type() == operator_type::_sem ||
                get_current_operator()->get_operator_type() == operator_type::_slb)
            )||(
                get_current_token_type() == token_type::_ID
            )||(
                get_current_token_type() == token_type::_NUM
            )
            ){
                auto q = statement();
                if(q != nullptr){
                    p->sibling = q;
                    p = q;
                }
            }
        }
        return t;
    }
    TreeNode * statement(){
        std::cout << "statement" << std::endl;
        TreeNode * t = nullptr;
        if(current_token != nullptr){
            switch (get_current_token_type())
            {
            case token_type::_keyword :
                switch (get_current_keyword()->get_keyword_type())
                {
                case keyword_type::_if:
                    t = selection_stmt();
                break;
                
                case keyword_type::_while:
                    t = iteration_stmt();
                break;

                case keyword_type::_return:
                    t = return_stmt();
                break;  
                default:
                    SyntaxError("语句中出现了非法的关键字！");
                    get_next_token();
                    break;
                }
                break;
            
            case token_type::_ID: case token_type::_NUM:
                t = expression_stmt();
                break;

            case token_type::_operator:
                switch (get_current_operator()->get_operator_type())
                {
                // 匹配 '{'
                case operator_type::_llb :
                    t = compound_stmt();
                    break;
                case operator_type::_sem: case operator_type::_slb:
                    t = expression_stmt();
                    break;        

                default:
                    SyntaxError("语句中出现了非法的运算符！");
                    get_next_token();
                    break;
                }
                break;
            default:
                SyntaxError("语句中出现了非法的token类型！");
                get_next_token();
                break;
            }
        }
        return t;
    }

    //expression_stmt -> expression ; | ;
    TreeNode* expression_stmt(){
        
        std::cout << "expression_stmt" << std::endl;
        TreeNode *t = nullptr;
        if(current_token != nullptr && get_current_token_type() == token_type::_operator && get_current_operator()->get_operator_type() == operator_type::_sem){
            match_operator(operator_type::_sem);
            //TODO 有可能返回空指针会炸
        }else{
            t = expression();
        }
        return t;
    }
    //selection_stmt -> IF ( expression ) statement | IF ( expression ) statement ELSE statement
    TreeNode * selection_stmt(){
        std::cout << "selection_stmt" << std::endl;
        auto t = TreeNode::newStmtNode(StmtKind::_selection_stmt,current_token->get_line());
        // match if
        match_keyword(keyword_type::_if);
        // match (
        match_operator(operator_type::_slb);
        t->child[0] = expression();
        // match )
        match_operator(operator_type::_srb);
        t->child[1] = statement();
        if(current_token != nullptr && get_current_token_type() == token_type::_keyword && get_current_keyword()->get_keyword_type() == keyword_type::_else){
            match_keyword(keyword_type::_else);
            t->child[2] = statement();
        }
        return t;
    }
    // iteration_stmt -> WHILE ( expression ) statement
    TreeNode * iteration_stmt(){
        std::cout << "iteration_stmt" << std::endl;
        auto t = TreeNode::newStmtNode(StmtKind::_iteration_stmt,current_token->get_line());
        // match while
        match_keyword(keyword_type::_while);
        // match (
        match_operator(operator_type::_slb);
        t->child[0] = expression();
        // match )
        match_operator(operator_type::_srb);
        t->child[1] = statement();
        return t;
    }

    TreeNode * return_stmt(){
        std::cout << "return_stmt" << std::endl;
        auto t = TreeNode::newStmtNode(StmtKind::_return_stmt,current_token->get_line());
        match_keyword(keyword_type::_return);
        // 匹配 ';'
        if(current_token != nullptr && get_current_token_type() == token_type::_operator && get_current_operator()->get_operator_type() == operator_type::_sem){
            match_operator(operator_type::_sem);
        }else{
            t->child[0] = expression();
            match_operator(operator_type::_sem);
        }
        return t;
    }

    TreeNode * expression(){
        std::cout << "expression" << std::endl;
        TreeNode *t = var();
        // 不是以ID开头，只能是simple_expression
        if(t == nullptr){
            t = simple_expression(nullptr); 
        }else{
            // 赋值语句 var = expression
            if(current_token != nullptr && get_current_token_type()== token_type::_operator && get_current_operator()->get_operator_type() == operator_type::_asi) {
                auto p = TreeNode::newExpNode(ExpKind::_assign,current_token->get_line());
                p->attr.op = operator_type::_asi;
                match_operator(operator_type::_asi);
                p->child[0] = t;
                p->child[1] = expression();
                return p;
            }else{
                //传入分为三种情况,需要在simple中进行区分：
                // 1. t = id 下一步是其他的
                // 2. t = id[expression]
                // 3. t = id 下一步是(    
                t = simple_expression(t);
            }
        }
        return t;
    }
    //simple_expression -> additive_expression [ relop additive_expression ]
    // relop -> < > <= >= == !=
    TreeNode * simple_expression(TreeNode * pass_node){
        std::cout << "simple_expression" << std::endl;
        auto n = additive_expression(pass_node);
        // 如果是operator
        if(current_token != nullptr && get_current_token_type() == token_type::_operator){
            auto current_op_type = get_current_operator()->get_operator_type();
            // 如果是6个关系运算符
            if(current_op_type >= operator_type::_les && current_op_type <= operator_type::_neq){
                auto p = TreeNode::newExpNode(ExpKind::_opK,current_token->get_line());
                p->attr.op = current_op_type;
                match_operator(current_op_type);
                p->child[0] = n;
                n = p;
                n->child[1] = additive_expression(nullptr);
            }
        }
        return n;
    }

    // additive_expression -> term {addop term}
    // addop -> + | -
    TreeNode * additive_expression(TreeNode * pass_node){
        std::cout << "additive_expression" << std::endl;
        auto n = term(pass_node);
        // 如果个符号是加号或者减号
        while(get_current_operator() != nullptr && (get_current_operator()->get_operator_type() == operator_type::_add || get_current_operator()->get_operator_type() == operator_type::_sub)){
            // 将这个运算符赋予节点
            auto p = TreeNode::newExpNode(ExpKind::_opK,current_token->get_line());
            p->child[0] = n;
            p->set_operator(get_current_operator()->get_operator_type());
            // 匹配此字符
            match_operator(get_current_operator()->get_operator_type());
            n = p;
            n->child[1] = term(nullptr);
        }
        return n;
    }
    // term -> factor {mulop  factor }
    // mulop -> * | /
    TreeNode * term(TreeNode * pass_node){
        std::cout << "term" << std::endl;
        auto n = factor(pass_node);
        while(get_current_operator() != nullptr && (get_current_operator()->get_operator_type() == operator_type::_mul || get_current_operator()->get_operator_type() == operator_type::_div)){
            TreeNode *p = TreeNode::newExpNode(ExpKind::_opK,current_token->get_line());
            p->child[0] = n;
            p->set_operator(get_current_operator()->get_operator_type());
            match_operator(get_current_operator()->get_operator_type());
            n = p;
            n->child[1] = factor(nullptr);
        }
        return n;
    }
    // factor ->
        // 1.(expression)
        // 2.ID (args)       // call
        // 3.ID [expression] // var
        // 4.ID              // var
        // 5.NUM             // NUM
    TreeNode * factor(TreeNode * pass_node){
        std::cout << "factor" << std::endl;
        TreeNode *t;
        // 如果从上层传下来了需要的
        if(pass_node != nullptr){
            // 匹配 ID(args)
            if(current_token != nullptr && get_current_token_type() == token_type::_operator && get_current_operator()->get_operator_type() == operator_type::_slb){
                t = call(pass_node);

            }else{
                t = pass_node;
            }
        }else{
            //如果从上层没传下来需要的节点
            if(current_token != nullptr){
                switch (get_current_token_type())
                {
                case token_type::_operator:
                    // 1. 匹配 (expression)
                    if(get_current_operator()->get_operator_type() == operator_type::_slb){
                        match_operator(operator_type::_slb);
                    }else{
                        SyntaxError("非法的字符");
                        get_next_token();
                    }
                    t = expression();
                    match_operator(operator_type::_srb);
                break;
                // 匹配 NUM
                case token_type::_NUM:
                    t = TreeNode::newExpNode(ExpKind::_num,current_token->get_line());
                    t->set_number(get_current_number()->get_number());
                    get_next_token();
                break;
                // 匹配纯ID
                case token_type::_ID:
                    t = var();
                    if(current_token != nullptr && get_current_token_type() == token_type::_operator && get_current_operator()->get_operator_type() == operator_type::_slb){
                        t = call(t)  ;
                    }
                break;
                default:
                    SyntaxError("非法的字符");
                    get_next_token();
                break;
                }
            }
        }
        return t;  
    }
    TreeNode * var(){
        std::cout << "var" << std::endl;
        auto t = TreeNode::newExpNode(ExpKind::_var,current_token->get_line());
        if(current_token != nullptr && get_current_token_type() == token_type::_ID){
            auto j = TreeNode::newExpNode(ExpKind::_id,current_token->get_line());
            j->set_id(get_current_id()->get_ID());
            t->child[0] = j;
            get_next_token();
            //如果匹配左括号 '[' 则为ID[expression]
            if(current_token != nullptr && get_current_token_type() == token_type::_operator && get_current_operator()->get_operator_type() == operator_type::_mlb){
                t->kind.exp = ExpKind::_array_var;
                match_operator(operator_type::_mlb);
                t->child[1] = expression();
                match_operator(operator_type::_mrb);
            }
            return t;
        }else{
            return nullptr;
        }
    }

    TreeNode *call(TreeNode *k){
        std::cout << "call" << std::endl;
        TreeNode *t = TreeNode::newExpNode(ExpKind::_call,current_token->get_line());
        if (k != nullptr){
            t->child[0] = k;
        }
        match_operator(operator_type::_slb);
        if(current_token != nullptr && get_current_token_type() == token_type::_operator && get_current_operator()->get_operator_type() == operator_type::_srb){
            match_operator(operator_type::_srb);
            t->child[1] = TreeNode::newExpNode(ExpKind::_empty,current_token->get_line());
        }else if(k != nullptr){
            t->child[1] = arg_list();
            match_operator(operator_type::_srb);
        }
        return t;
    }
    // args -> empty | expression {, expression} 
    // 此处已经在call中解决了empty的问题
    TreeNode * arg_list(){
        std::cout << "arg_list" << std::endl;
        auto t = TreeNode::newExpNode(ExpKind::_args,current_token->get_line());
        
        auto n = expression(); 
        t->child[0] = n;
        // 如果下一项为 ','
        TreeNode * p = t; // 滑动指针
        while(current_token!=nullptr && get_current_token_type() == token_type::_operator && get_current_operator()->get_operator_type() == operator_type::_com){
            match_operator(operator_type::_com);
            TreeNode *new_node = TreeNode::newExpNode(ExpKind::_args,current_token->get_line());
            new_node->child[0] = expression();
            new_node->attr.op = operator_type::_com;
            p->sibling = new_node;
            p = p->sibling;
        }
        return t;
    }
};
// cmlexer lex;
// cmlexer * cmparser::lexer = &lex;
// size_t cmparser::idx_token = 0;