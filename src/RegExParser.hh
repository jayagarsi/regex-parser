# ifndef REGEXPARSER_HH
# define REGEXPARSER_HH

# include "AutomataManager.hh"
# include "Parser.hh"

class RegExParser {

private:

    Parser p;
    AutomataManager am;
    NFA nfa;

    std::string enumToString(NodeType tp);
    std::string enumToString(StateType st);

    void printTree(const std::string& prefix, NodePtr& node, bool isLeft);
    void printTree(const std::string& prefix, State* &node, bool isLeft, std::vector<bool>& visited);

public:

    RegExParser();
    bool constructAndMatch(std::string& regex, std::string& s);
    void parseRegex(std::string& regex);
    bool isAccepted(std::string& s);

    
    void printBT(NodePtr& node);
    void printNFA(NFA& nfa);


};

# endif 