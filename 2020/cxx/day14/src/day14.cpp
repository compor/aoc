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
    mem[ins.loc] = val;
  }
}

mask_t apply_mask2(const mask_t &mask, ull loc) {
  mask_t fa{""};

  for (auto i = 0ul; i < mask.size(); ++i) {
    if (mask[i] == 'X') {
      fa.push_back('X');
    } else if (mask[i] == '1') {
      fa.push_back('1');
    } else {
      fa.push_back(loc & (1ULL << i) ? '1' : '0');
    }
  }

  return fa;
}

void generate_addresses(const mask_t &fa, ull a, ull offset,
                        std::vector<ull> &addresses) {
  if (offset >= fa.size()) {
    addresses.push_back(a);
    return;
  }

  if (fa[offset] != 'X') {
    if (fa[offset] == '1') {
      a |= 1ULL << offset;
    }

    offset++;
    generate_addresses(fa, a, offset, addresses);
  } else {
    generate_addresses(fa, a, offset + 1, addresses);

    a |= 1ULL << offset;
    generate_addresses(fa, a, offset + 1, addresses);
  }
}

void execute2(const ins_t &ins, mask_t &mask, mem_t &mem) {
  if (ins.mask != "") {
    mask = ins.mask;
  } else {
    auto loc = apply_mask2(mask, ins.loc);
    std::vector<ull> addresses;

    generate_addresses(loc, 0, 0, addresses);

    for (const auto &e : addresses) {
      mem[e] = ins.val;
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
  mask_t mask, mask2;
  mem_t mem, mem2;

  while (std::getline(fin, inputval)) {
    auto input = parse_entry(inputval);
    execute(input, mask, mem);
    execute2(input, mask2, mem2);
  }

  const auto &sum_address_space = [](const mem_t &mem) {
    ull sum = 0;
    for (const auto &e : mem) {
      sum += e.second;
    }
    return sum;
  };

  std::vector<ull> addresses;

  std::cout << sum_address_space(mem) << std::endl;
  std::cout << sum_address_space(mem2) << std::endl;

  return EXIT_SUCCESS;
}
