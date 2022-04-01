#include "TokenType.h"
const int MAX_TOKEN_SIZE = 256;
char TokenString[MAX_TOKEN_SIZE + 1];
int TokenIndex = 0;
const char* TokenToString(TOKENType t)
{
    switch (t)
    {
    case IF:        return "IF";        break;
    case ELSE:      return "ELSE";      break;
    case INT:       return "INT";       break;
    case RETURN:    return "RETURN";    break;
    case VOID:      return "VOID";      break;
    case WHILE:     return "WHILE";     break;
    case LE:        return "LE";        break;
    case GE:        return "GE";        break;
    case NEQ:       return "NEQ";       break;
    case ID:        return "ID";        break;
    case NUM:       return "NUM";       break;
    case ERROR:     return "ERROR";     break;
    case EOF_FILE:   return "EOF_FILE"; break;

    case '+':       return "'+'";       break;
    case '-':       return "'-'";       break;
    case '*':       return "'*'";       break;
    case '/':       return "'/'";       break;
    case '<':       return "'<'";       break;
    case '>':       return "'>'";       break;
    case '=':       return "'='";       break;
    case ';':       return "';'";       break;
    case ',':       return "','";       break;
    case '(':       return "'('";       break;
    case ')':       return "')'";       break;
    case '[':       return "'['";       break;
    case ']':       return "']'";       break;
    case '{':       return "'{'";       break;
    case '}':       return "'}'";       break;
    default:
        return "<Invalid Token>";
        break;
    }
}