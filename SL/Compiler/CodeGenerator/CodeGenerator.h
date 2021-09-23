//
// Created by a404m on 9/19/21.
//

#ifndef SL_CODEGENERATOR_H
#define SL_CODEGENERATOR_H


#include "../Parser/Parser.h"
#include "../additional/enums.h"

namespace SL {
    class CodeGenerator {
        friend class Runner;
    private:
    public:
        std::string holder;
        Parser parser;
        int lastHelper{};
    public:
        explicit CodeGenerator(Parser&);
    private:
        void generateByteCode(Node&);
        void generateByteCode(Node&,Command);
        void generateMoveByteCode(Node&);
        void createHelperTo(Node&, std::string&);
        void deleteLastHelper(std::string&);
        inline void setHelperName(int, std::string&);
        inline void deleteByteCodeHelpers(std::string&);
        inline void setCodeName(const std::string &, std::string&);
        inline void setString(const std::string &, std::string&);
        inline void setNumber(const std::string &, std::string&);
        inline void setBool(const std::string &, std::string&);
        void getArguments(Node &node, std::string&);
    };
}//namespace SL


#endif //SL_CODEGENERATOR_H
