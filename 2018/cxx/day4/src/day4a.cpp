//
//
//

#include <cstdlib>
// using std::exit

#include <iostream>
// using std::cout
// using std::cerr

#include <vector>
// using std::vector

#include <string>
// using std::string

#include <map>
// using std::map

#include <regex>
// using std::regex

#include <algorithm>
// using std::sort

#include <numeric>
// using std::accumulate

#include <fstream>
// using std::ifstream

struct hourmin {
  long long hour;
  long long min;

  hourmin(long long hour, long long min) : hour(hour), min(min) {}
};

struct timestamp {
  long long year;
  long long month;
  long long day;

  hourmin hm;

  timestamp(long long year, long long month, long long day, long long hour,
            long long min)
      : year(year), month(month), day(day), hm(hour, min) {}
};

struct logentry {
  timestamp ts;
  std::string text;

  logentry(long long year, long long month, long long day, long long hour,
           long long min, std::string text)
      : ts(year, month, day, hour, min), text(text) {}
};

bool lt(const logentry &lhs, const logentry &rhs) {
  if (lhs.ts.year > rhs.ts.year) {
    return false;
  }
  if (lhs.ts.year < rhs.ts.year) {
    return true;
  }

  if (lhs.ts.month > rhs.ts.month) {
    return false;
  }
  if (lhs.ts.month < rhs.ts.month) {
    return true;
  }

  if (lhs.ts.day > rhs.ts.day) {
    return false;
  }
  if (lhs.ts.day < rhs.ts.day) {
    return true;
  }

  if (lhs.ts.hm.hour > rhs.ts.hm.hour) {
    return false;
  }
  if (lhs.ts.hm.hour < rhs.ts.hm.hour) {
    return true;
  }

  if (lhs.ts.hm.min > rhs.ts.hm.min) {
    return false;
  }
  if (lhs.ts.hm.min < rhs.ts.hm.min) {
    return true;
  }

  return false;
}

struct guard_entry {
  long long id;

  std::vector<unsigned long> sleeps;

  guard_entry(long long id) : id(id), sleeps(60) {}
};

std::ostream &operator<<(std::ostream &os, const logentry &e) {
  os << e.ts.year << '-' << e.ts.month << '-' << e.ts.day << ' ' << e.ts.hm.hour
     << ':' << e.ts.hm.min << ' ' << e.text;

  return os;
}

decltype(auto) find_most_sleepyhead(const std::vector<guard_entry> &entries) {
  auto max_sleep_mins = 0;
  auto guard_it = entries.begin();

  for (auto it = entries.begin(); it != entries.end(); ++it) {
    auto sleep_mins = std::accumulate(it->sleeps.begin(), it->sleeps.end(), 0);
    if (max_sleep_mins < sleep_mins) {
      max_sleep_mins = sleep_mins;
      guard_it = it;
    }
  }

  return guard_it;
}

decltype(auto) find_sleepiest_minute(const guard_entry &ge) {
  auto max_it = std::max_element(ge.sleeps.begin(), ge.sleeps.end());
  return std::distance(ge.sleeps.begin(), max_it);
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

  std::vector<logentry> entries;
  entries.reserve(raw_entries.size());

  std::regex entry_regex{".([[:digit:]]+)-([[:digit:]]+)-([[:digit:]]+) "
                         "([[:digit:]]+):([[:digit:]]+). (.*)"};
  std::smatch matches;

  for (const auto &e : raw_entries) {
    if (std::regex_match(e, matches, entry_regex)) {
      if (matches.size() == 7) {
        char *c;
        entries.emplace_back(std::strtoul(matches[1].str().c_str(), &c, 10),
                             std::strtoul(matches[2].str().c_str(), &c, 10),
                             std::strtoul(matches[3].str().c_str(), &c, 10),
                             std::strtoul(matches[4].str().c_str(), &c, 10),
                             std::strtoul(matches[5].str().c_str(), &c, 10),
                             matches[6].str());
      }
    }
  }

  std::regex guard_regex{"Guard #([[:digit:]]+).*"};
  std::regex wakes_regex{".*wakes up.*"};
  std::regex sleeps_regex{".*falls asleep.*"};

  std::sort(entries.begin(), entries.end(), lt);
  // for (const auto &e : entries) {
  // std::cout << e << '\n';
  //}

  std::vector<guard_entry> guard_entries;

  long long gid = -1;
  decltype(guard_entries)::iterator current_guard;
  long long sleep_start = 0;
  long long sleep_end = 0;

  for (auto it = entries.begin(); it != entries.end(); ++it) {
    if (std::regex_match(it->text, matches, guard_regex)) {
      if (matches.size() == 2) {
        char *c;
        gid = std::strtoul(matches[1].str().c_str(), &c, 10);

        auto found = std::find_if(guard_entries.begin(), guard_entries.end(),
                                  [gid](const auto &e) { return e.id == gid; });

        if (found == guard_entries.end()) {
          guard_entries.emplace_back(gid);
          current_guard = --guard_entries.end();
        } else {
          current_guard = found;
        }
      }

      continue;
    }

    if (std::regex_match(it->text, matches, sleeps_regex)) {
      if (matches.size() == 1) {
        sleep_start = it->ts.hm.min;
      }

      continue;
    }

    if (std::regex_match(it->text, matches, wakes_regex)) {
      if (matches.size() == 1) {
        sleep_end = it->ts.hm.min;
      }

      for (; sleep_start < sleep_end; sleep_start++) {
        current_guard->sleeps[sleep_start]++;
      }
      sleep_start = 0;
      sleep_end = 0;

      continue;
    }
  }

  // for (const auto &e : guard_entries) {
  // std::cout << "guard " << e.id << " " << e.sleeps.size() << '\n';
  // for (auto &m : e.sleeps) {
  // std::cout << m << ' ';
  //}
  // std::cout << '\n';
  //}

  //

  const auto sleepyhead_it = find_most_sleepyhead(guard_entries);
  std::cout << "sleepyhead: " << sleepyhead_it->id << '\n';
  auto hardest_min = find_sleepiest_minute(*sleepyhead_it);
  std::cout << "hardest minute: " << hardest_min << '\n';

  std::cout << "result: " << hardest_min * sleepyhead_it->id << '\n';

  return 0;
}
