// Copyright 2022 NNTU-CS
#include "tree.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

namespace {

void PrintPerm(const std::vector<char>& perm) {
  for (char c : perm) std::cout << c;
  std::cout << '\n';
}

long long Factorial(int n) {
  long long result = 1;
  for (int i = 2; i <= n; ++i) result *= i;
  return result;
}

void RunDemo() {
  const std::vector<char> kInput = {'1', '2', '3'};
  PMTree tree(kInput);

  auto perms = getAllPerms(tree);
  std::cout << "getAllPerms (" << perms.size() << "):\n";
  for (const auto& p : perms) PrintPerm(p);

  std::cout << "\ngetPerm1(tree, 1): ";
  PrintPerm(getPerm1(tree, 1));
  std::cout << "getPerm1(tree, 6): ";
  PrintPerm(getPerm1(tree, 6));

  std::cout << "\ngetPerm2(tree, 1): ";
  PrintPerm(getPerm2(tree, 1));
  std::cout << "getPerm2(tree, 2): ";
  PrintPerm(getPerm2(tree, 2));

  std::cout << "\ngetPerm2(tree, 8): ";
  const auto bad = getPerm2(tree, 8);
  std::cout << (bad.empty() ? "(empty)" : "") << '\n';
}

using Clock = std::chrono::high_resolution_clock;
using Nanoseconds = std::chrono::nanoseconds;

long long MeasureGetAllPerms(int n) {
  std::vector<char> alpha;
  for (int i = 1; i <= n; ++i) alpha.push_back(static_cast<char>('0' + i));
  PMTree tree(alpha);

  auto t0 = Clock::now();
  auto perms = getAllPerms(tree);
  auto t1 = Clock::now();
  (void)perms;
  return std::chrono::duration_cast<Nanoseconds>(t1 - t0).count();
}

long long MeasureGetPerm(int n, bool use_fast, std::mt19937* rng) {
  std::vector<char> alpha;
  for (int i = 1; i <= n; ++i) alpha.push_back(static_cast<char>('0' + i));
  PMTree tree(alpha);

  std::uniform_int_distribution<long long> dist(1, Factorial(n));
  const int num = static_cast<int>(dist(*rng));

  auto t0 = Clock::now();
  auto perm = use_fast ? getPerm2(tree, num) : getPerm1(tree, num);
  auto t1 = Clock::now();
  (void)perm;
  return std::chrono::duration_cast<Nanoseconds>(t1 - t0).count();
}

void RunTimingExperiment() {
  std::cout << "\nn\tgetAllPerms\tgetPerm1\tgetPerm2\n";

  std::mt19937 rng(42);
  constexpr int kNMax = 8;

  std::vector<int> sizes;
  std::vector<long long> t_all, t_p1, t_p2;

  for (int n = 1; n <= kNMax; ++n) {
    const long long ta = MeasureGetAllPerms(n);
    const long long t1 = MeasureGetPerm(n, false, &rng);
    const long long t2 = MeasureGetPerm(n, true, &rng);

    sizes.push_back(n);
    t_all.push_back(ta);
    t_p1.push_back(t1);
    t_p2.push_back(t2);

    std::cout << n << '\t' << ta << '\t' << t1 << '\t' << t2 << '\n';
  }

  std::ofstream py("result/make_plot.py");

  auto WriteList = [&](const std::string& name,
                       const std::vector<long long>& v) {
    py << name << " = [";
    for (size_t i = 0; i < v.size(); ++i) {
      py << v[i];
      if (i + 1 < v.size()) py << ", ";
    }
    py << "]\n";
  };

  py << "import matplotlib\nmatplotlib.use('Agg')\n";
  py << "import matplotlib.pyplot as plt\n\n";

  py << "ns = [";
  for (size_t i = 0; i < sizes.size(); ++i) {
    py << sizes[i];
    if (i + 1 < sizes.size()) py << ", ";
  }
  py << "]\n";

  WriteList("t_all", t_all);
  WriteList("t_p1", t_p1);
  WriteList("t_p2", t_p2);

  py << R"(
fig, ax = plt.subplots(figsize=(8, 5))
ax.plot(ns, t_all, marker='o', label='getAllPerms')
ax.plot(ns, t_p1,  marker='s', label='getPerm1')
ax.plot(ns, t_p2,  marker='^', label='getPerm2')
ax.set_yscale('log')
ax.set_xlabel('n')
ax.set_ylabel('Time (ns), log scale')
ax.set_title('Runtime vs alphabet size')
ax.legend()
ax.grid(True, which='both', linestyle='--', alpha=0.5)
plt.tight_layout()
plt.savefig('result/plot.png', dpi=150)
)";
  py.close();
}

}  // namespace

int main() {
  RunDemo();
  RunTimingExperiment();
  return 0;
}
