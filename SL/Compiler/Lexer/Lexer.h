//
// Created by a404m on 9/19/21.
//

#ifndef SL_LEXER_H
#define SL_LEXER_H


#include "../Node/Node.h"
#include <list>

namespace SL {
    class Lexer {
        friend class Parser;
    private:
        std::string code;
        std::vector<std::vector<Node>> holder;
        std::vector<Node> line;
    public:
        explicit Lexer(const std::string&);

        void makeError(std::string,const Node &);
    private:
        inline void push_clear(Node &);

        inline void push_clear_ifnempty(Node &);
    };
}//namespace SL



#endif //SL_LEXER_H
