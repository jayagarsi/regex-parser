# include "Parser.hh"
# include <iostream>

Parser::Parser(const std::string& input) : s(input), pos(0) {}

NodePtr Parser::parse() {
    auto node = parseExpr();
    if (s.size() < pos) throw std::runtime_error("Unexpected symbol");
    return node;
}

char Parser::peek() const {
    return pos < s.size() ? s[pos] : '\0';
}

char Parser::consume() {
    return s[pos++];
}

bool Parser::accept(char s) {
    if (peek() == s) {
        consume();
        return true;
    }
    return false;
}

bool Parser::expect(char s) {
    if (s == ')') {
        consume();
        return true;
    }
    else {
        throw std::runtime_error("expected ')' at " + std::to_string(pos));
        return false;
    }
}

bool Parser::isValid(char s) {
    return (s != ')' and s != '|' and s != '\0'
        and s != '*' and s != '+' and s != '?');
}

NodePtr Parser::parseExpr() {                  // expr :   term ( | term)*
    expr += '(';        
    auto node = parseTerm();                  // process first term
    while (accept('|')) {
        expr += '|';
        auto next = std::make_unique<Node>();   /*            (ALT)            */
        next->type = NodeType::ALT;             /*             / \             */
        next->left_operand = std::move(node);   /*            /   \            */
        next->right_operand = parseTerm();      /*        (term)  (term)       */
        node = std::move(next);
    }
    expr += ')';
    return node;
}

NodePtr Parser::parseTerm() {                  // term :   factor*
    NodePtr node = nullptr;                    // nullptr in case there is no factor
    while (isValid(peek())) {
        auto next = parseFactor();
        if (node) {                                 // more than one factor
            auto n = std::make_unique<Node>();      /*               CONCAT             */
            n->type = NodeType::CONCAT;             /*               /   \              */
            n->left_operand = std::move(node);      /*              /     \             */
            n->right_operand = std::move(next);     /*         (factor)  (factor)       */
        }              
        else {                     // if no factor found or only one            
            node = std::move(next);                 /*  (FACTOR)  */
        }                                           
    }
    if (not node) {
        auto n = std::make_unique<Node>();
        n->type = NodeType::EMPTY;
        node = std::move(n);
    }
    return node;
}

NodePtr Parser::parseFactor() {
    expr += '(';
    auto node = parseAtom();
    expr += ')';
    char c = peek();
    if (c == '*' or c == '+' or c == '?') {
        auto n = std::make_unique<Node>();
        n->type = NodeType::STAR;
        n->left_operand = std::move(node);
        n->right_operand = nullptr;
        node = std::move(n);
        expr += c;
        consume();
    }
    return node;
}

NodePtr Parser::parseAtom() {
    char p = peek();
    if (p == '(') {
        consume();
        expr += '(';
        auto node = parseExpr();
        expr += ')';
        expect(')');
        return node;
    }
    /*
    else if (p == '+' or p == '*' or p == '?') {
        throw std::runtime_error("wrong format " + std::to_string(pos));
    }
    */
    else {
        auto node = std::make_unique<Node>();
        node->type = NodeType::CHAR;
        node->ch = consume();
        return node;
    }
}
