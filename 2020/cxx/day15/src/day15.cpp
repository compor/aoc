//
//
//

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <sstream>
#include <utility>
#include <vector>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "incorrect number of arguments provided!\n";
    std::exit(EXIT_FAILURE);
  }

  std::ifstream fin;
  fin.open(argv[1]);

  if (!fin) {
    std::cerr << "unable to open file: " << argv[1] << "!\n";
    std::exit(EXIT_FAILURE);
  }

  std::string inputval;

  while (std::getline(fin, inputval)) {
    auto input = inputval;
  }

  // std::cout << part1() << std::endl;

  return EXIT_SUCCESS;
}
