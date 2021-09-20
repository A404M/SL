//
// Created by a404m on 9/19/21.
//

#ifndef SL_RUNNER_H
#define SL_RUNNER_H


#include "../../Compiler/CodeGenerator/CodeGenerator.h"
#include "../Var/Var.h"

namespace SL {
    class Runner {
    private:
        std::map<std::string,Var> varHolder;
        std::string::const_iterator it,begin,end;
        CodeGenerator code;
    public:
        explicit Runner(CodeGenerator);

    private:
        void getNextNameOrString(std::string&);
    };
}//namespace SL


#endif //SL_RUNNER_H
