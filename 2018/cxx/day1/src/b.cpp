//
//
//

#include <cstdlib>
// using std::exit

#include <iostream>
// using std::cout
// using std::cerr

#include <fstream>
// using std::ifstream

#include <vector>
// using std::vector

#include <map>
// using std::map

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "incorrect number of arguments provided!\n";
    std::exit(-1);
  }

  std::ifstream fin;
  fin.open(argv[1]);

  if (!fin) {
    std::cerr << "unable to open file: " << argv[1] << "!\n";
    std::exit(-1);
  }

  std::map<int, unsigned> frequency_count;
  std::vector<int> changes;

  int change = 0;
  while (fin >> change) {
    changes.push_back(change);
  }

  int frequency = 0;
  frequency_count[frequency] = 1;

  bool found = false;
  while (!found) {
    for (auto c : changes) {
      frequency += c;

      if (!frequency_count.count(frequency)) {
        frequency_count[frequency] = 1;
      } else {
        frequency_count[frequency] += 1;

        if (frequency_count[frequency] >= 2) {
          found = true;
          break;
        }
      }
    }
  }

  std::cout << "first recurring frequency: " << frequency << "\n";

  return 0;
}
