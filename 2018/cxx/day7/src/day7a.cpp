//
//
//

#include <iostream>
// using std::cout
// using std::cerr

#include <string>
// using std::string

#include <vector>
// using std::vector

#include <map>
// using std::map

#include <regex>
// using std::regex

#include <fstream>
// using std::ifstream

#include <string>
// using std::string

using vertex_data_t = std::string;
using constraints_t = std::vector<std::pair<vertex_data_t, vertex_data_t>>;

using graph_t = std::map<vertex_data_t, std::vector<vertex_data_t>>;

void build_graph(const constraints_t &c, graph_t &g) {
  graph_t::mapped_type neighbours;

  for (auto &e : c) {
    std::for_each(c.begin(), c.end(), [&](auto &v) {
      if (e.first == v.first) {
        neighbours.emplace_back(v.second);
      }
    });

    std::sort(neighbours.begin(), neighbours.end());

    g[e.first] = neighbours;
  }
}

decltype(auto) reverse_constraints(const constraints_t &c) {
  constraints_t rc;

  for (auto &e : c) {
    rc.emplace_back(e.second, e.first);
  }

  return rc;
}

decltype(auto) is_ready(vertex_data_t &v, graph_t &rg,
                        std::map<vertex_data_t, bool> &visited) {
  return std::all_of(rg[v].begin(), rg[v].end(),
                     [&](auto &e) { return visited[e]; });
}

decltype(auto) traversal(graph_t &rg) {
  std::vector<vertex_data_t> to_visit;
  std::vector<vertex_data_t> order;
  std::map<vertex_data_t, bool> visited;

  for (auto &e : rg) {
    to_visit.push_back(e.first);
    for (auto &c : e.second) {
      to_visit.push_back(c);
    }
  }

  auto sue = [](auto &v) {
    std::sort(v.begin(), v.end());
    v.erase(std::unique(v.begin(), v.end()), v.end());
  };

  sue(to_visit);

  for (auto &e : to_visit) {
    visited[e] = false;
  }

  while (!to_visit.empty()) {
    std::sort(to_visit.begin(), to_visit.end());

    auto it = std::find_if(to_visit.begin(), to_visit.end(),
                           [&](auto &e) { return is_ready(e, rg, visited); });
    if (it == to_visit.end()) {
      std::cout << "boom\n";
      break;
    }

    vertex_data_t c = *it;

    visited[c] = true;
    order.push_back(c);

    to_visit.erase(std::remove(to_visit.begin(), to_visit.end(), c),
                   to_visit.end());
  }

  return order;
}

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

  std::string raw_entry;
  std::vector<std::string> raw_entries;

  while (std::getline(fin, raw_entry)) {
    raw_entries.push_back(raw_entry);
  }

  std::regex entry_regex{"Step ([[:graph:]]) must be finished before step "
                         "([[:graph:]]) can begin."};
  std::smatch matches;

  constraints_t c;
  for (const auto &e : raw_entries) {
    if (std::regex_match(e, matches, entry_regex)) {
      if (matches.size() == 3) {
        c.emplace_back(matches[1].str(), matches[2].str());
      }
    }
  }

  auto rc = reverse_constraints(c);
  graph_t rg;
  build_graph(rc, rg);
  auto o = traversal(rg);

  std::cout << "result: " << '\n';
  for (auto &e : o) {
    std::cout << e;
  }
  std::cout << '\n';

  return 0;
}
