#pragma once
#include "TokenType.h"
const int MAXCHILDREN = 3;
enum class Node_type{
    _var,
    _expression,
    simple_expression,
    additive_expression,
    addop
};
class TreeNode{
public:
    TreeNode();
    ~TreeNode() = default;
    TreeNode * child[MAXCHILDREN];

};