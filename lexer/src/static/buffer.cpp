#include "Buffer.h"
const int BUFFSIZE = 256; // 最大缓冲区长度
int bufflen = 0;
int lineno = 0;
int linepos = 0;
char linebuf[BUFFSIZE];
int EOF_flag = 0;   // 到达文件尾部
std::string inputPath, outputPath;
std::ifstream ifs;
std::ofstream ofs;

void setPath(const char * i, const char * o){
    inputPath.assign(i);
    outputPath.assign(o);
    setFileStream();
}

void setFileStream(){
    ifs.open(inputPath);
    ofs.open(outputPath);
}

int getNextChar(){
    if(linepos >= bufflen){ // 读取新一行
        lineno++;
        std::cout << "# " << lineno << "\t";
        if(ifs.getline(linebuf, BUFFSIZE)){ // 能读取到新一行
            bufflen = strlen(linebuf);
            linebuf[bufflen++] = '\n';
            linepos = 0;
        }else{       
            EOF_flag = 1;           // 读不到了，结束
            return EOF;
        }
    }
    return linebuf[linepos++];
}

void ungetNextChar(void){
    if (!EOF_flag) linepos-- ;
}