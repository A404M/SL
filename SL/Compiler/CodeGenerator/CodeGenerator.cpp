//
// Created by a404m on 9/19/21.
//

#include "CodeGenerator.h"

using namespace SL;

CodeGenerator::CodeGenerator(Parser& p) : holder({}), parser(std::move(p)), lastHelper(0){
    for(auto& node : parser.holder){
        generateByteCode(node);
        deleteByteCodeHelpers(holder);
    }
}

void CodeGenerator::generateByteCode(Node &node) {
    if(node.token == Node::OPERATOR){
        if(node == Node::OP_POSITIVE){
            generateByteCode(node,POSITIVE);
        }else if(node == Node::OP_NEGATIVE){
            generateByteCode(node,NEGATIVE);
        }else if(node == Node::OP_LOGICAL_NOT){
            generateByteCode(node,LOGICAL_NOT);
        }else if(node == Node::OP_MULTIPLICATION){
            generateByteCode(node,MULTIPLICATION);
        }else if(node == Node::OP_DEVIATION){
            generateByteCode(node,DEVIATION);
        }else if(node == Node::OP_PLUS){
            generateByteCode(node,PLUS);
        }else if(node == Node::OP_MINUS){
            generateByteCode(node,MINUS);
        }else if(node == Node::OP_GREATER_OR_EQUAL){
            generateByteCode(node,GREATER_OR_EQUAL);
        }else if(node == Node::OP_SMALLER_OR_EQUAL){
            generateByteCode(node,SMALLER_OR_EQUAL);
        }else if(node == Node::OP_GREATER){
            generateByteCode(node,GREATER);
        }else if(node == Node::OP_SMALLER){
            generateByteCode(node,SMALLER);
        }else if(node == Node::OP_EQUAL){
            generateByteCode(node,EQUAL);
        }else if(node == Node::OP_NOT_EQUAL){
            generateByteCode(node,NOT_EQUAL);
        }else if(node == Node::OP_LOGICAL_AND){
            generateByteCode(node,LOGICAL_AND);
        }else if(node == Node::OP_LOGICAL_OR){
            generateByteCode(node,LOGICAL_OR);
        }else if(node == Node::OP_ASSIGN){
            generateMoveByteCode(node);
        }
    }
    else if(node.token == Node::BLOCK){
        if(node == Node::BK_PARENTHESIS){
            generateByteCode(node,NATIVE_CALL);
        }
    }
    else if(node.token == Node::KEYWORD){

    }
}

void CodeGenerator::generateByteCode(Node &node, Command command) {
    switch (node.specialToken) {
        case Node::OP_LEFT:
        case Node::OP_RIGHT:
            if(node.operands[0].operands.empty()){
                createHelperTo(node.operands[0],holder);
            }else{
                generateByteCode(node.operands[0]);
            }
            holder += command;
            setHelperName(lastHelper,holder);
            if(node.token == Node::KEYWORD){
                deleteLastHelper(holder);
            }
            break;
        case Node::OP_BOTH://order is matter
            if(node.operands[0].operands.empty()){
                createHelperTo(node.operands[0],holder);
                if(node.operands[1].operands.empty()){
                    if(node.operands[1].token == Node::ID){
                        holder += command;
                        setHelperName(lastHelper,holder);
                        setCodeName(node.operands[1].str,holder);
                    }else{
                        createHelperTo(node.operands[1],holder);
                        holder += command;
                        setHelperName(lastHelper-1,holder);
                        setHelperName(lastHelper,holder);
                        deleteLastHelper(holder);
                    }
                }else{
                    generateByteCode(node.operands[1]);
                    holder += command;
                    setHelperName(lastHelper-1,holder);
                    setHelperName(lastHelper,holder);
                    deleteLastHelper(holder);
                }
            }else if(node.operands[1].operands.empty()){
                generateByteCode(node.operands[0]);
                if(node.operands[1].token == Node::ID){
                    holder += command;
                    setHelperName(lastHelper,holder);
                    setCodeName(node.operands[1].str,holder);
                }else{
                    createHelperTo(node.operands[1],holder);
                    holder += command;
                    setHelperName(lastHelper-1,holder);
                    setHelperName(lastHelper,holder);
                    deleteLastHelper(holder);
                }
            }else{
                generateByteCode(node.operands[0]);
                generateByteCode(node.operands[1]);
                holder += command;
                setHelperName(lastHelper-1,holder);
                setHelperName(lastHelper,holder);
                deleteLastHelper(holder);
            }
            break;
        case Node::OP_BETWEEN:
            if(node.operands[0].token != Node::ID){
                parser.lexer.makeError("Expected function name:",node.operands[0]);
            }
            if(node.operands[1].isEmpty()){
                holder += command;
                setHelperName(++lastHelper,holder);
                setCodeName(node.operands[0].str,holder);
                holder += (char)0;//zero argument
            }else{
                int helpers = lastHelper;
                std::string arguments;
                arguments = (char)0;
                getArguments(node.operands[1], arguments);
                holder += command;
                setHelperName(++lastHelper,holder);//returned value
                setCodeName(node.operands[0].str,holder);
                holder += arguments;
                //deleting helpers
                holder += MOVE_VAR;
                setHelperName(++helpers,holder);
                setHelperName(lastHelper,holder);
                while (lastHelper > helpers) {
                    deleteLastHelper(holder);
                }
                //the last helper is returned value
            }
            break;
        case Node::INVALID:
        default:
            parser.lexer.makeError("Unexpected element:",node);
    }
}

