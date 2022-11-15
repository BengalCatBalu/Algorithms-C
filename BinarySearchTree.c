#include <iostream>

class Node {
public:
    uint32_t data;
    Node* left;
    Node* right;
    explicit Node(uint32_t key) : data(key), left(nullptr), right(nullptr) {
    }
};

class Tree {
public:
    Tree() {
        root_ = nullptr;
    };

    Tree(uint32_t a) {
        root_ = new Node(a);
    }

    ~Tree() {
        delet(root_);
    };

    void insert(uint32_t key) {
        Node* curr = root_;
        if (curr == nullptr) {
            root_ = new Node(key);
            return;
        }
        while (curr && curr->data != key) {
            if (curr->data > key && curr->left == nullptr) {
                curr->left = new Node(key);
                return;
            }
            if (curr->data < key && curr->right == nullptr) {
                curr->right = new Node(key);
                return;
            }
            if (curr->data > key) {
                curr = curr->left;
            } else {
                curr = curr->right;
            }
        }
    };

    uint32_t getHeight() const {
        return height(root_);
    };
    uint32_t height(Node* rot) const {
        if (rot == nullptr) {
            return 0;
        }
        return 1 + std::max(height(rot->left), height(rot->right));
    }

    uint32_t max2() {
        Node* curr = root_;
        if (root_->right == nullptr) {
            curr = root_->left;
            while (curr->right) {
                curr = curr->right;
            }
            return curr->data;
        }
        while (curr->right->right) {
            curr = curr->right;
        }
        Node* firstvar = curr;
        Node* secondvar;
        if (curr->right->left) {
            secondvar = curr->right->left;
            while (secondvar->right) {
                secondvar = secondvar->right;
            }
            return std::max(secondvar->data, firstvar->data);
        }
        return firstvar->data;
    };
    void print() {
        prints(root_);
    }
    void prints(Node* rot) {
        if (rot) {
            prints(rot->left);
            std::cout << rot->data << '\n';
            prints(rot->right);
        }
    }
    void list() {
        lists(root_);
    }
    void lists(Node* rot) {
        if (rot) {
            lists(rot->left);
            if (!rot->left && !rot->right) {
                std::cout << rot->data << '\n';
            }
            lists(rot->right);
        }
    }

private:
    void delet(Node* node) {
        if (node) {
            delet(node->left);
            delet(node->right);
            delete node;
        }
    }
    Node* root_;
};

int main() {
    Tree tr = Tree();
    uint32_t a;
    std::cin >> a;
    while (a != 0) {
        tr.insert(a);
        std::cin >> a;
    }
    tr.list();
}
