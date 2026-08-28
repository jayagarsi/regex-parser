# include "RegExParser.hh"
# include "Parser.hh"

# include "AutomataManager.hh"

RegExParser::RegExParser() {}

bool RegExParser::isAccepted(const std::string& regex, const std::string& s) {
    p = Parser(input);
    auto tree = p.parse();
    parseTree = std::move(tree);
    am.astToNFA(parseTree);
    return am.isMatch(regex, s);
}