void CodeGenerator::generateMoveByteCode(Node &node) {
    if(node.operands[0].operands.empty() && node.operands[0].token == Node::ID) {
        if (node.operands[1].operands.empty()) {
            switch (node.operands[1].token) {
                case Node::STRING:
                    holder += MOVE_STRING;
                    setCodeName(node.operands[0].str,holder);
                    setString(node.operands[1].str,holder);
                    break;
                case Node::NUMBER:
                    holder += MOVE_NUMBER;
                    setCodeName(node.operands[0].str,holder);
                    setNumber(node.operands[1].str,holder);
                    break;
                case Node::BOOL:
                    holder += MOVE_BOOL;
                    setCodeName(node.operands[0].str,holder);
                    setBool(node.operands[1].str,holder);
                    break;
                case Node::ID:
                    holder += MOVE_VAR;
                    setCodeName(node.operands[0].str,holder);
                    setCodeName(node.operands[1].str,holder);
                default:
                    parser.lexer.makeError("Unexpected element:",node);
            }
            holder += MOVE_VAR;
            setHelperName(++lastHelper,holder);
            setCodeName(node.operands[0].str,holder);
        }else{
            generateByteCode(node.operands[1]);
            holder += MOVE_VAR;
            setCodeName(node.operands[0].str,holder);
            setHelperName(lastHelper,holder);
            //no need to delete helper
        }
    }else{
        parser.lexer.makeError("Expected variable name:",node);
    }
}

void CodeGenerator::createHelperTo(Node &node,std::string &result) {
    switch(node.token){
        case Node::ID:
            holder += MOVE_VAR;
            setHelperName(++lastHelper,result);
            setCodeName(node.str,result);
            break;
        case Node::STRING:
            holder += MOVE_STRING;
            setHelperName(++lastHelper,result);
            setString(node.str,result);
            break;
        case Node::NUMBER:
            holder += MOVE_NUMBER;
            setHelperName(++lastHelper,result);
            setNumber(node.str,result);
            break;
        case Node::BOOL:
            holder += MOVE_BOOL;
            setHelperName(++lastHelper,result);
            setBool(node.str,result);
            break;
        default:
            parser.lexer.makeError("Unexpected element:",node);
    }
}

void CodeGenerator::deleteLastHelper(std::string &result) {
    result += DELETE;
    setHelperName(lastHelper--,result);
}

void CodeGenerator::setHelperName(int name, std::string &result) {
    setCodeName(std::to_string(name),result);
}

void CodeGenerator::deleteByteCodeHelpers(std::string &result) {
    while(lastHelper){
        deleteLastHelper(result);
    }
}

void CodeGenerator::setCodeName(const std::string &str,std::string &result) {
    result += (char)str.size();
    result += str;
}

void CodeGenerator::setString(const std::string &str,std::string &result) {
    result += str.size();
    result += str;
}

void CodeGenerator::setNumber(const std::string &str,std::string &result) {
    const Number number = stold(str);
    unsigned char temp[sizeof(Number)];
    std::copy(
            reinterpret_cast<const unsigned char*>(&number),
            reinterpret_cast<const unsigned char*>(&number) + sizeof(Number),
            &temp[0]
    );
    result += std::string(reinterpret_cast<char*>(&temp),sizeof(Number) / sizeof(char));
}

void CodeGenerator::setBool(const std::string &str,std::string &result) {
    result += (str == "true");
}

void CodeGenerator::getArguments(Node &node, std::string &arguments) {
    switch (node.token) {
        case Node::STRING:
        case Node::NUMBER:
        case Node::BOOL:
            createHelperTo(node,holder);
            setHelperName(lastHelper,arguments);
            ++arguments[0];
            break;
        case Node::BLOCK:
        case Node::OPERATOR:
            if(node == Node::OP_COMMA){
                getArguments(node.operands[0],arguments);
                getArguments(node.operands[1],arguments);
            }else{
                generateByteCode(node);
                setHelperName(lastHelper, arguments);
                ++arguments[0];
            }
            break;
        case Node::ID:
            setCodeName(node.str,arguments);
            ++arguments[0];
            break;
        default:
            parser.lexer.makeError("Unexpected element in function arguments:",node);
    }
}
