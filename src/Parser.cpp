# include "Parser.hh"
# include <iostream>

Parser::Parser(const std::string& input) : s(input), pos(0) {}

NodePtr Parser::parse() {
    auto node = parseExpr();
    if (pos != s.size()) throw std::runtime_error("Unexpected symbol");
    return node;
}

char Parser::peek() const {
    return pos < s.size() ? s[pos] : '\0';
}

char Parser::consume() {
    return s[pos++];
}


bool Parser::isTerm(char s) {
    return (s != ')' and s != '|' and s != '\0');
}

NodePtr Parser::parseExpr() {                  // expr :   term ( | term)*
    expr += '(';        
    auto node = parseTerm();                  // process first term
    while (peek() == '|') {
        consume();
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
    NodePtr node = nullptr;                    // term pointer
    while (isTerm(peek())) {               // keep looking until end of term is found (end of line, '|' or ')'
        auto next = parseFactor();
        if (not node)                          // only one factor found
            node = std::move(next);
        else {                                 // concatenation of factors
            auto n = std::make_unique<Node>();      /*               CONCAT             */
            n->type = NodeType::CONCAT;             /*               /   \              */
            n->left_operand = std::move(node);      /*              /     \             */
            n->right_operand = std::move(next);     /*         (factor)  (factor)       */
            node = std::move(n);
        }
    }
    if (not node) {         // if no factor was found declare node empty
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
        consume();
        auto n = std::make_unique<Node>();
        switch(c) {
            case '*':
                n->type = NodeType::STAR;
                break;
            case '+':
                n->type = NodeType::PLUS;
                break;
            case '?':
                n->type = NodeType::QUESTION;
                break;
            default:
                break;
        }
        n->left_operand = std::move(node);
        n->right_operand = nullptr;
        node = std::move(n);
        expr += c;
    }
    return node;
}

NodePtr Parser::parseAtom() {
    if (peek() == '(') {
        consume();
        expr += '(';
        auto node = parseExpr();
        if (peek() != ')')
            throw std::runtime_error("expected ')' at " + std::to_string(pos));
        expr += ')';
        consume();
        return node;
    }
    else {
        auto node = std::make_unique<Node>();
        node->type = NodeType::CHAR;
        node->ch = consume();
        expr += node->ch;
        return node;
    }
}
