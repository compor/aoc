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

using mask_t = std::string;
using ull = unsigned long long;

struct ins_t {
  mask_t mask;
  ull loc;
  ull val;
};

using mem_t = std::map<ull, ull>;

ins_t parse_entry(const std::string &entry) {
  std::string entryval;
  std::istringstream iss(entry);
  ins_t instruction{"", 0, 0};

  while (std::getline(iss, entryval, ' ')) {
    if (entryval == "mask") {
      std::getline(iss, entryval, ' ');
      std::getline(iss, entryval, ' ');
      std::reverse(entryval.begin(), entryval.end());
      instruction.mask = entryval;
    } else {
      entryval.erase(0, 4); // remove "mem[" prefix
      entryval.erase(entryval.find(']'));
      instruction.loc = std::stoull(entryval);
      std::getline(iss, entryval, ' ');
      std::getline(iss, entryval, ' ');
      instruction.val = std::stoull(entryval);
    }
  }

  return instruction;
}

ull apply_mask(const mask_t &mask, ull val) {
  for (auto i = 0ul; i < mask.size(); ++i) {
    if (mask[i] == '0') {
      ull bitmask = ~0ULL & ~(1ULL << i);
      val &= bitmask;
    } else if (mask[i] == '1') {
      ull bitmask = 1ULL << i;
      val |= bitmask;
    }
  }

  return val;
}

void execute(const ins_t &ins, mask_t &mask, mem_t &mem) {
  if (ins.mask != "") {
    mask = ins.mask;
  } else {
    auto val = apply_mask(mask, ins.val);
    if (!mem.count(ins.loc)) {
      mem.insert({ins.loc, val});
    } else {
      mem[ins.loc] = val;
    }
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

  std::string inputval;
  mask_t mask;
  mem_t mem;

  while (std::getline(fin, inputval)) {
    auto input = parse_entry(inputval);
    execute(input, mask, mem);
  }

  const auto &part1 = [](const mem_t &mem) {
    ull sum = 0;
    for (const auto &e : mem) {
      sum += e.second;
    }
    return sum;
  };

  std::cout << part1(mem) << std::endl;

  return EXIT_SUCCESS;
}
