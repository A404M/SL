//
// Created by a404m on 9/19/21.
//

#ifndef SL_RUNNER_H
#define SL_RUNNER_H


#include "../../Compiler/CodeGenerator/CodeGenerator.h"
#include "../Var/Var.h"
#include <map>

namespace SL {
    class Runner {
    private:
        std::map<std::string,Var> varHolder;
        CodeGenerator code;
        std::string::const_iterator it,begin,end;
    public:
        explicit Runner(CodeGenerator&);

    private:
        void getNextNameOrString(std::string&);

        void print(const std::string&,const std::vector<std::string>&);
        void stringInput(const std::string&,const std::vector<std::string>&);
        void numberInput(const std::string&,const std::vector<std::string>&);
        void booleanInput(const std::string&,const std::vector<std::string>&);
    };
}//namespace SL


#endif //SL_RUNNER_H
