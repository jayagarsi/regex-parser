# include "AutomataManager.hh"
# include <iostream>
# include <stack>

AutomataManager::AutomataManager() {}

State* AutomataManager::makeState(NFA& nfa, StateType type, char c, State* out, State* out1) {
    auto s = std::make_unique<State>(State{c, type, out, out1, nextID++});
    State* raw = s.get();
    nfa.states.push_back(std::move(s));
    return raw;
}

Fragment AutomataManager::astToNFA(NFA& N, NodePtr& ast) {
    switch (ast->type) {
        case NodeType::ALT: {
            // Recursively compute fragments of alternation operands
            auto fragment_left = astToNFA(N, ast->left_operand);
            auto fragment_right = astToNFA(N, ast->right_operand);
            // Create new State in DFA with split type
            auto s = makeState(N, StateType::SPLIT, 0, fragment_left.start, fragment_right.start);
            // Generate a new array with all dangling nodes of both fragments
            auto dangling = fragment_left.dangling;
            dangling.insert(dangling.end(), fragment_right.dangling.begin(), fragment_right.dangling.end());
            auto f = Fragment{s, dangling};         // create new fragment
            return f;
        }
        case NodeType::CONCAT: {
            // Recursively compute fragments of concatenation operands
            auto fragment_left = astToNFA(N, ast->left_operand);
            auto fragment_right = astToNFA(N, ast->right_operand);
            // All left fragment danglings now go to right fragment initial state
            for (auto ptr : fragment_left.dangling) *ptr = fragment_right.start;
            // Fragment becomes initial of left fragment
            auto f = Fragment{fragment_left.start, fragment_right.dangling};
            return f;
        }
        case NodeType::STAR: {
            auto fragment_left = astToNFA(N, ast->left_operand);
            auto s = makeState(N, StateType::SPLIT, 0, fragment_left.start, nullptr);
            for (auto ptr : fragment_left.dangling) *ptr = s;
            std::vector<State**> dangling = {&s->second_out};
            auto f = Fragment{s, dangling};
            return f;
        }
        case NodeType::PLUS: {
            auto fragment_left = astToNFA(N, ast->left_operand);
            auto s = makeState(N, StateType::SPLIT, 0, fragment_left.start, nullptr);
            for (auto ptr : fragment_left.dangling) *ptr = s;
            std::vector<State**> dangling = {&s->second_out};
            auto f = Fragment{fragment_left.start, dangling};
            return f;
        }
        case NodeType::QUESTION: {
            auto fragment_left = astToNFA(N, ast->left_operand);
            auto s = makeState(N, StateType::SPLIT, 0, fragment_left.start, nullptr);
            auto dangling = fragment_left.dangling;
            dangling.push_back(&s->second_out);
            auto f = Fragment{s, dangling};
            return f;
        }
        default: {  // NodeType::CHAR or NodeType::EMPTY
            auto s = makeState(N, StateType::UNIQUE, ast->ch, nullptr, nullptr);
            std::vector<State**> dangling = {&s->first_out};
            auto f = Fragment{s, dangling};
            return f;
        }
    }
}

SetState AutomataManager::epsilonClosure(SetState& T) {
    std::stack<State*> stack;
    SetState closure = T;                       // initialize stack with input
    for (auto t : T) closure.insert(t);         // initialize ε-closure with incoming states
    while (not stack.empty()) {
        auto t = stack.top();
        stack.pop();
        if (t->type == StateType::SPLIT) {
            if (t->first_out and closure.insert(t->first_out).second)
                stack.push(t->first_out);
            if (t->second_out and closure.insert(t->second_out).second)
                stack.push(t->second_out);
        }
        
        else if (t->type == StateType::UNIQUE) {
            if (t->first_out and t->first_out->c == '\0' and closure.insert(t->first_out).second) {
                stack.push(t->first_out);
            }
        }
    }
    return closure;
}

bool AutomataManager::simulateNFA(NFA& N, std::string& s) {
    SetState start;
    start.insert(N.start);
    auto current = epsilonClosure(start);
    for (char c : s) {
        SetState moveSet;
        for (auto state : current) {
            if (state->type == StateType::UNIQUE and state->c == c) {
                moveSet.insert(state->first_out);
            }
        }
        if (moveSet.empty()) return false;
        current = epsilonClosure(moveSet);
    }
    return current.count(N.accept) > 0;
}

bool AutomataManager::simulateDFA(DFA& D, std::string& s) {
    return false;
}

NFA AutomataManager::regexToNFA(NodePtr& ast) {
    NFA nfa;
    Fragment f = astToNFA(nfa, ast);
    State* finalState = makeState(nfa, StateType::MATCH, 0, nullptr, nullptr);
    for (auto ptr : f.dangling) *ptr = finalState;
    nfa.start = f.start;
    nfa.accept = finalState;
    return nfa;
}

DFA AutomataManager::nfaToDFA(NFA& nfa) {
    DFA dfa;
    // auto DStates = epsilonClosure({ nfa.start });
    return dfa;
}

bool AutomataManager::isMatch(NFA& N, std::string& s) {
    if (N.states.size() > 0) {
        return simulateNFA(N, s);
    }
    else {
        throw std::runtime_error("No NFA can be simulated");
    }
}

bool AutomataManager::isMatch(DFA& D, std::string& s) {
    if (D.states.size() > 0) {
        return simulateDFA(D, s);
    }
    else {
        throw std::runtime_error("No NFA can be simulated");
    }
}

