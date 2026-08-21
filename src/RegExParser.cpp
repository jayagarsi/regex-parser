# include "RegExParser.hh"
# include "Parser.hh"

RegExParser::RegExParser(const std::string& input) {
    Parser p(input);
    auto tree = p.parse();
    parseTree = std::move(tree);
}

bool RegExParser::isAccepted() {
    
}