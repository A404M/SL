//
// Created by a404m on 9/19/21.
//

#include "Parser.h"

using namespace SL;

const std::vector<std::vector<Node>> Parser::orders{
        {
                Node::BK_PARENTHESIS
        },{
                Node::OP_POSITIVE,
                Node::OP_NEGATIVE,
                Node::OP_LOGICAL_NOT
        },{
                Node::OP_MULTIPLICATION,
                Node::OP_DEVIATION
        },{
                Node::OP_PLUS,
                Node::OP_MINUS
        },{
                Node::OP_GREATER,
                Node::OP_SMALLER,
                Node::OP_GREATER_OR_EQUAL,
                Node::OP_SMALLER_OR_EQUAL,
        },{
                Node::OP_EQUAL,
                Node::OP_NOT_EQUAL
        },{
                Node::OP_LOGICAL_AND
        },{
                Node::OP_LOGICAL_OR
        },{
                Node::OP_ASSIGN
        },{
                Node::OP_COMMA
        }
};

Parser::Parser(Lexer& l) : lexer(std::move(l)) {
    for(auto& line : lexer.holder) {
        auto begin = line.begin(),end = line.end();
        parseLine(begin,end,line);
        holder.push_back(std::move(*begin));
    }
    lexer.holder.clear();
}

void Parser::parseLine(std::vector<Node>::iterator &begin,std::vector<Node>::iterator &end,std::vector<Node>& line) {
    auto orderIt = orders.begin();
    for (auto it = end;end-begin > 1;it = end){
        for(auto temp = begin;temp < end;++temp){
            switch(temp->token){
                case Node::BLOCK:
                case Node::OPERATOR:
                case Node::KEYWORD:
                    if(temp->operands.empty()) {
                        setOperatorKind(begin, end, temp);
                        auto founded = isInBy_string_token(*orderIt, temp);
                        if (founded != orderIt->cend()) {
                            if (*founded == *temp) {
                                std::swap(it,temp);
                                if(it->token == Node::OPERATOR && it->str == "="){//left associative
                                    std::swap(temp,it);
                                    continue;
                                }else{
                                    goto BRK;
                                }
                            } else {
                                temp->specialToken = Node::INVALID;
                            }
                        }
                    }
                    break;
                default:
                    continue;
            }
            continue;
            BRK:break;
        }
        if(it == end){
            ++orderIt;
            if(orderIt == orders.end()){
                Node *pointer = &*begin;
                while(!pointer->operands.empty()){
                    pointer = &*(pointer->operands.end()-1);
                }
                lexer.makeError("Excepted ';':",*pointer);
            }
            continue;
        }
        switch (it->token) {
            case Node::BLOCK:
                if(isOperand(*(it-1))) {
                    int in = 1;
                    ++it;
                    auto b = it;
                    while (true) {
                        if (it->token == Node::BLOCK) {
                            if (it->str == "(")
                                ++in;
                            else if (it->str == ")")
                                --in;
                            else
                                throw std::runtime_error("Parser::parseLine");

                            if (!in) {
                                if(it == b){
                                    it->clear();
                                }else {
                                    auto diff = end-it;
                                    parseLine(b,it,line);
                                    line.erase(it);
                                    --it;
                                    end = it+diff;
                                }
                                break;
                            }
                        }
                        ++it;
                        if (it >= end) {
                            --it;
                            Node *pointer = &*it;
                            while(!pointer->operands.empty()){
                                pointer = &*(pointer->operands.end()-1);
                            }
                            lexer.makeError("Excepted ')':",*pointer);
                        }
                    }

                    --it;
                    goto PUSH_BOTH;
                }else{
                    int in = 1;
                    auto b = ++it;
                    while (true) {
                        if (it->token == Node::BLOCK) {
                            if (it->str == "(")
                                ++in;
                            else if (it->str == ")")
                                --in;
                            else
                                throw std::runtime_error("Parser::parseLine");

                            if (!in) {
                                auto diff = end-it;
                                parseLine(b,it,line);
                                --b;
                                --it;
                                *b = std::move(*it);
                                line.erase(it,it+2);
                                --it;
                                end = it+diff;

                                break;
                            }
                        }
                        ++it;
                        if (it >= line.end()) {
                            --it;
                            Node *pointer = &*it;
                            while(!pointer->operands.empty()){
                                pointer = &*(pointer->operands.end()-1);
                            }
                            lexer.makeError("Excepted ')':",*pointer);
                        }
                    }
                }
                break;
            case Node::OPERATOR:
                switch (it->specialToken) {
                    case Node::OP_LEFT: {
                        auto before = it - 1;
                        it->operands.push_back(std::move(*before));
                        line.erase(before);
                        //--it;//standard 23.1.2
                        --end;
                    }
                        break;
                    case Node::OP_RIGHT: {
                        auto after = it + 1;
                        it->operands.push_back(std::move(*after));
                        line.erase(after);
                        //no need to change it | standard 23.1.2
                        --end;
                    }
                        break;
                    case Node::OP_BOTH: {
                        PUSH_BOTH:auto after = it + 1;
                        auto before = it - 1;
                        it->operands.push_back(std::move(*before));
                        it->operands.push_back(std::move(*after));
                        *before = std::move(*it);
                        line.erase(it,++after);
                        //--it;//standard 23.1.2
                        end-=2;
                    }
                        break;
                    default:
                        lexer.makeError("Unknown operator '" + it->str + "':",*it);
                }
                break;
            case Node::KEYWORD:{

            }
                break;
            default:
                lexer.makeError("Unexpected element '" + it->str + "':",*it);
        }
    }
}

void Parser::setOperatorKind(const std::vector<Node>::iterator &begin,const std::vector<Node>::iterator &end,const std::vector<Node>::iterator &it) {
    const auto& before = *(it-1);
    auto after = it+1;
    if(it->token == Node::BLOCK){
        it->specialToken = Node::OP_BETWEEN;
    }else if(it == begin || !isOperand(before)){
        if(it == end || after == end || !isOperand(*after)){
            return;
        }else{
            it->specialToken = Node::OP_RIGHT;
        }
    }else if(it == end || !isOperand(*after)){
        it->specialToken = Node::OP_LEFT;
    }else{
        it->specialToken = Node::OP_BOTH;
    }
}

bool Parser::isOperand(const Node &node) {
    switch (node.token) {
        case Node::STRING:
        case Node::NUMBER:
        case Node::BOOL:
        case Node::ID:
            return true;
        case Node::BLOCK:
        case Node::OPERATOR:
            return !node.operands.empty();
        default:
            return false;
    }
}

std::vector<Node>::const_iterator Parser::isInBy_string_token(const std::vector<Node> &vec, const std::vector<Node>::const_iterator &it) {
    for(auto temp = vec.cbegin(), end = vec.cend();temp < end;++temp)
        if(temp->token == it->token && temp->str == it->str)
            return temp;

    return vec.cend();
}
