# include "AutomataManager.hh"
# include <iostream>

AutomataManager::AutomataManager() {}

State* AutomataManager::makeState(StateType type, char c, State* out, State* out1) {
    auto s = std::make_unique<State>(State{c, type, out, out1, nextID++});
    State* raw = s.get();
    nfa.states.push_back(std::move(s));
    return raw;
}

Fragment AutomataManager::astToNFA(NodePtr& ast) {
    switch (ast->type) {
        case NodeType::ALT: {
            // Recursively compute fragments of alternation operands
            auto fragment_left = astToNFA(ast->left_operand);
            auto fragment_right = astToNFA(ast->right_operand);
            // Create new State in DFA with split type
            auto s = makeState(StateType::SPLIT, 0, fragment_left.start, fragment_right.start);
            // Generate a new array with all dangling nodes of both fragments
            auto dangling = fragment_left.dangling;
            dangling.insert(dangling.end(), fragment_right.dangling.begin(), fragment_right.dangling.end());
            auto f = Fragment{s, dangling};         // create new fragment
            return f;
        }
        case NodeType::CONCAT: {
            // Recursively compute fragments of concatenation operands
            auto fragment_left = astToNFA(ast->left_operand);
            auto fragment_right = astToNFA(ast->right_operand);
            // All left fragment danglings now go to right fragment initial state
            for (auto ptr : fragment_left.dangling) *ptr = fragment_right.start;
            // Fragment becomes initial of left fragment
            auto f = Fragment{fragment_left.start, fragment_right.dangling};
            return f;
        }
        case NodeType::STAR: {
            auto fragment_left = astToNFA(ast->left_operand);
            auto s = makeState(StateType::SPLIT, 0, fragment_left.start, nullptr);
            for (auto ptr : fragment_left.dangling) *ptr = s;
            std::vector<State**> dangling = {&s->second_out};
            auto f = Fragment{s, dangling};
            return f;
        }
        case NodeType::PLUS: {
            auto fragment_left = astToNFA(ast->left_operand);
            auto s = makeState(StateType::SPLIT, 0, fragment_left.start, nullptr);
            for (auto ptr : fragment_left.dangling) *ptr = s;
            std::vector<State**> dangling = {&s->second_out};
            auto f = Fragment{fragment_left.start, dangling};
            return f;
        }
        case NodeType::QUESTION: {
            auto fragment_left = astToNFA(ast->left_operand);
            auto s = makeState(StateType::SPLIT, 0, fragment_left.start, nullptr);
            auto dangling = fragment_left.dangling;
            dangling.push_back(&s->second_out);
            auto f = Fragment{s, dangling};
            return f;
        }
        default: {  // NodeType::CHAR or NodeType::EMPTY
            auto s = makeState(StateType::UNIQUE, ast->ch, nullptr, nullptr);
            std::vector<State**> dangling = {&s->first_out};
            auto f = Fragment{s, dangling};
            return f;
        }
    }
}

NFA AutomataManager::regexToNFA(NodePtr& ast) {
    Fragment f = astToNFA(ast);
    State* finalState = makeState(StateType::MATCH, 0, nullptr, nullptr);
    for (auto ptr : f.dangling) *ptr = finalState;
    nfa.start = f.start;
    nfa.accept = finalState;
    return std::move(nfa);
}

bool AutomataManager::isMatch(NFA& N, std::string& s) {
    std::vector<bool> alreadyOn(N.states.size(), false);
}
