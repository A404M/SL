//
// Created by a404m on 9/19/21.
//

#include "Parser.h"

#include <utility>

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

Parser::Parser(Lexer l) : lexer(std::move(l)) {
    for(auto& line : lexer.holder) {
        parseLine(line);
        holder.push_back(line[0]);
    }
    lexer.clear();
}

void Parser::parseLine(std::vector<Node>& line) {
    auto orderIt = orders.begin();

    for (auto it = line.end(),begin = line.begin(),end = it;line.size() > 1;end = line.end(),it = end){
        for(auto temp = begin;temp < end;++temp){
            if(temp->operands.empty()) {
                setOperatorKind(begin, end, temp);
                auto founded = isInBy_string_token(*orderIt, temp);
                if (founded != orderIt->cend()) {
                    if (*founded == *temp) {
                        it = temp;
                        if(it->token == Node::OPERATOR && it->str == "="){//left associative
                            continue;
                        }else{
                            break;
                        }
                    } else {
                        temp->specialToken = Node::INVALID;
                    }
                }
            }
        }
        if(it == end){
            ++orderIt;
            if(orderIt == orders.end()){
                Node *pointer = &*line.begin();
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
                    std::vector<Node> tempLine;
                    while (true) {
                        if (it->token == Node::BLOCK) {
                            if (it->str == "(")
                                ++in;
                            else if (it->str == ")")
                                --in;
                            else
                                throw std::runtime_error("Parser::parseLine");

                            if (!in) {
                                if(tempLine.empty()){
                                    it->clear();
                                }else {
                                    parseLine(tempLine);
                                    *it = tempLine[0];
                                }
                                break;
                            }
                        }
                        if (it >= line.end()) {
                            --it;
                            Node *pointer = &*it;
                            while(!pointer->operands.empty()){
                                pointer = &*(pointer->operands.end()-1);
                            }
                            lexer.makeError("Excepted ')':",*pointer);
                        }
                        tempLine.push_back(*it);
                        line.erase(it);
                    }

                    --it;
                    it->operands.push_back(*(it-1));
                    it->operands.push_back(*(it+1));
                    line.erase(it+1);
                    line.erase(it-1);
                    --it;//standard 23.1.2
                }else{
                    int in = 1;
                    line.erase(it);
                    std::vector<Node> tempLine;
                    while (true) {
                        if (it->token == Node::BLOCK) {
                            if (it->str == "(")
                                ++in;
                            else if (it->str == ")")
                                --in;
                            else
                                throw std::runtime_error("Parser::parseLine");

                            if (!in) {
                                parseLine(tempLine);
                                *it = tempLine[0];
                                break;
                            }
                        }
                        if (it >= line.end()) {
                            --it;
                            Node *pointer = &*it;
                            while(!pointer->operands.empty()){
                                pointer = &*(pointer->operands.end()-1);
                            }
                            lexer.makeError("Excepted ')' :",*pointer);
                        }
                        tempLine.push_back(*it);
                        line.erase(it);
                    }
                }
                break;
            case Node::OPERATOR:
                switch (it->specialToken) {
                    case Node::OP_LEFT:
                        it->operands.push_back(*(it-1));
                        line.erase(it-1);
                        --it;//standard 23.1.2
                        break;
                    case Node::OP_RIGHT:
                        it->operands.push_back(*(it+1));
                        line.erase(it+1);
                        //no need to change it | standard 23.1.2
                        break;
                    case Node::OP_BOTH:
                        it->operands.push_back(*(it-1));
                        it->operands.push_back(*(it+1));
                        line.erase(it+1);
                        line.erase(it-1);
                        --it;//standard 23.1.2
                        break;
                    default:
                        lexer.makeError("Unknown operator '" + it->str + "':",*it);
                }
                break;
            case Node::KEYWORD:{
                if(it->str == "print"){
                    it->operands.push_back(*(it+1));
                    line.erase(it+1);
                }
            }
                break;
            default:
                lexer.makeError("Unexpected element '" + it->str + "':",*it);
        }
    }
}

void Parser::setOperatorKind(const std::vector<Node>::iterator &begin,const std::vector<Node>::iterator &end,const std::vector<Node>::iterator &it) {
    if(it->token == Node::BLOCK){
        it->specialToken = Node::OP_BETWEEN;
    }else if(it == begin || !isOperand(*(it-1))){
        if(it == end || !isOperand(*(it+1))){
            return;
        }else{
            it->specialToken = Node::OP_RIGHT;
        }
    }else if(it == end || !isOperand(*(it+1))){
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