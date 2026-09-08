#include "Parser.hh"
#include "AutomataManager.hh"
#include <iostream>

int main() {
    struct TestCase { std::string pattern; std::string input; bool expected; };
    std::vector<TestCase> tests = {
        {"(a|b)*", "aba", true},
        {"(a|b)*", "abc", false},
        {"a*", "", true},
        {"a+", "", false},
        {"a+", "aaa", true},
        {"ab", "ab", true},
        {"ab", "a", false},
        {"a?b", "b", true},
        {"a?b", "ab", true},
        {"(ab)*c", "ababc", true},
        {"(ab)*c", "abab", false},
    };

    int passed = 0;
    for (auto& t : tests) {
        std::string pattern = t.pattern;
        Parser parser(pattern);
        NodePtr ast = parser.parse();
        AutomataManager mgr;
        NFA nfa = mgr.regexToNFA(ast);
        std::string input = t.input;
        bool result = mgr.isMatch(nfa, input);
        bool ok = (result == t.expected);
        passed += ok;
        std::cout << (ok ? "PASS" : "FAIL") << " pattern=\"" << t.pattern
                  << "\" input=\"" << t.input << "\" expected=" << t.expected
                  << " got=" << result << std::endl;
    }
    std::cout << passed << "/" << tests.size() << " passed" << std::endl;
    return 0;
}
