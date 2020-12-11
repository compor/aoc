//
//
//

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <utility>
#include <vector>

using group_answer_t = std::vector<std::string>;

using customs_declaration_t = std::vector<std::string>;

group_answer_t parse_entry(const std::string &entry) {
  group_answer_t answers;
  std::string entryval;
  std::istringstream iss(entry);

  while (std::getline(iss, entryval, ' ')) {
    if (entryval != "")
      answers.push_back(entryval);
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

    current += e + " ";
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
             [&](const auto &e) {
               std::set<char> collective_answer;

               for (auto &k : e) {
                 collective_answer.insert(k.begin(), k.end());
               }

               acc += collective_answer.size();
             });
    return acc;
  };
  const auto &part2 = [&customs_declarations]() {
    auto acc = 0;

    for_each(customs_declarations.begin(), customs_declarations.end(),
             [&](const auto &e) {
               std::string is(*e.begin());

               for (auto k : e) {
                 std::string tmp;

                 std::sort(is.begin(), is.end());
                 std::sort(k.begin(), k.end());

                 std::set_intersection(is.begin(), is.end(), k.begin(), k.end(),
                                       std::back_inserter(tmp));
                 is = tmp;
               }

               acc += is.size();
             });
    return acc;
  };

  std::cout << part1() << std::endl;
  std::cout << part2() << std::endl;

  return EXIT_SUCCESS;
}
