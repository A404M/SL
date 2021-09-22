//
// Created by a404m on 9/19/21.
//

#include "Node.h"

using namespace SL;

const Node Node::BK_PARENTHESIS = Node{"(", Node::BLOCK, Node::OP_BETWEEN};
const Node Node::OP_POSITIVE = Node{"+", Node::OPERATOR, Node::OP_RIGHT};
const Node Node::OP_NEGATIVE = Node{"-", Node::OPERATOR, Node::OP_RIGHT};
const Node Node::OP_LOGICAL_NOT = Node{"!", Node::OPERATOR, Node::OP_RIGHT};
const Node Node::OP_MULTIPLICATION = Node{"*", Node::OPERATOR, Node::OP_BOTH};
const Node Node::OP_DEVIATION = Node{"/", Node::OPERATOR, Node::OP_BOTH};
const Node Node::OP_PLUS = Node{"+", Node::OPERATOR, Node::OP_BOTH};
const Node Node::OP_GREATER = Node{">",Node::OPERATOR,Node::OP_BOTH};
const Node Node::OP_SMALLER = Node{"<",Node::OPERATOR,Node::OP_BOTH};
const Node Node::OP_GREATER_OR_EQUAL = Node{">=",Node::OPERATOR,Node::OP_BOTH};
const Node Node::OP_SMALLER_OR_EQUAL = Node{"<=",Node::OPERATOR,Node::OP_BOTH};
const Node Node::OP_MINUS = Node{"-", Node::OPERATOR, Node::OP_BOTH};
const Node Node::OP_EQUAL = Node{"==", Node::OPERATOR, Node::OP_BOTH};
const Node Node::OP_NOT_EQUAL = Node{"!=", Node::OPERATOR, Node::OP_BOTH};
const Node Node::OP_LOGICAL_AND = Node{"&&", Node::OPERATOR, Node::OP_BOTH};
const Node Node::OP_LOGICAL_OR = Node{"||", Node::OPERATOR, Node::OP_BOTH};
const Node Node::OP_ASSIGN = Node{"=", Node::OPERATOR, Node::OP_BOTH};
const Node Node::OP_COMMA = Node{",", Node::OPERATOR, Node::OP_BOTH};

Node::Node(std::string s, Token t, SpecialToken sp, std::initializer_list<Node> il,std::string::size_type i) : str(std::move(s)), token(t), specialToken(sp), operands(il), index(i) {/*empty*/}

void Node::clear() {
    token = NONE;
    str.clear();
    operands.clear();
}

bool Node::isEmpty() const{
    return token == NONE;
}

bool Node::operator==(const Node &node) const {
    return this->str == node.str && this->token == node.token && this->specialToken == node.specialToken;
}

bool Node::operator!=(const Node &node) const {
    return this->str != node.str || this->token != node.token || this->specialToken != node.specialToken;
}

bool Node::operator>(const Node &node) const {///@todo find a better algorithm
    std::string t = this->str + (char)this->token + (char)this->specialToken,
            n = node.str + (char)node.token + (char)node.specialToken;
    return t > n;
}

bool Node::operator<(const Node &node) const {///@todo find a better algorithm
    std::string t = this->str + (char)this->token + (char)this->specialToken,
            n = node.str + (char)node.token + (char)node.specialToken;
    return t < n;
}

bool Node::operator>=(const Node &node) const {
    return !(*this < node);
}

bool Node::operator<=(const Node &node) const {
    return !(*this > node);
}

bool Node::isEqualByTokenAndString(const Node &node) const {
    return token == node.token && str == node.str;
}
