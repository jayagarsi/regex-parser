# ifndef NFA_HH
# define NFA_HH

# include "Parser.hh"
# include <unordered_map>
# include <vector>

struct State {
    std::unordered_map<char, State> transitions;
    std::vector<State> epsilons;
}

/**
 * @brief Thompson's NFA
 * 
 * Construction of an NFA as designed in the McNaughton-Yamada-Thompson
 * algorithm. An NFA N generated from a regular expression E using 
 * this process has the following properties:
 *   - N has only one initial state
 *   - N has only one accepting state
 *   - The number of states of A is linear in the size of E
 *   - The number of transitions leaving any state is at most two
 * 
 */
class NFA {

private:

    int start, end;                 // index of the start and end states;
    std::vector<State> states;      // list of states

public:

    NFA(NodePtr ast);

    NFA regexToNFA(NodePtr regex);
    NFA mergeNFA(NFA n1, NFA n2);

};

# endif