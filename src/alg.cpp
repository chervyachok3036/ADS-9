// Copyright 2022 NNTU-CS
#include "tree.h"

#include <iostream>
#include <fstream>
#include <locale>
#include <cstdlib>
#include <vector>

void PMTree::BuildTree(TreeNode* node, const std::vector<char>& remaining) {
  if (remaining.empty()) return;

  for (size_t i = 0; i < remaining.size(); ++i) {
    TreeNode* child = new TreeNode(remaining[i]);
    node->children.push_back(child);

    std::vector<char> next;
    next.reserve(remaining.size() - 1);
    for (size_t j = 0; j < remaining.size(); ++j) {
      if (j != i) next.push_back(remaining[j]);
    }
    BuildTree(child, next);
  }
}

PMTree::PMTree(const std::vector<char>& elems)
    : root_(new TreeNode('\0')), depth_(static_cast<int>(elems.size())) {
  BuildTree(root_, elems);
}

PMTree::~PMTree() { delete root_; }

namespace {

void Dfs(TreeNode* node,
         std::vector<char>* path,
         int target_depth,
         std::vector<std::vector<char>>* result) {
  if (static_cast<int>(path->size()) == target_depth) {
    result->push_back(*path);
    return;
  }
  for (TreeNode* child : node->children) {
    path->push_back(child->value);
    Dfs(child, path, target_depth, result);
    path->pop_back();
  }
}

long long Factorial(int n) {
  long long result = 1;
  for (int i = 2; i <= n; ++i) result *= i;
  return result;
}
}

std::vector<std::vector<char>> getAllPerms(const PMTree& tree) {
  std::vector<std::vector<char>> result;
  if (!tree.GetRoot()) return result;

  std::vector<char> path;
  Dfs(tree.GetRoot(), &path, tree.GetDepth(), &result);
  return result;
}

std::vector<char> getPerm1(PMTree& tree, int num) {
  auto all = getAllPerms(tree);
  if (num < 1 || num > static_cast<int>(all.size())) return {};
  return all[num - 1];
}

std::vector<char> getPerm2(PMTree& tree, int num) {
  if (!tree.GetRoot() || tree.GetDepth() == 0) return {};

  const int n = tree.GetDepth();
  if (num < 1 || static_cast<long long>(num) > Factorial(n)) return {};

  std::vector<char> result;
  result.reserve(n);

  TreeNode* current = tree.GetRoot();
  int remainder = num - 1;

  for (int level = 0; level < n; ++level) {
    const long long branch_size = Factorial(n - level - 1);
    const int child_idx = static_cast<int>(remainder / branch_size);
    remainder = static_cast<int>(remainder % branch_size);

    if (child_idx >= static_cast<int>(current->children.size())) return {};

    current = current->children[child_idx];
    result.push_back(current->value);
  }

  return result;
}
