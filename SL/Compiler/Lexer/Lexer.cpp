//
// Created by a404m on 9/19/21.
//

#include "Lexer.h"

using namespace SL;

Lexer::Lexer(std::string &str) : code(std::move(str)),holder({}) {
    code += "\n";
    Node node;
    for(auto it = code.begin(),begin = it,end = code.end();it < end;++it){
        auto &current = *it;
        if(node.token == Node::STRING){
            if(current == '\\'){
                ++it;
                switch (*it) {
                    case 'n':
                        node.str += '\n';
                        break;
                    case 't':
                        node.str += '\t';
                        break;
                    case 'v':
                        node.str += '\v';
                        break;
                    case 'b':
                        node.str += '\b';
                        break;
                    case 'r':
                        node.str += '\r';
                        break;
                    case 'a':
                        node.str += '\a';
                        break;
                    case '\\':
                        node.str += '\\';
                        break;
                    case '\"':
                        node.str += '\"';
                        break;
                    case '\'':
                        node.str += '\'';
                        break;
                    default:
                        node.index = it-begin;
                        makeError(std::string("Unknown char \\") + *it + "in String:",node);
                }
                continue;
            }else if(node.str[0] == current){
                node.str.erase(0,1);//erase " or '
                node.index = it-begin;
                push_clear(node);
                continue;
            }
        }
        else if(isStringMarker(current)){
            node.index = it-begin;
            push_clear_ifnempty(node);
            node.token = Node::STRING;
        }
        else if(current == '\n'){
            node.index = it-begin;
            push_clear_ifnempty(node);
            node.token = Node::SYMBOL;
        }else if(current == '\\'){
            if(*(++it) == '\n'){
                ++it;
                continue;
            }else{
                makeError("Unexpected character '\\':",node);
            }
        }
        else if(isSpace(current)){
            node.index = it-begin;
            push_clear_ifnempty(node);
            continue;
        }
        else if(isNumber(current) || (node.token == Node::NUMBER && current == '.')){
            if(node.token == Node::ID) {
                //goto PUSH;
            }else if(node.token != Node::NUMBER) {
                node.index = it-begin;
                push_clear_ifnempty(node);
                node.token = Node::NUMBER;
            }
        }
        else if(isIdentifier(current)){
            if(node.token != Node::ID){
                node.index = it-begin;
                push_clear_ifnempty(node);
                node.token = Node::ID;
            }
        }
        else if(isOperator(current)){
            if(node.token != Node::OPERATOR || !isOperator(node.str+*it)){
                if(node.str+current == "//"){
                    for(;it < end && *it != '\n';++it);
                    node.clear();
                    continue;
                }
                node.index = it-begin;
                push_clear_ifnempty(node);
                node.token = Node::OPERATOR;
            }
        }
        else if(isBlock(current)){
            node.index = it-begin;
            push_clear_ifnempty(node);//blocks have size = 1
            node.token = Node::BLOCK;
        }
        else if(isSymbol(current)){
            node.index = it-begin;
            push_clear_ifnempty(node);
            node.token = Node::SYMBOL;
        }else{
            node.index = it-begin;
            makeError(std::string("Unknown character \'")+*it+"\':",node);
        }
        //PUSH:
        node.str += current;
    }
    if(!line.empty()){
        holder.push_back(line);
    }
}

void Lexer::makeError(std::string errorMessage,const Node &node) {
    int lineNumber = 0;
    errorMessage += '\n';
    std::string temp;
    for(auto it = code.begin() + node.index,begin = code.begin(); it >= begin; --it){
        if(*it == '\n'){
            ++lineNumber;
        }else if(!lineNumber){
            temp += *it;
        }
    }
    for(auto rit = temp.rbegin(),rend = temp.rend();rit < rend;++rit){
        if(*rit == '\n'){
            ++lineNumber;
        }else if(!lineNumber){
            errorMessage += *rit;
        }
    }
    std::string::size_type arrowPadding = temp.size()-1;
    temp.clear();
    for(auto it = code.begin()+node.index+1,end = code.end();it < end && *it != '\n';errorMessage+=*it,++it);
    errorMessage += '\n';
    errorMessage += std::string(arrowPadding,' ');
    errorMessage += "^ line ";
    errorMessage += std::to_string(lineNumber);
    throw std::runtime_error(errorMessage);
}

void Lexer::push_clear(Node &node) {
    if(node.token == Node::ID) {
        if (isKeyword(node.str))
            node.token = Node::KEYWORD;
        else if (isBoolean(node.str))
            node.token = Node::BOOL;
    }else if(node.token == Node::SYMBOL){
        if(node.str == ";" || node.str == "\n"){
            holder.push_back(line);
            line.clear();
            node.clear();
            return;
        }
    }
    line.push_back(node);
    node.clear();
}

void Lexer::push_clear_ifnempty(Node &node) {
    if(!node.isEmpty()){
        push_clear(node);
    }
}