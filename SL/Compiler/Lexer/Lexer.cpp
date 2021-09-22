//
// Created by a404m on 9/19/21.
//

#include "Lexer.h"

using namespace SL;

Lexer::Lexer(const std::string &str) : holder({}),primaryHolder({}),code(str+" ") {
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
        }else if(current == '\n'){
            node.index = it-begin;
            push_clear_ifnempty(node);
            node.token = Node::SYMBOL;
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
    int in = 0;
    std::vector<Node> line;
    for(auto it = primaryHolder.begin(),begin = it,end = primaryHolder.end();it < end;++it){
        if(it->token == Node::SYMBOL){
            if(it->str == "\n"){
                if(it == begin || in || ((it+1)->token == Node::SYMBOL && (it+1)->str == "\n") || (it+1)->isEqualByTokenAndString(Node::OP_COMMA) || (line.end()-1)->isEqualByTokenAndString(Node::OP_COMMA)){
                    continue;
                }else{
                    if (!line.empty()) {
                        holder.push_back(line);
                        line.clear();
                    }
                }
            }else{
                if (it->str == ";") {
                    if (!line.empty()) {
                        holder.push_back(line);
                        line.clear();
                    }
                }
            }
        }else{
            line.push_back(*it);
            if (it->token == Node::BLOCK) {
                if (it->str == "(")
                    ++in;
                else if (it->str == ")")
                    --in;
            }
        }
    }
    if(in){
        makeError("Excepted ')' :",*(line.end()-1));
    }
    if (!line.empty()) {
        holder.push_back(line);
        line.clear();
    }
    primaryHolder.clear();
}

void Lexer::clear() {
    holder.clear();
}

void Lexer::makeError(std::string errorMessage,const Node &node) {
    int lineNumber = 0;
    errorMessage += '\n';
    std::string temp;
    for(auto it = code.begin() + node.index,begin = code.begin(); it > begin; --it){
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

bool Lexer::isEmpty() {
    return holder.empty();
}

void Lexer::push_clear(Node &node) {
    if(node.token == Node::ID) {
        if (isKeyword(node.str))
            node.token = Node::KEYWORD;
        else if (isBoolean(node.str))
            node.token = Node::BOOL;
    }
    primaryHolder.push_back(node);
    node.clear();
}

void Lexer::push_clear_ifnempty(Node &node) {
    if(!node.isEmpty()){
        push_clear(node);
    }
}