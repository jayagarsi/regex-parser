# ifndef AUTOMATAMANAGER_HH
# define AUTOMATAMANAGER_HH

# include "Parser.hh"
# include <unordered_map>
# include <vector>

enum StateType { UNIQUE, SPLIT, MATCH };

struct State {
    char c = 0;                       // char of outgoing transtition
    StateType type;
    State* first_out = nullptr;       // pointer to other state
    State* second_out = nullptr;      // pointer if there is a second transition
    int id;
};

struct Fragment {
    State* start;                     // initial node
    std::vector<State**> dangling;    // list of transitions with no assigned state yet
};

struct NFA {
    State* start;
    State* accept;
    std::vector<std::unique_ptr<State>> states;      // list of states
};

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
class AutomataManager {

private:

    NFA nfa;
    int nextID = 0;
    State* makeState(StateType type, char c, State* out, State* out1);
    Fragment astToNFA(NodePtr& ast);

public:

    AutomataManager();

    NFA regexToNFA(NodePtr& ast);
    bool isMatch(NFA& N, std::string& s);

};

# endif