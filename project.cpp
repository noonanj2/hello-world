#include <iostream>
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::to_string;
using std::getline;

int get_line_sum(const string &line) {
    int line_sum = 0;
    int length = line.length();
    for (int i = 0; i < length; i++) {
        // Turn the digit chars into the correct corresponding ints
        int digit = static_cast<int>(line[i]) - '0'; 
        line_sum += digit;
    }
    return line_sum;
}

// Tells us the neighborhood of the given index in the string, i.e., the three
// chars surrounding it
string get_neighborhood(const string &line, const int &i) {
    int length = line.length();
    // Add the endpoints of the line to the opposite sides; this is necessary for
    // the function to work on the first and last indices
    string extended_line = line[length - 1] + line + line[0];
    string neighborhood =
        extended_line.substr(i, 3); // Position i in the extended line is the same
                                    // as position i-1 in the original line
    return neighborhood;
}

// Tells us what to change a character in the line into, based on its neighbors
// and the rules
char get_next_state(const string &neighborhood, const string &rules) {
    int rule_length = rules.length();
    // Each line in the rules string is 9 characters long (including \n)
    for (int line_start = 0; line_start < rule_length; line_start += 9) {
        if (neighborhood == rules.substr(line_start, 3)) {
            // The new state is the 8th character in the line, i.e. 7th after the
            // start
            return rules[line_start + 7];
        }
    }
    return '0'; // If the neighborhood isn't in the rules, default to '0'
}

// I use a non-const ref for line because we actually want to change the value
// of the input parameter
void update_line(string &line, const string &rules) {
    int length = line.length();
    // The last neighborhood will be affected by altering the beginning of the
    // string, so we save it here first
    string last_neighborhood = get_neighborhood(line, length - 1);
    string next_neighborhood = get_neighborhood(line, 0);
    string neighborhood;
    for (int i = 0; i < length - 1; i++) {
        // The neighborhood used to change character i must be saved before character
        // i-1 gets changed, so I set up the code to set "neighborhood" equal to the
        // previous iteration's next_neighborhood and then find the new
        // next_neighborhood before actually changing line[i].
        neighborhood = next_neighborhood;
        next_neighborhood = get_neighborhood(line, i + 1);
        line[i] = get_next_state(neighborhood, rules);
    }
    line[length - 1] = get_next_state(last_neighborhood, rules);
}

// Since line will be altered by the function, I am not making it a const.
// I am also not making it a ref because we were not told to alter the input
// parameters for this function.
string run_cellular_automata(const string &rules, const int &no_of_lines, string line) {
    string output = "";
    for (int i = 0; i < no_of_lines; i++) {
        string line_sum = to_string(get_line_sum(line));
        output += line;
        output += " sum = " + line_sum + '\n';
        update_line(line, rules);
    }
    return output;
}

int main() {
    string rule_line, line;
    string rules = "";
    int no_of_lines;
    while (getline(cin, rule_line)) {
        if (rule_line == ".")
            break;
        rules += rule_line + '\n';
    }
    cin >> no_of_lines;
    cin >> line;
    string output = run_cellular_automata(rules, no_of_lines, line);
    cout << output;
    return 0; // Since it says on Piazza that Mimir will mess up if main 
              // doesn't have a return statement, I am just returning 0
}