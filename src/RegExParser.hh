# ifndef REGEXPARSER_HH
# define REGEXPARSER_HH

# include "AutomataManager.hh"
# include "Parser.hh"

/**
 * @brief Class for a Regular Expression engine
 * 
 * This class manages all the calling to functions and
 * printing auxiliary methods for the engine.
 */
class RegExParser {

private:

    /** @brief Parser instance */
    Parser p;

    /** @brief Automata Manager instance */
    AutomataManager am;

    /** @brief Generated NFA */
    NFA nfa;

    /**
     * @brief Transforms NodeType into string
     * 
     * @param tp NodeType instance
     * @return std::string equivalent string
     */
    std::string enumToString(NodeType tp);

    /**
     * @brief Transforms StateType into string
     * 
     * @param st StateType instance
     * @return std::string equivalent string
     */
    std::string enumToString(StateType st);

    /**
     * @brief Auxiliary method to print an AST tree
     * 
     * @param prefix string of tree level
     * @param node subtree being printed
     * @param isLeft true if subtree is the left subtree of parent
     */
    void printTree(const std::string& prefix, NodePtr& node, bool isLeft);

    /**
     * @brief Auxiliary method to print a NFA tree
     * 
     * @param prefix string of tree level
     * @param node subtree being printed
     * @param isLeft true if subtree is the left subtree of parent
     * @param visited boolean array of visited states
     */
    void printTree(const std::string& prefix, State* &node, bool isLeft, std::vector<bool>& visited);

public:

    /**
     * @brief Construct a new Reg Ex Parser object
     * 
     */
    RegExParser();

    /**
     * @brief Generates the NFA and the parse tree and checks
     * if string belongs to the language
     * 
     * @param regex regular expression string
     * @param s string to match
     * @return true s belongs to language accepted by NFA
     * @return false otherwise
     */
    bool constructAndMatch(std::string& regex, std::string& s);

    /**
     * @brief Parses the regular expression and generates
     * the AST
     * 
     * @param regex regular expression string
     */
    void parseRegex(std::string& regex);

    /**
     * @brief Computes if s is accepted in the language
     * 
     * @param s string to be matched
     */
    bool isAccepted(std::string& s);

    /**
     * @brief Printer for the AST
     * 
     * @param node root of the tree
     */
    void printAST(NodePtr& node);

    /**
     * @brief Printer for the NFA
     * 
     * @param nfa root of the tree
     */
    void printNFA(NFA& nfa);


};

# endif 