//
//
//

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <numeric>
#include <set>
#include <sstream>
#include <utility>
#include <vector>

using sll = long long int;
using ull = unsigned long long int;

struct instruction_t {
  std::string op;
  sll val;
};

using program_t = std::vector<instruction_t>;
using acc_t = sll;

void execute(program_t &program, acc_t &acc) {
  ull pc = 0;
  std::set<ull> visited;

  for (; !visited.count(pc);) {
    sll step = 1;

    if (program[pc].op == "acc") {
      acc += program[pc].val;
    } else if (program[pc].op == "jmp") {
      step = program[pc].val;
    } else {
      // nop
    }

    visited.insert(pc);
    pc += step;
  }
}

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

  std::string inputval1, inputval2;
  program_t program;

  while (fin >> inputval1 >> inputval2) {
    program.push_back({inputval1, std::atoll(inputval2.c_str())});
  }

  acc_t acc = 0;
  execute(program, acc);

  std::cout << acc << std::endl;

  return EXIT_SUCCESS;
}
