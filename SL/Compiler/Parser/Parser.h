//
// Created by a404m on 9/19/21.
//

#ifndef SL_PARSER_H
#define SL_PARSER_H


#include "../Lexer/Lexer.h"

namespace SL {
    class Parser {
        friend class CodeGenerator;
    private:
        std::vector<Node> holder;
        Lexer lexer;
        static const std::vector<std::vector<Node>> orders;
    public:
        explicit Parser(Lexer);

    private:
        void parseLine(std::vector<Node>::iterator &,std::vector<Node>::iterator &,std::vector<Node>&);

        static void setOperatorKind(const std::vector<Node>::iterator &,const std::vector<Node>::iterator &,const std::vector<Node>::iterator &);
        static bool isOperand(const Node&);
        static std::vector<Node>::const_iterator isInBy_string_token(const std::vector<Node> &, const std::vector<Node>::const_iterator &);
    };
}


#endif //SL_PARSER_H
