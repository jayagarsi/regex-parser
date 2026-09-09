# include <emscripten/emscripten.h>
# include "Parser.hh"
# include "AutomataManager.hh"
# include <string>
# include <cstring>
# include <sstream>

int g_nodeCounter = 0;

std::string nodeLabel(NodeType t, char ch) {
    switch (t) {
        case NodeType::ALT: return "|";
        case NodeType::CONCAT: return "·";
        case NodeType::STAR: return "*";
        case NodeType::PLUS: return "+";
        case NodeType::QUESTION: return "?";
        case NodeType::EMPTY: return "ε";
        case NodeType::CHAR: return std::string(1, ch);
    }
    return "?";
}

void astToMermaid(Node* node, std::ostringstream& out) {
    int myId = g_nodeCounter++;
    out << "N" << myId << "((\"" << nodeLabel(node->type, node->ch) << "\"))\n";
    if (node->left_operand) {
        int leftId = g_nodeCounter;
        astToMermaid(node->left_operand.get(), out);
        out << "N" << myId << " --- N" << leftId << "\n";
    }
    if (node->right_operand) {
        int rightId = g_nodeCounter;
        astToMermaid(node->right_operand.get(), out);
        out << "N" << myId << " --- N" << rightId << "\n";
    }
}

extern "C" {
EMSCRIPTEN_KEEPALIVE
const char* getASTDiagram(const char* pattern) {
    static std::string result;
    try {
        std::string p(pattern);
        Parser parser(p);
        NodePtr ast = parser.parse();
        g_nodeCounter = 0;
        std::ostringstream out;
        out << "graph LR\n";
        astToMermaid(ast.get(), out);
        result = out.str();
    } catch (...) {
        result = "graph LR\nErr((\"invalid\"))";
    }
    return result.c_str();
}
}

std::string stateLabel(int id) { return "S" + std::to_string(id); }

extern "C" {
EMSCRIPTEN_KEEPALIVE
const char* getNFADiagram(const char* pattern) {
    static std::string result;
    try {
        std::string p(pattern);
        Parser parser(p);
        NodePtr ast = parser.parse();
        AutomataManager mgr;
        NFA nfa = mgr.regexToNFA(ast);

        std::ostringstream out;
        out << "graph LR\n";
        for (auto& sPtr : nfa.states) {
            State* s = sPtr.get();
            if (s->type == StateType::UNIQUE && s->first_out)
                out << stateLabel(s->id) << " -->|" << s->c << "| " << stateLabel(s->first_out->id) << "\n";
            if (s->type == StateType::SPLIT) {
                if (s->first_out) out << stateLabel(s->id) << " -->|ε| " << stateLabel(s->first_out->id) << "\n";
                if (s->second_out) out << stateLabel(s->id) << " -->|ε| " << stateLabel(s->second_out->id) << "\n";
            }
        }
        result = out.str();
    } catch (...) {
        result = "graph LR\nErr((\"invalid\"))";
    }
    return result.c_str();
}
}

// Simple static buffer used to hand error messages back to JS.
static std::string g_lastError;

extern "C" {

// Returns: 1 = match, 0 = no match, -1 = parse/runtime error.
// On -1, call getLastError() from JS to retrieve the error message.
EMSCRIPTEN_KEEPALIVE
int matchRegex(const char* pattern, const char* input) {
    try {
        std::string patternStr(pattern);
        Parser parser(patternStr);
        NodePtr ast = parser.parse();

        AutomataManager mgr;
        NFA nfa = mgr.regexToNFA(ast);

        std::string inputStr(input);
        bool result = mgr.isMatch(nfa, inputStr);

        g_lastError.clear();
        return result ? 1 : 0;
    } catch (const std::exception& e) {
        g_lastError = e.what();
        return -1;
}

}
}
