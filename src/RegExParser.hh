# ifndef REGEXPARSER_HH
# define REGEXPARSER_HH

class RegExParser {

private:

    Parser p;
    AutomataManager am();

public:

    RegExParser();
    bool isAccepted(const std::string regex, const std::string s);


};

# endif 