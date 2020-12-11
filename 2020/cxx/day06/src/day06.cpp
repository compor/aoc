//
//
//

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <optional>
#include <regex>
#include <set>
#include <sstream>
#include <utility>
#include <vector>

using group_answer_t = std::set<char>;

using customs_declaration_t = std::vector<std::string>;

group_answer_t parse_entry(const std::string &entry) {
  std::regex entry_regex{"\\s*([a-z]*)\\s*"};
  std::smatch matches;
  group_answer_t answers;
  std::string entryval;
  std::istringstream iss(entry);

  while (std::getline(iss, entryval, ' ')) {
    if (std::regex_match(entryval, matches, entry_regex)) {
      for (auto &e : matches[1].str()) {
        answers.insert(e);
      }
    }
  }

  return answers;
}

auto parse_list(const customs_declaration_t &cd) {
  std::vector<group_answer_t> group_answers;
  std::string current = "";

  for (auto &e : cd) {
    if (e == "") {
      group_answers.push_back(parse_entry(current));
      current = "";
    }

    current += e;
  }

  group_answers.push_back(parse_entry(current));

  return group_answers;
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
  customs_declaration_t input;

  while (std::getline(fin, inputval)) {
    input.push_back(inputval);
  }

  const auto &customs_declarations = parse_list(input);

  const auto &part1 = [&customs_declarations]() {
    auto acc = 0;
    for_each(customs_declarations.begin(), customs_declarations.end(),
             [&acc](const auto &e) { acc += e.size(); });
    return acc;
  };

  std::cout << part1() << std::endl;

  return EXIT_SUCCESS;
}
