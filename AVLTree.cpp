#include <algorithm>
#include <iostream>

struct Node {
    int height;
    Node* left;
    Node* right;
    int value;
    explicit Node(int key) : height(1), left(nullptr), right(nullptr), value(key) {
    }
};

class AVLTree {
public:
    AVLTree() : root_(nullptr), size_(0){};

    int getHeight() {
        return height(root_);
    };

    void insert(int value) {
        root_ = insert(root_, value);
    };

    void erase(int value) {
        root_ = remove(root_, value);
    }
    int* find(int value) {
        Node* curr = root_;
        if (curr == nullptr) {
            return nullptr;
        }
        while (curr->value != value) {
            if (value < curr->value) {
                curr = curr->left;
            } else {
                curr = curr->right;
            }
            if (curr == nullptr) {
                return nullptr;
            }
        }
        return &(curr->value);
    }

    int* traversal() {
        int* mass = new int[size_ + 1];
        int i = 0;
        preOrder(root_, mass, i);
        return mass;
    }

    int* lowerBound(int value) {
        bool find = false;
        int* ret = nullptr;
        return preOrderNode(root_, value, find, ret);
    }

    bool empty() {
        return size_ == 0;
    }

    Node* getRoot() {
        return root_;
    }

    int getSize() {
        return size_;
    }

    ~AVLTree() {
        if (root_) {
            delet(root_);
        }
    };

private:
    int balancefactor(Node* p) {
        return height(p->right) - height(p->left);
    }

    int height(Node* rt) {
        if (rt == nullptr) {
            return 0;
        }
        return rt->height;
    }

    Node* rotateright(Node* p) {
        Node* q = p->left;
        p->left = q->right;
        q->right = p;
        fixheight(p);
        fixheight(q);
        return q;
    }

    Node* rotateleft(Node* q) {
        Node* p = q->right;
        q->right = p->left;
        p->left = q;
        fixheight(q);
        fixheight(p);
        return p;
    }
    void fixheight(Node* rt) {
        rt->height = std::max(height(rt->left), height(rt->right)) + 1;
    }

    Node* balance(Node* p) {
        fixheight(p);
        if (balancefactor(p) > 1) {
            if (balancefactor(p->right) == -1) {
                p->right = rotateright(p->right);
            }
            return rotateleft(p);
        }
        if (balancefactor(p) < -1) {
            if (balancefactor(p->left) == 1) {
                p->left = rotateleft(p->left);
            }
            return rotateright(p);
        }
        return p;
    }

    void delet(Node* node) {
        if (node) {
            delet(node->left);
            delet(node->right);
            delete node;
        }
    }
    Node* findmin(Node* p) {
        return p->left ? findmin(p->left) : p;
    }
    Node* removemin(Node* p) {
        if (p->left == nullptr) {
            return p->right;
        }
        p->left = removemin(p->left);
        return balance(p);
    }
    Node* remove(Node* p, int k) {
        if (!p) {
            return nullptr;
        }
        if (k < p->value) {
            p->left = remove(p->left, k);
        } else if (k > p->value) {
            p->right = remove(p->right, k);
        } else {
            Node* q = p->left;
            Node* r = p->right;
            size_--;
            delete p;
            if (!r) {
                return q;
            }
            Node* min = findmin(r);
            min->right = removemin(r);
            min->left = q;
            return balance(min);
        }
        return balance(p);
    }
    Node* insert(Node* rt, int key) {
        if (rt == nullptr) {
            Node* nr = new Node(key);
            size_++;
            return nr;
        }
        if (key < rt->value) {
            rt->left = insert(rt->left, key);
        } else if (key > rt->value) {
            rt->right = insert(rt->right, key);
        }
        return balance(rt);
    }

    void preOrder(Node* root, int* mass, int& i) {
        if (root != nullptr) {
            preOrder(root->left, mass, i);
            mass[i] = root->value;
            i++;
            preOrder(root->right, mass, i);
        }
    }
    int* preOrderNode(Node* root, int searchval, bool& find, int*& ret) {
        if (root != nullptr) {
            preOrderNode(root->left, searchval, find, ret);
            if (root->value >= searchval && !find) {
                find = true;
                ret = this->find(root->value);
            }
            preOrderNode(root->right, searchval, find, ret);
        }
        return ret;
    }
    Node* root_;
    int size_;
};
