//
// Created by a404m on 9/19/21.
//

#ifndef SL_NODE_H
#define SL_NODE_H


#include "../additional/functions.h"
#include "../additional/types.h"
#include <vector>

namespace SL {
    struct Node {
    public:
        enum Token {
            NONE = 0,
            STRING,
            NUMBER,
            BOOL,
            BLOCK,//{} () ...
            OPERATOR,
            KEYWORD,
            ID,
            SYMBOL
        };
        enum SpecialToken{
            INVALID = 0,
            OP_LEFT,//i.e. a++
            OP_RIGHT,//i.e. +a
            OP_BOTH,//i.e. a+2
            OP_BETWEEN//i.e. (a+2)
        };
        std::string str;
        Token token;
        SpecialToken specialToken;
        std::vector<Node> operands;
        std::string::size_type index;

        explicit Node(std::string  = "", Token = NONE, SpecialToken = INVALID, std::initializer_list<Node> = {},std::string::size_type = 0);

        void clear();

        bool isEmpty() const;

        //used for std::map
        bool operator==(const Node&) const;
        bool operator!=(const Node&) const;
        bool operator>(const Node&) const;
        bool operator<(const Node&) const;
        bool operator>=(const Node&) const;
        bool operator<=(const Node&) const;

        bool isEqualByTokenAndString(const Node&) const;

        static const Node BK_PARENTHESIS;

        static const Node OP_POSITIVE;
        static const Node OP_NEGATIVE;
        static const Node OP_LOGICAL_NOT;

        static const Node OP_MULTIPLICATION;
        static const Node OP_DEVIATION;

        static const Node OP_PLUS;
        static const Node OP_MINUS;

        static const Node OP_GREATER;
        static const Node OP_SMALLER;
        static const Node OP_GREATER_OR_EQUAL;
        static const Node OP_SMALLER_OR_EQUAL;

        static const Node OP_EQUAL;
        static const Node OP_NOT_EQUAL;

        static const Node OP_LOGICAL_AND;

        static const Node OP_LOGICAL_OR;

        static const Node OP_ASSIGN;

        static const Node OP_COMMA;
    };
}//namespace SL



#endif //SL_NODE_H
