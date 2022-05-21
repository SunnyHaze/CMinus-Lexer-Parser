## C-Minus 的词法规则
- 关键字： if  else  int  return  void  while
- 专用符号：  +  -  *  /  <  <=  >  >=  ==  !=  =  ;  ,  (  )  [  ]  {  }  /*  */
- 其他标记为 ID 和 NUM ，通过下列正则表达式定义：
```
ID = letter letter*
NUM = digit digit*
Letter = a|..|z|A|..|Z
Digit = 0|..|9
```
- 空格由空白、换行符、制表符组成。
- 注释由 /\*...\*/ 围起来。

## C-Minus 的语法规则
**C-Minus 的 BNF 语法如下：**
1. ❤program -> declaration_list
2. ❤declaration_list -> declaration_list declaration  |  declaration
3. ❤declaration -> var_declaration  |  fun_declaration
4. ❤var_declaration -> type_specifier ID  |  type_specifier ID [ NUM ] 
5. ❤type_specifier -> INT  |  VOID
6. ❤fun_declaration -> type_specifier ID ( params )  compound_stmt
7. ❤params -> param_list  |  VOID
8. ❤param_list -> param_list , param  |  param
9. ❤param -> type_specifier ID  |  type_specifier ID [ ]
10. ❤compound_stmt -> { local_declarations statement_list }
11. ❤local_declarations -> local_declarations var_declaration  |  empty
12. ❤statement_list -> statement_list statement  |  empty
13. ❤statement -> expression_stmt  |  compound_stmt  |  selection_stmt  |  iteration_stmt  |  return_stmt
14. ❤expression_stmt -> expression ;  |  ;
15. ❤selection_stmt -> IF ( expression ) statement  |  IF ( expression ) statement ELSE statement
16. ❤iteration_stmt -> WHILE ( expression ) statement
17. ❤return_stmt -> RETURN;  |  RETURN expression; 
18. ❤expression -> var = expression  |  simple_expression
19. ❤var -> ID  |  ID [ expression ]
20. ❤simple_expression -> additive_expression relop additive_expression  |  additive_expression
21. ❤relop -> LE  |  LT  |  GT  |  GE  |  EQ  |  NE
22. ❤additive_expression -> additive_expression addop term  |  term
23. ❤addop -> +  |  -
24. ❤term -> term mulop factor  |  factor
25. ❤mulop -> *  |  /
26. ❤factor -> ( expression )  |  var  |  call  |  NUM         
27. ❤call -> ID ( args )  
28. ❤args -> arg_list  |  empty                               
29. ❤arglist -> arg_list , expression  |  expression

## EBNF语法如下：
```
program → declaration-list

declaration-list → declaration-list {declaration}

declaration → var-declaration|fun-declaration

var-declaration → type-specifier ID | type-specifier ID[NUM];

type-specifier → int|void

fun-declaration → type-specifier ID(params)|compound-stmt

params → params-list|void

param-list → param{,param}

param → type-specifier ID{[]}

compound-stmt → {local-declarations statement-list}

local-declarations → empty{var-declaration}

statement-list → {statement}

statement → expression-stmt|compound-stmt|selection-stmt|

iteration-stmt|return-stmt

expression-stmt → [expression];

selection-stmt → if(expression) statement [else statement]

iteration-stmt → while(expression) statement

return-stmt → return[expression];

expression → var=expression|simple-expression

var → ID|ID[expression]

simple-expression → additive-expression {relop additive-expression}

relop → <=|<|>|>=|==|!=

additive-expression → term{addop term}

addop → +|-

term → factor{mulop factor}

mulop → *|/

factor → (expression)|var|call|NUM

call → ID(args)                     //done

args → arg-list|empty               //done

arg-list → expression{, expression} //done
```