#include "lexer/Lexer.h"
#include "TokenType.h"
#include "parser/abstract_tree.h"

class cmparser{
    cmlexer *lexer; // 获取lexer指针，以操作lexer.results进行语法分析
    size_t idx_token = 0;
    token_base * get_next_token(){
        if(lexer->results.size() == idx_token){
            return NULL;
        }else{
            return lexer->results[idx_token++];
        }
    }
public:
    cmparser();
    cmparser(cmlexer * lex): lexer(lex){}
    ~cmparser() = default;
    void reset_status(){
        idx_token = 0;
    }
    
};