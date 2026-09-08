# include "RegExParser.hh"
# include <iostream>

RegExParser::RegExParser() {}

std::string RegExParser::enumToString(NodeType tp) {
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

std::string RegExParser::enumToString(StateType st) {
    switch(st) {
        case 0:
            return "UNIQUE";
            break;
        case 1:
            return "SPLIT";
            break;
        case 2:
            return "MATCH";
            break;
        default:
            return "";
            break;
    }
}


void RegExParser::printTree(const std::string& prefix, NodePtr& node, bool isLeft)
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
        printTree( prefix + (isLeft ? "│   " : "    "), node->left_operand, true);
        printTree( prefix + (isLeft ? "│   " : "    "), node->right_operand, false);
    }
}


void RegExParser::printTree(const std::string& prefix, State* &node, bool isLeft, std::vector<bool>& visited)
{
    if (node and not visited[node->id])
    {
        visited[node->id] = true;
        std::cout << node->id << " " << enumToString(node->type) << " ";
        if (node->c == 0) std::cout << "null";
        else std::cout << node->c;
        if (node->first_out and node->second_out) {
            std::cout << " -> " << node->first_out->id << " " << node->second_out->id << std::endl;
            printTree( prefix + (isLeft ? "│   " : "    "), node->first_out, true, visited);
            printTree( prefix + (isLeft ? "│   " : "    "), node->second_out, false, visited);
        }
        else if (node->first_out) {
            std::cout << " -> " << node->first_out->id << std::endl;
            printTree( prefix + (isLeft ? "│   " : "    "), node->first_out, true, visited);
        }
        else if (node->second_out) {
            std::cout << " -> " << node->second_out->id << std::endl;
            printTree( prefix + (isLeft ? "│   " : "    "), node->second_out, true, visited);
        }
    }
}

bool RegExParser::constructAndMatch(std::string& regex, std::string& s) {
    parseRegex(regex);
    return isAccepted(s);
}

void RegExParser::parseRegex(std::string& regex) {
    p.set(regex);
    auto tree = p.parse();
    nfa = am.regexToNFA(tree);
}

bool RegExParser::isAccepted(std::string& s) {
    return am.isMatch(nfa, s);
}

void RegExParser::printAST(NodePtr& node) {
    printTree("", node, false);
    std::cout << std::endl; 
}

void RegExParser::printNFA(NFA& nfa) {
    std::vector<bool> visited(nfa.numStates, false);
    printTree("", nfa.start, false, visited);
    std::cout << std::endl;
}
