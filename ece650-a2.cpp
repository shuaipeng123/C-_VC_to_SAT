#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <sstream>
#include <tuple>
#include <vector>

using namespace std;

template <typename Out>
void split(const std::string &s, char delim, Out result) {
  std::stringstream ss;
  ss.str(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    *(result++) = item;
  }
}

std::vector<std::string> split(const std::string &s, char delim) {
  std::vector<std::string> elems;
  split(s, delim, std::back_inserter(elems));
  return elems;
}

void shortest_path(int, int, int, int);

bool build_points_from_E(int);

std::vector<int> points1;
std::vector<std::string> nums;

// std::ifstream infile("test.txt");
int main(int argc, char **argv) {
  // Test code. Replaced with your code

  // Print command line arguments that were used to start the program

  // separator character
  // read from stdin until EOF
  int dp;
  int sp;
  int points;
  unsigned num;

  int length = 0;
  bool flag_v = true;
  bool flag_s = true;
  bool flag_e = true;
  while (!std::cin.eof()) {
    // std::cout << "Enter numbers separated by comma: ";
    std::string line;
    std::getline(std::cin, line);
    // cout << line << endl;
    std::istringstream input(line);
    if (line.size() <= 0) {
      if (std::cin.eof()) {
        std::cerr  << "EOF\n";
      } else {
        std::cerr << "Error:in shortest path no input\n";
      }
    } else {
	
      char ch;
      input >> ch;
      ws(input);

      if (input.fail()) {
        std::cerr << "Error:in shortest path  need to parse a character\n";
      }

      if (ch == 'V' && flag_v) {
	std::cout<<line<<std::endl;
	 nums.clear();
  	points1.clear();
        // parse an integer
        input >> num;

        if (input.fail()) {
          std::cerr << "Error:in shortest path  need to parse a number\n";
          break;
        } else {
          points = num;
          flag_v = true;
        }
        // if eof bail out

      } // end V
      else if (ch == 's' && flag_s) {
        // read a character
        // Note that whitespace is ignored
        std::vector<int> sp_dp;
        while (!input.eof()) {
          if (input.eof()) {
            std::cerr << "Error:in shortest path Unexpected argument\n";
            break;
          }

          // parse an integer
          input >> num;
          if (input.fail()) {
            std::cerr << "Error:in shortest path no starting point\n";
            break;
          } else {
            sp_dp.push_back(num);
            flag_s = false;
          }
        }

        sp = sp_dp.at(0);
        dp = sp_dp.at(1);
        if (sp >= points || dp >= points) {
          flag_s = true;
	   flag_e = true;
          flag_v = true;
          std::cerr << "Error: in shortest path wrong input format for s\n";
        } else {
          shortest_path(points, length, sp, dp);
          flag_s = true;
          flag_e = true;
          flag_v = true;
        }
      } // end s if
      else if (ch == 'E' && flag_e) {
	std::cout<<line<<std::endl;
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

          bool flag_point = build_points_from_E(points);
          if (!flag_point) {
            points1.clear();
            std::cerr << "Error: in shortest path  wrong input for E\n";
          } else
            flag_e = true;
        }

      } else
        std::cerr << "Error:in shortest path  wrong input\n"<<std::flush;
    }
  }
}

void shortest_path(int points, int length, int sp, int dp) {
  int infinity = 9999999;
  int dis[points];
  int flag[points];
  int edg[points][points];
  if (sp == dp) {
    std::cout << sp << std::endl;
    return;
  }
  if (length == 1) {
    std::cout << 0 << std::endl;
  }
  for (int i = 0; i < points; i++) {
    for (int j = 0; j < points; j++) {
      if (i == j) {
        edg[i][j] = 0;
      } else {
        edg[i][j] = infinity;
      }
    }
  }

  for (int i = 0; i < length; i = i + 2) {

    edg[points1.at(i)][points1.at(i + 1)] = 1;
    edg[points1.at(i + 1)][points1.at(i)] = 1;
  }

  // setting distance to starting point
  for (int i = 0; i < points; i++) {
    dis[i] = edg[sp][i];
  }

  // setting flag for starting point
  for (int i = 0; i < points; i++) {
    flag[i] = 0;
  }
  flag[sp] = 1;
  int min, u;
  std::vector<tuple<int, int>> first;
  for (int i = 0; i < points - 1; i++) {
    min = infinity;
    for (int j = 0; j < points; j++) {
      if (flag[j] == 0 && dis[j] < min) {
        min = dis[j];
        u = j;
      }
    }
    std::tuple<int, int> foo(u, sp);
    // int u_sp=u*10+sp;
    first.emplace_back(foo);
    flag[u] = 1;
    for (int v = 0; v < points; v++) {
      if (edg[u][v] < infinity) {

        if (dis[v] > dis[u] + edg[u][v]) {
          dis[v] = dis[u] + edg[u][v];
          std::tuple<int, int> foo(u, v);
          first.emplace_back(foo);
        }
      }
    }
  }
  // int tmp=-1;
  int tmp_dp = dp;
  if (dis[dp] != infinity) {
    std::vector<int> result;
    for (std::vector<tuple<int, int>>::iterator it = first.end() - 1;
         it != first.begin(); --it) {

      if (std::get<1>(*it) == tmp_dp) {
        // std::cout  <<std::get<0>(*it)<<"->"<<std::get<1>(*it)<<std::endl;
        result.emplace_back(tmp_dp);
        // tmp=(*it/10);
        tmp_dp = std::get<0>(*it);
        // std::cout  << tmp_dp<<"this is tmp_dp"<<std::endl;
      }
      // std::cout << ' ' <<std::get<0>(*it)<<std::get<1>(*it)<<endl;
    }
    // std::cout  << sp<<"->"<<tmp_dp<<std::endl;
    result.emplace_back(tmp_dp);
    result.emplace_back(sp);
    for (std::vector<int>::reverse_iterator it = result.rbegin();
         it != result.rend(); ++it) {
      if (it != result.rend() - 1)
        std::cout << *it << "-";
      else
        std::cout << *it<<std::endl;
    }
  } else {
    std::cout << "Error: there is no path";
  }
 
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
