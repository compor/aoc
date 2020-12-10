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

  std::string inputval;
  std::vector<std::string> input;

  while (std::getline(fin, inputval)) {
    input.push_back(inputval);
  }

  return 0;
}
