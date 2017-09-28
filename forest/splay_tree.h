/**
 * @file splay_tree.h
 */

#ifndef SPLAY_TREE_H
#define SPLAY_TREE_H

#include <iostream>
#include <algorithm>
#include <queue>
#include <fstream>
#include <memory>

/**
 * @brief The forest library namespace
 */
namespace forest {
        template <typename key_t, typename value_t>
        struct splay_tree_node {
                key_t key;     ///< The key of the node
                value_t value; ///< The value of the node
                std::weak_ptr<splay_tree_node> parent;  ///< A pointer to the parent of the node
                std::shared_ptr<splay_tree_node> left;    ///< A pointer to the left child of the node
                std::shared_ptr<splay_tree_node> right;   ///< A pointer to the right child of the node
                /**
                 * @brief Constructor of a splay tree node
                 */
                splay_tree_node(key_t key, value_t value) {
                        this->key = key;
                        this->value = value;
						this->parent.reset();
                        this->left = nullptr;
                        this->right = nullptr;
                }
                /**
                 * @brief Prints to the std::cout information about the node
                 */
                void info() const {
                        std::cout << this->key << "\t";
                        if (this->left != nullptr) {
                                std::cout << this->left->key << "\t";
                        } else {
                                std::cout << "null" << "\t";
                        }
                        if (this->right != nullptr) {
                                std::cout << this->right->key << "\t";
                        } else {
                                std::cout << "null" << "\t";
                        }
                        if (this->parent.lock() != nullptr) {
                                std::cout << this->parent.lock()->key << std::endl;
                        } else {
                                std::cout << "null" << std::endl;
                        }
                }
        };
        template <typename key_t, typename value_t>
        class splay_tree {
        private:
                std::shared_ptr<splay_tree_node <key_t, value_t> > root;
                void pre_order_traversal(std::shared_ptr<splay_tree_node <key_t, value_t> > &x) {
                        if (x == nullptr) return;
                        x->info();
                        pre_order_traversal(x->left);
                        pre_order_traversal(x->right);
                }
                void in_order_traversal(std::shared_ptr<splay_tree_node <key_t, value_t> > &x) {
                        if (x == nullptr) return;
                        in_order_traversal(x->left);
                        x->info();
                        in_order_traversal(x->right);
                }
                void post_order_traversal(std::shared_ptr<splay_tree_node <key_t, value_t> > &x) {
                        if (x == nullptr) return;
                        post_order_traversal(x->left);
                        post_order_traversal(x->right);
                        x->info();
                }
                void breadth_first_traversal(std::shared_ptr<splay_tree_node <key_t, value_t> > &x) {
                        std::queue <std::shared_ptr<splay_tree_node <key_t, value_t> > > queue;
                        if (x == nullptr) return;
                        queue.push(x);
                        while(queue.empty() == false) {
                                std::shared_ptr<splay_tree_node <key_t, value_t> > y = queue.front();
                                y->info();
                                queue.pop();
                                if (y->left != nullptr) queue.push(y->left);
                                if (y->right != nullptr) queue.push(y->right);
                        }
                }
                unsigned long long height(std::shared_ptr<splay_tree_node <key_t, value_t> > &x) {
                        if (x == nullptr) return 0;
                        return std::max(height(x->left), height(x->right)) + 1;
                }
                unsigned long long size(std::shared_ptr<splay_tree_node <key_t, value_t> > &x) {
                        if (x == nullptr) return 0;
                        return size(x->left) + size(x->right) + 1;
                }
                void graphviz(std::ofstream &file, std::shared_ptr<splay_tree_node <key_t, value_t> > &x, unsigned long long *count) {
                        if (x == nullptr) return;
                        graphviz(file, x->left, count);
                        if (x->left != nullptr) {
                                file << "\t" << x->key << " -> " << x->left->key << ";" << std::endl;
                        } else {
                                file << "\t" << "null" << *count << " " << "[shape=point]" << ";" << std::endl;
                                file << "\t" << x->key << " -> " << "null" << *count << ";" << std::endl;
                                (*count)++;
                        }
                        if (x->right != nullptr) {
                                file << "\t" << x->key << " -> " << x->right->key << ";" << std::endl;
                        } else {
                                file << "\t" << "null" << *count << " " << "[shape=point]" << ";" << std::endl;
                                file << "\t" << x->key << " -> " << "null" << *count << ";" << std::endl;
                                (*count)++;
                        }
                        graphviz(file, x->right, count);
                }
                void left_rotate(std::shared_ptr<splay_tree_node <key_t, value_t> > &x) {
                        std::shared_ptr<splay_tree_node <key_t, value_t> > y = x->right;
                        if(y != nullptr) {
                                x->right = y->left;
                                if(y->left != nullptr) y->left->parent = x;
                                y->parent = x->parent;
                        }
                        if(x->parent.lock() == nullptr) {
                                root = y;
                        } else if (x == x->parent.lock()->left) {
                                x->parent.lock()->left = y;
                        } else {
                                x->parent.lock()->right = y;
                        }
                        if(y != nullptr) {
                                y->left = x;
                        }
                        x->parent = y;
                }
                void right_rotate(std::shared_ptr<splay_tree_node <key_t, value_t> > &x) {
                        std::shared_ptr<splay_tree_node <key_t, value_t> > y = x->left;
                        if (y != nullptr) {
                                x->left = y->right;
                                if (y->right != nullptr) y->right->parent = x;
                                y->parent = x->parent;
                        }
                        if(x->parent.lock() == nullptr) {
                                root = y;
                        } else if (x == x->parent.lock()->left) {
                                x->parent.lock()->left = y;
                        } else {
                                x->parent.lock()->right = y;
                        }
                        if(y != nullptr) {
                                y->right = x;
                        }
                        x->parent = y;
                }
                std::shared_ptr<splay_tree_node <key_t, value_t> > find_parent(std::shared_ptr<splay_tree_node <key_t, value_t> > &x) {
                        return x->parent.lock();
                }
                std::shared_ptr<splay_tree_node <key_t, value_t> > find_grand_parent(std::shared_ptr<splay_tree_node <key_t, value_t> > &x) {
                        if (find_parent(x) != nullptr) {
                                return find_parent(x)->parent.lock();
                        }
                        return nullptr;
                }
                void splay(std::shared_ptr<splay_tree_node <key_t, value_t> > &x) {
                        while (find_parent(x) != nullptr) {
                                if (find_grand_parent(x) == nullptr) {
                                        if (find_parent(x)->left == x) {
                                                right_rotate(find_parent(x));
                                        } else if (find_parent(x)->right == x){
                                                left_rotate(find_parent(x));
                                        }
                                } else if (find_parent(x)->left == x && find_grand_parent(x)->left == find_parent(x)) {
                                        right_rotate(find_grand_parent(x));
                                        right_rotate(find_parent(x));
                                } else if (find_parent(x)->right == x && find_grand_parent(x)->right == find_parent(x)) {
                                        left_rotate(find_grand_parent(x));
                                        left_rotate(find_parent(x));
                                } else if (find_parent(x)->left == x && find_grand_parent(x)->right == find_parent(x)) {
                                        right_rotate(find_parent(x));
                                        left_rotate(find_parent(x));
                                } else if (find_parent(x)->right == x && find_grand_parent(x)->left == find_parent(x)){
                                        left_rotate(find_parent(x));
                                        right_rotate(find_parent(x));
                                }
                        }
                }
        public:
                splay_tree() {
                        root = nullptr;
                }
                ~splay_tree() {

                }
                /**
                 * @brief Performs a Pre Order Traversal starting from the root node
                 * @return void
                 */
                void pre_order_traversal() {
                        pre_order_traversal(root);
                }
                /**
                 * @brief Performs a In Order Traversal starting from the root node
                 * @return void
                 */
                void in_order_traversal() {
                        in_order_traversal(root);
                }
                /**
                 * @brief Performs a Post Order Traversal starting from the root node
                 * @return void
                 */
                void post_order_traversal() {
                        post_order_traversal(root);
                }
                /**
                 * @brief Performs a Breadth First Traversal starting from the root node
                 * @return void
                 */
                void breadth_first_traversal() {
                        breadth_first_traversal(root);
                }
                /**
                 * @brief Generates a .dot file representing the Splay Tree
                 * @param filename The filename of the .dot file
                 * @return void
                 */
                void graphviz(const std::string &filename) {
                        std::ofstream file;
                        unsigned long long count = 0;
                        file.open(filename);
                        file << "digraph {" << std::endl;
                        graphviz(file, root, &count);
                        file << "}" << std::endl;
                        file.close();
                }
                /**
                 * @brief Inserts a new node into the Splay Tree
                 * @param key The key for the new node
                 * @param value The value for the new node
                 * @return true if the new node was inserted and false otherwise
                 */
                const std::shared_ptr<splay_tree_node <key_t, value_t> > insert(key_t key, value_t value) {
                        std::shared_ptr<splay_tree_node <key_t, value_t> > current = root;
                        std::shared_ptr<splay_tree_node <key_t, value_t> > parent = nullptr;
                        while(current!=nullptr) {
                                parent = current;
                                if (key > current->key) {
                                        current = current->right;
                                } else if (key < current->key) {
                                        current = current->left;
                                } else {
                                        return current;
                                }
                        }
                        current = std::make_shared<splay_tree_node <key_t, value_t> >(key, value);
                        current->parent = parent;
                        if(parent == nullptr) {
                                root = current;
                        } else if (current->key > parent->key) {
                                parent->right = current;
                        } else if (current->key < parent->key) {
                                parent->left = current;
                        }
                        splay(current);
                        return current;
                }
                /**
                 * @brief Performs a binary search starting from the root node
                 * @return The node with the key specified
                 */
                const std::shared_ptr<splay_tree_node <key_t, value_t> > search(key_t key) {
                        std::shared_ptr<splay_tree_node <key_t, value_t> > x = root;
                        while (x != nullptr) {
                                if (key > x->key) {
                                        x = x->right;
                                } else if (key < x->key) {
                                        x = x->left;
                                } else {
                                        return x;
                                }
                        }
                        return nullptr;
                }
                /**
                 * @brief Finds the node with the minimum key
                 * @return The node with the minimum key
                 */
                const std::shared_ptr<splay_tree_node <key_t, value_t> > minimum() {
                        std::shared_ptr<splay_tree_node <key_t, value_t> > x = root;
                        if (x == nullptr) return nullptr;
                        while(x->left != nullptr) x = x->left;
                        return x;
                }
                /**
                 * @brief Finds the node with the maximum key
                 * @return The node with the maximum key
                 */
                const std::shared_ptr<splay_tree_node <key_t, value_t> > maximum() {
                        std::shared_ptr<splay_tree_node <key_t, value_t> > x = root;
                        if (x == nullptr) return nullptr;
                        while(x->right != nullptr) x = x->right;
                        return x;
                }
                /**
                 * @brief Finds the height of the tree
                 * @return The height of the splay tree
                 */
                unsigned long long height() {
                        return height(root);
                }
                /**
                 * @brief Finds the size of the tree
                 * @return The size of the splay tree
                 */
                unsigned long long size() {
                        return size(root);
                }
                /**
                 * @brief Finds if the splay tree is empty
                 * @return true if the splay tree is empty and false otherwise
                 */
                bool empty() {
                        if (root == nullptr) {
                                return true;
                        } else {
                                return false;
                        }
                }
        };
}

#endif
