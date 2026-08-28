# include "Parser.hh"
# include "AutomataManager.hh"
# include <iostream>
# include <string>

std::string enumToString(NodeType tp) {
    switch(tp) {
        case 0:
            return "CHAR";
            break;
        case 1:
            return "CONCAT";
            break;
        case 2:
            return "ALT";
            break;
        case 3:
            return "STAR";
            break;
        case 4:
            return "PLUS";
            break;
        case 5:
            return "QUESTION";
            break;
        case 6:
            return "EMPTY";
            break;
        default:
            return "";
            break;
    }
}

void printBT(const std::string& prefix, NodePtr& node, bool isLeft)
{
    if (node)
    {
        std::cout << prefix;
        std::cout << (isLeft ? "├──" : "└──" );

        // print the value of the node
        std::cout << enumToString(node->type);
        if (node->type == NodeType::CHAR) std::cout << " " << node->ch;
        std::cout << std::endl;

        // enter the next tree level - left and right branch
        printBT( prefix + (isLeft ? "│   " : "    "), node->left_operand, true);
        printBT( prefix + (isLeft ? "│   " : "    "), node->right_operand, false);
    }
}

void printBT(NodePtr& node) {
    printBT("", node, false);    
}

int main () {
    std::string s;
    std::cin >> s;
    Parser p(s);
    NodePtr tree = p.parse();
    printBT(tree);
    AutomataManager am;
    auto nfa = am.regexToNFA(tree);
    
}