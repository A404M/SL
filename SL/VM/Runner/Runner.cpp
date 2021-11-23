//
// Created by a404m on 9/19/21.
//

#include "Runner.h"

using namespace SL;

const std::map<std::string,void (*)(const std::string&,const std::vector<std::string>&,Runner::VarHolder&)> Runner::functions{
        {"print",print},
        {"stringInput",stringInput},
        {"numberInput",numberInput},
        {"booleanInput",booleanInput},
        {"pow",pow}
};

Runner::Runner(CodeGenerator& c) : varHolder({}), code(std::move(c)),
                it(code.holder.begin()),begin(it),end(code.holder.end()) {
    Command command;
    std::string word1,word2;
    while(it < end){
        command = (Command)(*it);
        ++it;
        switch (command) {
            case POSITIVE:
                getNextNameOrString(word1);
                varHolder[word1].positive();
                break;
            case NEGATIVE:
                getNextNameOrString(word1);
                varHolder[word1].negative();
                break;
            case LOGICAL_NOT:
                getNextNameOrString(word1);
                varHolder[word1].logicalNot();
                break;
            case MULTIPLICATION:
                getNextNameOrString(word1);
                getNextNameOrString(word2);
                varHolder[word1].multiplication(varHolder[word2]);
                break;
            case DEVIATION:
                getNextNameOrString(word1);
                getNextNameOrString(word2);
                varHolder[word1].deviation(varHolder[word2]);
                break;
            case PLUS:
                getNextNameOrString(word1);
                getNextNameOrString(word2);
                varHolder[word1].plus(varHolder[word2]);
                break;
            case MINUS:
                getNextNameOrString(word1);
                getNextNameOrString(word2);
                varHolder[word1].minus(varHolder[word2]);
                break;
            case GREATER:
                getNextNameOrString(word1);
                getNextNameOrString(word2);
                varHolder[word1].greater(varHolder[word2]);
                break;
            case SMALLER:
                getNextNameOrString(word1);
                getNextNameOrString(word2);
                varHolder[word1].smaller(varHolder[word2]);
                break;
            case GREATER_OR_EQUAL:
                getNextNameOrString(word1);
                getNextNameOrString(word2);
                varHolder[word1].greaterOrEqual(varHolder[word2]);
                break;
            case SMALLER_OR_EQUAL:
                getNextNameOrString(word1);
                getNextNameOrString(word2);
                varHolder[word1].smallerOrEqual(varHolder[word2]);
                break;
            case EQUAL:
                getNextNameOrString(word1);
                getNextNameOrString(word2);
                varHolder[word1].equal(varHolder[word2]);
                break;
            case NOT_EQUAL:
                getNextNameOrString(word1);
                getNextNameOrString(word2);
                varHolder[word1].notEqual(varHolder[word2]);
                break;
            case LOGICAL_AND:
                getNextNameOrString(word1);
                getNextNameOrString(word2);
                varHolder[word1].logicalAnd(varHolder[word2]);
                break;
            case LOGICAL_OR:
                getNextNameOrString(word1);
                getNextNameOrString(word2);
                varHolder[word1].logicalOr(varHolder[word2]);
                break;
            case MOVE_NUMBER: {
                getNextNameOrString(word1);
                word2 = code.holder.substr(it - begin, sizeof(Number));
                it += sizeof(Number);
                auto num = reinterpret_cast<const Number *>(word2.c_str())[0];
                varHolder[word1].move(Var(num));
            }
                break;
            case MOVE_STRING:
                getNextNameOrString(word1);
                getNextNameOrString(word2);
                varHolder[word1].move(Var(word2));
                break;
            case MOVE_BOOL:
                getNextNameOrString(word1);
                varHolder[word1].move(Var(*it == 1));
                ++it;
                break;
            case MOVE_VAR:
                getNextNameOrString(word1);
                getNextNameOrString(word2);
                varHolder[word1].move(varHolder[word2]);
                break;
            case DELETE:
                getNextNameOrString(word1);
                varHolder.erase(word1);
                break;
            case NATIVE_CALL: {
                getNextNameOrString(word1);//returned value
                getNextNameOrString(word2);//function name
                int argumentsNumber = (unsigned char)*it;
                ++it;
                std::vector<std::string> args;
                std::string temp;
                while(argumentsNumber){
                    getNextNameOrString(temp);
                    args.push_back(temp);
                    --argumentsNumber;
                }
                (*(functions.at(word2)))(word1,args,varHolder);
            }
                break;
            default:
                throw std::runtime_error("Runner::Runner");
        }
    }
}

void Runner::getNextNameOrString(std::string &str) {
    auto size = *it;
    ++it;
    str = code.holder.substr(it-begin,size);
    it += size;
}

void Runner::print(const std::string &,const std::vector<std::string> &args,VarHolder &varH){
    for(const auto& var : args){
        varH[var].print();
    }
    //no returnVar
}

void Runner::stringInput(const std::string &returnVar,const std::vector<std::string> &,VarHolder &varH) {
    std::string temp;
    *Var::input >> temp;
    varH[returnVar] = Var(temp);
}

void Runner::numberInput(const std::string &returnVar,const std::vector<std::string> &,VarHolder &varH) {
    Number temp;
    *Var::input >> temp;
    if(std::cin.fail()){
        throw std::runtime_error("Runner::numberInput");
    }
    varH[returnVar] = Var(temp);
}

void Runner::booleanInput(const std::string &returnVar,const std::vector<std::string> &,VarHolder &varH) {
    std::string temp;
    *Var::input >> temp;
    if(temp == "true"){
        varH[returnVar] = Var(true);
    }else if(temp == "false"){
        varH[returnVar] = Var(false);
    }else{
        throw std::runtime_error("Runner::booleanInput");
    }

}

void Runner::pow(const std::string &returnVar,const std::vector<std::string> &args,VarHolder &varH) {
    auto number = varH[args[0]];
    auto p = varH[args[0]];
    varH[args[0]].pow(varH[args[1]]);
    varH[returnVar] = varH[args[0]];
}
