#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <sstream>
#include <tuple>
#include <vector>
#include <math.h>
// defined std::unique_ptr
#include <memory>
// defines Var and Lit
#include "minisat/core/SolverTypes.h"
// defines Solver
#include "minisat/core/Solver.h"

using namespace std;

#include "Proposition.h"

template<typename Out>
void split(const std::string &s, char delim, Out result) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}

std::vector <std::string> split(const std::string &s, char delim) {
    std::vector <std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

void permutation(std::vector<int> bar);

bool build_points_from_E(int);

void rule1(int vertice_number, int vc_size);

void rule2(int vertice_number, int vc_size);

std::vector<int> rule4(int point);

void clause_creation(std::vector<int> bar);

void clause_creation_reverse(std::vector<int> bar);

bool reduction_solver(int k_value);

std::vector<int> points1;
std::vector <std::string> nums;
std::vector <Proposition> props;
std::unique_ptr <Minisat::Solver> solver(new Minisat::Solver());
Minisat::vec <Minisat::Lit> vars;
int vertice_number;

// std::ifstream infile("test.txt");
int main(int argc, char **argv) {
    // Test code. Replaced with your code

    // Print command line arguments that were used to start the program

    // separator character
    // read from stdin until EOF


    unsigned num;

    int length = 0;
    bool flag_v = true;

    bool flag_e = true;
    while (!std::cin.eof()) {
        // std::cout << "Enter numbers separated by comma: ";
        std::string line;
        std::getline(std::cin, line);
        // cout << line << endl;
        std::istringstream input(line);
        if (line.size() <= 0) {
            if (std::cin.eof()) {
                std::cerr << "EOF\n";
            } else {
                std::cerr << "Error:in vertice cover no input\n";
            }
        } else {

            char ch;
            input >> ch;
            ws(input);

            if (input.fail()) {
                std::cerr << "Error:in vertice cover   need to parse a character\n";
            }

            if (ch == 'V' && flag_v) {
                //std::cout<<line<<std::endl;
                nums.clear();
                points1.clear();
                // parse an integer
                input >> num;

                if (input.fail()) {
                    std::cerr << "Error:in vertice cover   need to parse a number\n";
                    break;
                } else {
                    vertice_number = num;

                    flag_v = true;
                }
                // if eof bail out

            } // end V
            else if (ch == 'E' && flag_e) {
                //std::cout<<line<<std::endl;
                nums = split(line, ',');
                // done parsing a line, print the numbers
                length = nums.size();
                /*
                    if (length == 1) {
                      std::cout
                          << "Warning:Since there are no edges, no path can be in this "
                             "graph\n";
                    }
                */
                if (!nums.empty() && length != 1) {

                    bool flag_point = build_points_from_E(vertice_number);

                    if (!flag_point) {
                        points1.clear();
                        std::cerr << "Error: in vertex cover  wrong input for E\n";
                    } else {
                        /**
                          rule1(vertice_number,floor(vertice_number/2.0));
                          rule2(vertice_number,floor(vertice_number/2.0));
                        int count=1;
                        for (std::vector<int>::iterator it = points1.begin() ;
                            it != points1.end(); ++it) {
                            std::vector<int> bar;
                            std::vector<int> bar2;
                            bar=rule4((*it+1));
                            for (std::vector<int>::iterator its = bar.begin() ;
                            its != bar.end(); ++its) {
                                    //std::cout<<*its<<"it in it"<<std::endl;
                            }
                            std::advance(it,1);
                            bar2=rule4((*it+1));
                            std::vector<int> two_bar=bar;
                            two_bar.insert(two_bar.end(),bar2.begin(),bar2.end());
                            for (std::vector<int>::iterator itt = two_bar.begin() ;
                            itt != two_bar.end(); ++itt) {
                                    std::cout<<*itt<<"it in is"<<std::endl;
                            }
                            clause_creation(two_bar);
                            //if(count%2==0)
                            //	std::cout<<std::endl;
                            count++;
                        }
                         bool res = solver->solve();
                        **/
                        int tmp_high = vertice_number;
                        int low = 1;
                        int mid = 0;
                        while (low <= tmp_high) {
                            mid = (low + tmp_high) / 2;
                            if (reduction_solver(mid)) {
                                tmp_high = mid - 1;
                            } else {
                                low = mid + 1;
                            }

                            //std::cout<<mid<<"this is mid"<<std::endl;
                        }
                        bool res = reduction_solver(low);
                        //std::cout << "The result is: " << res << "\n";
                        //std::cout << "satisfying assignment is: ";
                        if (res) {
                            for (int s = 0; s < vars.size(); s++) {

                                //std::cout <<"l"<<s<<"=" << Minisat::toInt(solver->modelValue(vars[s])) << " ";
                                if (Minisat::toInt(solver->modelValue(vars[s])) == 0)
                                    std::cout << (props.at(s).index_i - 1) << " ";
                            }
                        }
                        std::cout << std::endl;
                        flag_e = true;
                    }
                }

            } else
                std::cerr << "Error:in vertice cover  wrong input\n" << std::flush;
        }
    }
}

bool reduction_solver(int k_value) {
    solver.reset(new Minisat::Solver());
    props.clear();
    vars.clear();
    rule1(vertice_number, k_value);
    rule2(vertice_number, k_value);
    int count = 1;
    for (std::vector<int>::iterator it = points1.begin();
         it != points1.end(); ++it) {
        std::vector<int> bar;
        std::vector<int> bar2;
        bar = rule4((*it + 1));

        std::advance(it, 1);
        bar2 = rule4((*it + 1));
        std::vector<int> two_bar = bar;
        two_bar.insert(two_bar.end(), bar2.begin(), bar2.end());

        clause_creation(two_bar);
        //if(count%2==0)
        //	std::cout<<std::endl;
        count++;
    }
    bool res = solver->solve();
    return res;
}

void rule1(int vertice_number, int vc_size) {
    int index = 1;
    //std::cout<<"rule 1 VN:"<<vertice_number<<std::endl;
    //std::cout<<"rule 1 VCS:"<<vc_size<<std::endl;
    //std::list<std::vector<int>> ckln;




    for (int i = 1; i <= vertice_number; i++) {

        for (int j = 1; j <= vc_size; j++) {
            Proposition obj1;
            obj1.position = index;
            obj1.index_i = i;
            obj1.index_k = j;
            props.push_back(obj1);
            index++;
            Minisat::Lit l = Minisat::mkLit(solver->newVar());
            vars.push(l);
        }

    }
    //above build the line object for whole task
    /*
      for (std::vector<Proposition>::iterator it = line.begin() ;
            it != line.end(); ++it) {

            std::cout<<(*it).index_i<<" index_i ";
              std::cout<<(*it).position<<" position ";
              std::cout<<std::endl;
        }
    */
    std::vector<int> bar_rule3;
    for (int i = 1; i <= vc_size; i++) {

        for (std::vector<Proposition>::iterator it = props.begin();
             it != props.end(); ++it) {
            if (i == (*it).index_k) {

                //std::cout<<(*it).position;

                bar_rule3.push_back((*it).position);

            }
        }
        clause_creation(bar_rule3);
        permutation(bar_rule3);
        bar_rule3.clear();

    }
    /*
    for(int i=0;i<vc_size;i++)
    {

        for(int j=i;j<vertice_number*vc_size;j=j+2)
        {
            std::cout<<line.at(j)<<" ";

        }
        std::cout<<"0"<<std::endl;
    }
    */
}


void rule2(int vertice_number, int vc_size) {

    //int index=1;
    //std::cout<<"rule 2 VN:"<<vertice_number<<std::endl;
    //std::cout<<"rule 2 VCS:"<<vc_size<<std::endl;
    std::vector<int> bar_rule2;
    for (std::vector<Proposition>::iterator it = props.begin();
         it != props.end(); ++it) {
        if (((*it).position) % vc_size == 0) {
            bar_rule2.push_back((*it).position);
            //std::cout<<bar_rule2.size()<<" size "<<std::endl;

            permutation(bar_rule2);
            bar_rule2.clear();
        } else {
            //std::cout<<((*it).position)%vc_size<<" % "<<std::endl;
            bar_rule2.push_back((*it).position);
        }
    }
}


std::vector<int> rule4(int point) {

    //int index=1;
    //std::cout<<"rule 4 point:"<<point<<std::endl;

    std::vector<int> bar;
    //std::cout<<props.size()<<"I am in methdo"<<std::endl;
    for (std::vector<Proposition>::iterator iter = props.begin();
         iter != props.end(); ++iter) {
        if (point == (*iter).index_i) {
            bar.push_back((*iter).position);
            //std::cout<<(*iter).position<<"I am in methdo"<<std::endl;

        }
    }

    return bar;

}


//n is the size of vector
//r is the number being chosen to permute
void permutation(std::vector<int> bar) {
    if (bar.size() > 1) {
        int r = 2;
        int n = bar.size();
        //std::cout<<n<<" n value"<<std::endl;
        std::vector<bool> v(n);
        std::fill(v.begin(), v.begin() + r, true);

        do {
            std::vector<int> permu_vector;
            for (int i = 0; i < n; ++i) {
                if (v[i]) {
                    permu_vector.push_back(bar.at(i));
                    //std::cout << bar.at(i) << " ";
                }
            }
            clause_creation_reverse(permu_vector);
            // std::cout << "\n";
        } while (std::prev_permutation(v.begin(), v.end()));
    }
}


void clause_creation(std::vector<int> bar) {

    //std::cout<<std::endl;
    Minisat::vec <Minisat::Lit> clause;
    for (auto i = 0; i < bar.size(); ++i) {

        clause.push(vars[bar.at(i) - 1]);

    }

    solver->addClause(clause);
}

void clause_creation_reverse(std::vector<int> bar) {

    //std::cout<<std::endl;
    Minisat::vec <Minisat::Lit> clause;
    for (auto i = 0; i < bar.size(); ++i) {

        clause.push(~vars[bar.at(i) - 1]);

    }

    solver->addClause(clause);
}

bool build_points_from_E(int points) {
    for (std::string x : nums) {
        std::size_t found = x.find_first_of("E{}<>");
        while (found != std::string::npos) {
            x[found] = ' ';
            found = x.find_first_of("E{}<>", found + 1);
        }
        int integer = std::stoi(x);
        points1.push_back(integer);
        if (integer >= points)
            return false;
    }
    return true;
}
