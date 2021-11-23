//
// Created by a404m on 9/19/21.
//

#ifndef SL_VAR_H
#define SL_VAR_H


#include <iostream>
#include <string>
#include <variant>
#include "../../Compiler/additional/types.h"

namespace SL {
    class Var {
    public:
        static std::ostream *output;
        static std::istream *input;
    public:
        enum Type{
            STRING = 1,
            NUMBER,
            BOOL
        };
    private:
        Type type;
        std::variant<std::string, Number, bool> holder;
    public:
        explicit Var(const String&);
        explicit Var(const Number& = 0);
        explicit Var(Bool);

        Var(const Var&);

        void positive();
        void negative();
        void logicalNot();

        void multiplication(const Var&);
        void deviation(const Var&);

        void plus(const Var&);
        void minus(const Var&);

        void greater(const Var&);
        void smaller(const Var&);
        void greaterOrEqual(const Var&);
        void smallerOrEqual(const Var&);

        void equal(const Var&);
        void notEqual(const Var&);

        void logicalAnd(const Var&);
        void logicalOr(const Var&);

        void move(const Var&);

        void print();

        void pow(const Var&);

        /*bool operator==(const Var&);
        bool operator>(const Var&);
        bool operator<(const Var&);
        bool operator>=(const Var&);
        bool operator<=(const Var&);*/
    };
}


#endif //SL_VAR_H
