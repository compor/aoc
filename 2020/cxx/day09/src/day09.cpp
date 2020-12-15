//
//
//

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <list>
#include <numeric>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using ull = unsigned long long;

int main(int argc, char *argv[]) {
  std::ifstream fin;
  fin.open(argv[1]);

  std::string inputval;
  std::list<ull> window;
  std::set<ull> p;
  const ull n = 5; // preamble length
  ull val = 0;

  ull i = 0;
  while (fin >> inputval) {
    val = std::stoull(inputval);

    if (i < n) {
      window.push_back(val);
      p.insert(val);
    } else {
      for (const auto &e : window) {
        auto c = val - e;
        if (p.count(c)) {
          goto found;
        }
      }

      break;

    found:
      p.erase(window.front());
      window.pop_front();
      window.push_back(val);
      p.insert(val);
    }

    i++;
  }

  std::cout << val << std::endl;

  return EXIT_SUCCESS;
}
