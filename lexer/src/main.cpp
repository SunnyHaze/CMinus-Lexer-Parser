#include <iostream>
#include "TokenType.h"
#include "Buffer.h"

enum Status{
    START,
    INASSIGN,
    INCOMMENT,
    INNUM,
    INID,
    DONE
};

// 按行数显示所有的文本内容
void showInputFile(){
    char c;
    while((c = getNextChar()) != EOF){
        std::cout<<c;
    }
}

int main(int argc, char* argv[]){
    const char *input = ".\\targetCode\\input.c";
    const char *output = "./targetCode/output.txt";
    setPath(input,output);
    showInputFile();
    return 0;
}