# ifndef REGEXPARSER_HH
# define REGEXPARSER_HH

class RegExParser {

public:

    RegExParser(const std::string& input);

    bool isAccepted(const std::string s);

private:

    NodePtr parseTree;


};

# endif 