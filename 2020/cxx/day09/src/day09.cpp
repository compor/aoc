//
//
//

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using ull = unsigned long long;
using bag2idx = std::map<std::string, ull>;

bool is_reachable(const std::vector<std::vector<ull>> &g,
                  std::set<ull> &visited, ull src, ull dst) {
  if (src == dst)
    return true;

  if (visited.count(src)) {
    return false;
  }
  visited.insert(src);

  for (auto i = 0ull; i < g[src].size(); ++i) {
    if (g[src][i] && is_reachable(g, visited, i, dst)) {
      return true;
    }
  }

  return false;
}

void count(const std::vector<std::vector<ull>> &g, ull src, ull &sum,
           ull cnt = 1) {
  for (auto i = 0ull; i < g[src].size(); ++i) {
    if (g[src][i]) {
      sum += cnt * g[src][i];
      count(g, i, sum, cnt * g[src][i]);
    }
  }
}

int main(int argc, char *argv[]) {
  std::ifstream fin;
  fin.open(argv[1]);

  std::string inputval;
  bag2idx bags;

  ull idx = 0;
  while (std::getline(fin, inputval)) {
    std::istringstream ss(inputval);
    std::string stok;

    std::string srcbag{""};
    ss >> stok;
    srcbag += stok;
    ss >> stok;
    srcbag += " " + stok;
    // std::cout << srcbag << std::endl;
    bags[srcbag] = idx++;
  }

  fin.clear();
  fin.seekg(0);

  std::vector<ull> col(bags.size(), 0ull);
  std::vector<std::vector<ull>> g(bags.size(), col);

  while (std::getline(fin, inputval)) {
    for (auto &c : inputval) {
      if (c == ',')
        c = ' ';
    }
    std::istringstream ss(inputval);
    std::string stok;

    std::string srcbag{""};
    ss >> stok;
    srcbag += stok;
    ss >> stok;
    srcbag += " " + stok;
    // std::cout << srcbag << std::endl;

    ss >> stok >> stok;

    std::string num, destbag;
    while (ss.good()) {
      ss >> num >> destbag >> stok;
      if (num == "no")
        break;
      destbag += " " + stok;
      ss >> stok;

      ull n = std::stoull(num);
      g[bags[srcbag]][bags[destbag]] = n;
    }
  }

  auto shinygold = bags["shiny gold"];

  ull cnt = 0;
  for (auto i = 0ull; i < g.size(); ++i) {
    std::set<ull> visited;
    if (i != shinygold) {
      if (is_reachable(g, visited, i, shinygold))
        cnt++;
    }
  }

  // for (auto &col : g) {
  // for (auto &e : col) {
  // std::cout << e;
  //}
  // std::cout << '\n';
  //}

  ull sum = 0;
  count(g, shinygold, sum);

  std::cout << cnt << std::endl;
  std::cout << sum << std::endl;

  return EXIT_SUCCESS;
}
