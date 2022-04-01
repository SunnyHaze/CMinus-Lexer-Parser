# pragma once
extern const int MAX_TOKEN_SIZE;
extern char TokenString[];
extern int TokenIndex;
enum TOKENType{
    // 基本关键字
    IF=256,
    ELSE,
    INT,
    RETURN,
    VOID,
    WHILE,
    // 双符号运算符
    LE, // <= less equal
    GE, // >= greater equal
    EQ, // == equal
    NEQ, // != not equal
    // 对象
    ID, // Identifier
    NUM, // number
    // 错误类型
    ERROR, 
    EOF_FILE
};
// 其他的单目运算符，直接通过它对应的ASCII码来表示

// 从enum类型到文本类型的转换
const char * TokenToString(TOKENType t);

