# ifndef PARSER_HH
# define PARSER_HH

# include <string>
# include <memory>

/**
 * @brief NodeType enumeration
 * 
 * This represents the type that a node in the syntax tree can have.
 * Each node can either be an operation or a set of atomic chars.
 * We have the concatenation, alternation (|) star (*), plus (*),
 * question (?) and empty value (\varepsilon)
 *
 */
enum NodeType { CHAR, CONCAT, ALT, STAR, PLUS, QUESTION, EMPTY };

/**
 * @brief Node struct
 * 
 * Represents a node in the Abstract Syntax Tree. It has four attribtues:
 *   - type: type of node in the AST. If it is a leave node, it can be
 *           a char value or an empty value. Otherwise it represents the
 *           operation applied to the two subtrees.
 *   - ch: if type == CHAR it is the value of the char
 *   - left_operand: left sub-tree
 *   - right_operand: right sub-tree
 */
struct Node {
    NodeType type;
    char ch = 0;                        // useful when t+ype == Char
    std::unique_ptr<Node> left_operand;     // left operand or unique operand
    std::unique_ptr<Node> right_operand;    // right operand if there is
};

using NodePtr = std::unique_ptr<Node>;

/* UNAMBIGOUS LL(1) GRAMMAR for Regular Exprs
    expr    := term ('|' term)*
    term    := factor*
    factor  := atom postfix?
    postfix := '*' | '+' | '?'
    atom    := CHAR | '(' expr ')'
*/

/** @class Parser
 * @brief Recursive Descent Parser for a LL(1) grammar for the language
 * of simple Regular Expressions. The grammar for this can be seen in the ANTLR file
 * Here is the unambigous LL(1) version:
 *
 *    expr -> term ('|' term)*
 *    term -> factor*
 *    factor -> atom postfix?
 *    atom -> CHAR | '(' expr ')'
 *
 *    CHAR -> ['a'-'z'] | ['A' - 'Z'] | ['0'-'9']
 *    postfix -> '*' | '+' | '?'
 * 
 * Since the grammar is so simple, the easiest solution is to use the mentioned
 * Recursive Descent Parser, which implements a method for every non-terminal expression.
 */
class Parser {

public:

    /**
     * @brief Construct a new Parser object
     * 
     */
    Parser();

    /**
     * @brief Construct a new Parser object
     * 
     * @param input the regular expression string
     */
    Parser(std::string& input);

    /**
     * @brief Setter for the input attribute
     * 
     * @param input new regular expression string
     */
    void set(const std::string& input);

    /**
     * @brief Parses the input and outputs the AST
     * 
     * @return NodePtr root of the AST
     */
    NodePtr parse();

private:
    /** @brief Regular expression string */
    std::string s = "";

    /** @brief Parsed expression with operator precedence */
    std::string expr;

    /** @brief Position in the string */
    size_t pos = 0;

    /**
     * @brief Peeks at the next character
     * If the input string is fully parsed returns '\0'
     * otherwise the corresponding char
     * 
     * @return char 
     */
    char peek() const;

    /**
     * @brief Peeks and consumes the next char
     * Peeks and moves the pointer to the string
     * 
     * @return char 
     */
    char consume();

    /**
     * @brief Checks if the parsed string is a Term
     * 
     * @param s 
     * @return true 
     * @return false 
     */
    bool isTerm(char s);

    /**
     * @brief Parse *Expr* rule of the grammar
     * 
     * @return NodePtr subtree with root *expr*
     */
    NodePtr parseExpr();

    /**
     * @brief Parse *Term* rule of the grammar
     * 
     * @return NodePtr subtree with root *term*
     */
    NodePtr parseTerm();

    /**
     * @brief Parse *Factor* rule of the grammar
     * 
     * @return NodePtr subtree with root *factor*
     */
    NodePtr parseFactor();

    /**
     * @brief Parse *Atom* rule of the grammar
     * 
     * @return NodePtr subtree with root *atom*
     */
    NodePtr parseAtom();
};

# endif