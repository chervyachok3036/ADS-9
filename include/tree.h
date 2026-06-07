// Copyright 2022 NNTU-CS
#ifndef INCLUDE_TREE_H_
#define INCLUDE_TREE_H_

#include <vector>

struct TreeNode {
  char value;
  std::vector<TreeNode*> children;

  explicit TreeNode(char val) : value(val) {}

  ~TreeNode() {
    for (TreeNode* child : children) {
      delete child;
    }
  }
};

class PMTree {
 public:
  explicit PMTree(const std::vector<char>& elems);
  ~PMTree();

  TreeNode* GetRoot() const { return root_; }
  int GetDepth() const { return depth_; }

 private:
  TreeNode* root_;
  int depth_;

  void BuildTree(TreeNode* node, const std::vector<char>& remaining);
};

std::vector<std::vector<char>> getAllPerms(const PMTree& tree);
std::vector<char> getPerm1(PMTree& tree, int num);
std::vector<char> getPerm2(PMTree& tree, int num);

#endif  // INCLUDE_TREE_H_
