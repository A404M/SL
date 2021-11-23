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
    public:
        typedef std::map<std::string,Var> VarHolder;
        typedef void (*FunctionPointer)(const std::string&,const std::vector<std::string>&,VarHolder&);
    private:
        VarHolder varHolder;
        CodeGenerator code;
        std::string::const_iterator it,begin,end;
        static const std::map<std::string,FunctionPointer> functions;
    public:
        explicit Runner(CodeGenerator&);

    private:
        void getNextNameOrString(std::string&);

        static void print(const std::string&,const std::vector<std::string>&,VarHolder&);
        static void stringInput(const std::string&,const std::vector<std::string>&,VarHolder&);
        static void numberInput(const std::string&,const std::vector<std::string>&,VarHolder&);
        static void booleanInput(const std::string&,const std::vector<std::string>&,VarHolder&);
        static void pow(const std::string&,const std::vector<std::string>&,VarHolder&);
    };
}//namespace SL


#endif //SL_RUNNER_H
