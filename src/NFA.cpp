# include "NFA.hh"

NFA::NFA(NodePtr ast) {

}

void NFA::regexToNFA(NodePtr ast) {
    switch (ast->type) {
        case NodeType::CHAR:
            std::cout << "hola" << std::endl;
            break;
        case NodeType::CONCAT:
            std::cout << "hola" << std::endl;
            break;
        case NodeType::ALT:
            std::cout << "hola" << std::endl;
            break;
        case NodeType::STAR:
            std::cout << "hola" << std::endl;
            break;
        case NodeType::PLUS:
            std::cout << "hola" << std::endl;
            break;
        case NodeType::QUESTION:
            std::cout << "hola" << std::endl;
            break;
        case NodeType::EMPTY:
            std::cout << "hola" << std::endl;
            break;
        default:
            return;
            break;
    }
}