//
// Created by a404m on 9/19/21.
//

#include "Var.h"

using namespace SL;

Var::Var(const String &str) : type(STRING), holder(str) {/*empty*/}

Var::Var(const Number &num) : type(NUMBER), holder(num) {/*empty*/}

Var::Var(Bool b) : type(BOOL), holder(b) {/*empty*/}

Var::Var(const Var &var) = default;

void Var::positive() {
    switch (type) {
        case NUMBER:
            holder = +std::get<Number>(holder);
            break;
        default:
            throw std::runtime_error("Var::positive");
    }
}

void Var::negative() {
    switch (type) {
        case NUMBER:
            holder = -std::get<Number>(holder);
            break;
        default:
            throw std::runtime_error("Var::negative");
    }
}

void Var::logicalNot() {
    switch (type) {
        case BOOL:
            holder = !std::get<Bool>(holder);
            break;
        default:
            throw std::runtime_error("Var::logicalNot");
    }
}

void Var::multiplication(const Var &var) {
    if(type == var.type && type == NUMBER){
        std::get<Number>(holder) *= std::get<Number>(var.holder);
    }else{
        throw std::runtime_error("Var::multiplication");
    }
}

void Var::deviation(const Var &var) {
    if(type == var.type && type == NUMBER){
        std::get<Number>(holder) /= std::get<Number>(var.holder);
    }else{
        throw std::runtime_error("Var::deviation");
    }
}

void Var::plus(const Var &var) {
    if(type == var.type){
        if(type == NUMBER){
            std::get<Number>(holder) += std::get<Number>(var.holder);
        }else if(type == STRING){
            std::get<String>(holder) += std::get<String>(var.holder);
        }else{
            goto THROW;
        }
    }else{
        THROW:throw std::runtime_error("Var::plus");
    }
}

void Var::minus(const Var &var) {
    if(type == var.type && type == NUMBER){
        std::get<Number>(holder) -= std::get<Number>(var.holder);
    }else{
        throw std::runtime_error("Var::minus");
    }
}

void Var::greater(const Var &var) {
    if(type == var.type)
        *this = Var(holder > var.holder);
    else
        throw std::runtime_error("Var::greater");
}

void Var::smaller(const Var &var) {
    if(type == var.type)
        *this = Var(holder < var.holder);
    else
        throw std::runtime_error("Var::smaller");
}

void Var::greaterOrEqual(const Var &var) {
    if(type == var.type)
        *this = Var(holder >= var.holder);
    else
        throw std::runtime_error("Var::greaterOrEqual");
}

void Var::smallerOrEqual(const Var &var) {
    if(type == var.type)
        *this = Var(holder <= var.holder);
    else
        throw std::runtime_error("Var::smallerOrEqual");
}

void Var::equal(const Var &var) {
    *this = Var(type == var.type && holder == var.holder);
}

void Var::notEqual(const Var &var) {
    *this = Var(type != var.type || holder != var.holder);
}

void Var::logicalAnd(const Var &var) {
    if(type == var.type && type == BOOL)
        *this = Var(std::get<Bool>(holder) && std::get<Bool>(var.holder));
    else
        throw std::runtime_error("Var::logicalAnd");
}

void Var::logicalOr(const Var &var) {
    if(type == var.type && type == BOOL)
        *this = Var(std::get<Bool>(holder) || std::get<Bool>(var.holder));
    else
        throw std::runtime_error("Var::logicalOr");
}

void Var::move(const Var &var) {
    *this = var;
}

void Var::print() {
    switch (type) {
        case STRING:
            *output << std::get<String>(holder);
            break;
        case NUMBER:
            *output << std::get<Number>(holder);
            break;
        case BOOL:
            *output << (std::get<Bool>(holder)? "true" : "false");
            break;
        default:
            throw std::runtime_error("Var::print");
    }
}

void Var::pow(const Var &var) {
    int p = std::get<Number>(var.holder);
    Number value = std::get<Number>(holder);
    Number result = 1;
    while(p > 0){
        result *= value;
        --p;
    }
    holder = result;
}
