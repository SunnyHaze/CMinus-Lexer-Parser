# pragma once
#include "lexer/Lexer.h"
#include "TokenType.h"
#include "parser/abstract_tree.h"
class TreeNode;
class cmparser{
    cmlexer *lexer; // 获取lexer指针，以操作lexer进行语法分析
    token_base * current_token;
    size_t idx_token;
    bool error_state = 0;
    // 抽象语法树的根节点
    TreeNode * AST;     
public:
    // 获取下一个token 指针
    token_base * get_next_token();
    // 获取当前token 指针
    token_base * get_current_token();
    // 获取当前token 类型
    token_type get_current_token_type();
    // 在确认token类型后，以keyword 获取
    token_keyword* get_current_keyword();
    // 在确认token类型后， 以 number获取
    token_number* get_current_number();
    // 在确认token类型后， 以运算符获取
    token_operator* get_current_operator();
    // 在确认Token类型后， 以标识符获取
    token_identifier* get_current_id();
    //发生语法错误的报错，传入的字符串作为报错信息
    void SyntaxError(std::string str);
    // Match下一个关键字，如果不匹配则抛异常
    bool match_keyword(keyword_type t);
    // Match 下一个运算符，如果和参数列表中的不匹配则抛异常
    bool match_operator(operator_type t);
public:
    cmparser() = default;
    // 需要传入lexer作为初始化起点
    cmparser(cmlexer * lex): lexer(lex) {}
    ~cmparser() = default;
    void reset_status(){
        idx_token = 0;
    }
    // 运行parser
    TreeNode * parse();
private: 
    // program -> declaration_list
    TreeNode * program();

    // declaration_list ->  declaration {declaration}
    TreeNode * declaration_list();

    // declaration -> var_declaration  |  fun_declaration
    // FIRST(var_declaration) ∩ FIRST(fun_declaration) = type_specifier ID
    TreeNode * declaration();

    // fun_declaration -> type_specifier ID ( params )  compound_stmt
    TreeNode * fun_declaration(TreeNode * type_specifier, TreeNode * ID);

    // params -> paramlist | VOID
    TreeNode * params();

    // param_list -> param {, param}
    TreeNode * param_list(TreeNode * pass_node);

    // param -> type_specifier ID [ \[\] ] //用转义符表达真实匹配的信息
    TreeNode * param(TreeNode * pass_node);

    TreeNode * compound_stmt();
    TreeNode * local_declarations();

    // var_declaration -> type_specifier ID; | type_specifier ID [ NUM ] ; 
    // 有声明列表则返回对应节点，否则返回nullptr
    // 因为存在“左公共因子” ，所以通过传参数来区别上层传下来的内容
    TreeNode * var_declaration(TreeNode * type_specifier, TreeNode * ID);

    // statement_list -> {statement}
    TreeNode * statement_list();

    // statement -> expression_stmt  |  compound_stmt  |  selection_stmt  |  iteration_stmt  |  return_stmt
    TreeNode * statement();

    //expression_stmt -> expression ; | ;
    TreeNode* expression_stmt();
    //selection_stmt -> IF ( expression ) statement | IF ( expression ) statement ELSE statement
    TreeNode * selection_stmt();
    
    // iteration_stmt -> WHILE ( expression ) statement
    TreeNode * iteration_stmt();

    // return_stmt -> RETURN;  |  RETURN expression; 
    TreeNode * return_stmt();

    // expression -> var = expression  |  simple_expression
    TreeNode * expression();
    //simple_expression -> additive_expression [ relop additive_expression ]
    // relop -> < > <= >= == !=
    TreeNode * simple_expression(TreeNode * pass_node);

    // additive_expression -> term {addop term}
    // addop -> + | -
    TreeNode * additive_expression(TreeNode * pass_node);
    // term -> factor {mulop  factor }
    // mulop -> * | /
    TreeNode * term(TreeNode * pass_node);
    // factor ->
        // 1.(expression)
        // 2.ID (args)       // call
        // 3.ID [expression] // var
        // 4.ID              // var
        // 5.NUM             // NUM
    TreeNode * factor(TreeNode * pass_node);

    // var -> ID  |  ID [ expression ]
    TreeNode * var();

    //call -> ID ( args )
    TreeNode *call(TreeNode *k);

    // args -> empty | expression {, expression} 
    // 此处已经在call中解决了empty的问题
    TreeNode * arg_list();
};
