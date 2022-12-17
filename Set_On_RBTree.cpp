#include <iostream>
#include <vector>

enum Color { RED, BLACK };

template <typename T>
class Node {
public:
    T value;
    Node<T>* left;
    Node<T>* right;
    Node<T>* parent;
    Color color;
    Node() {
        value = T();
        parent = left = right;
        color = RED;
    }
    explicit Node(const T& x) : value(x) {
        parent = left = right = nullptr;
        color = RED;
    }
};

template <typename ValueType>
class RBTree {
public:
    struct Iterator {
        Iterator() {
            v_ = nullptr;
        };
        explicit Iterator(Node<ValueType>* node) {
            v_ = node;
        };

        const ValueType& operator*() const {
            if (v_) {
                return v_->value;
            } else {
                return ValueType();
            }
        };
        const ValueType* operator->() const {
            if (v_) {
                return &v_->value;
            } else {
                return &ValueType();
            }
        };
        Iterator& operator++() {
            if (v_->right) {
                // std::cout << "1";
                v_ = v_->right;
                while (v_->left) {
                    v_ = v_->left;
                }
                return *this;
            } else if (v_->parent->left == v_) {
                // std::cout << "2";
                v_ = v_->parent;
            } else if (v_->parent->right == v_) {
                // std::cout << "3";
                while (v_->parent && v_->parent->right == v_) {
                    v_ = v_->parent;
                }
                v_ = v_->parent;
            }
            return *this;
        };
        Iterator operator++(int) {
            Iterator tempr;
            tempr.v_ = v_;
            ++(*this);
            return tempr;
        };

        Iterator& operator--() {
            if (v_->left) {
                v_ = v_->left;
                while (v_->right) {
                    v_ = v_->right;
                }
                return *this;
            } else if (v_->parent->right == v_) {
                v_ = v_->parent;
                return *this;
            } else if (v_->parent->left == v_) {
                while (v_->parent && v_->parent->left == v_) {
                    v_ = v_->parent;
                }
                v_ = v_->parent;
            }
            return *this;
        };
        Iterator operator--(int) {
            Iterator* tempr(v_);
            --(*this);
            return *tempr;
        };
        bool operator==(const Iterator& other) const {
            if (other.v_) {
                if (v_) {
                    return (!(v_->value < other.v_->value) && !(other.v_->value < v_->value));
                }
                return false;
            } else {
                return v_ == nullptr;
            }
        };
        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        };

    private:
        Node<ValueType>* v_;
    };
    RBTree() : root(nullptr), sizef_(0){};

    RBTree(std::initializer_list<ValueType> list) : root(nullptr), sizef_(0) {
        for (auto x : list) {
            this->insert(x);
        }
    }

    RBTree(const RBTree& other) {
        root = fill(other.root);
        sizef_ = other.size();
    }
    RBTree<ValueType>& operator=(const RBTree& other) {
        if (root) {
            delet(root);
        }
        root = nullptr;
        root = fill(other.root);
        sizef_ = other.size();
        return *this;
    }
    void insert(const ValueType& n) {
        Node<ValueType>* new_node = new Node(n);
        sizef_++;
        if (root == nullptr) {
            new_node->color = BLACK;
            root = new_node;
        } else {
            Node<ValueType>* temper = search(n);
            if (!(temper->value < n) && !(n < temper->value)) {
                return;
            }
            new_node->parent = temper;
            if (n < temper->value) {
                temper->left = new_node;
            } else {
                temper->right = new_node;
            }
            fixRedRed(new_node);
        }
    }

    void erase(const ValueType& n) {
        if (root == nullptr) {
            return;
        }
        Node<ValueType>* v = search(n);
        if (!(!(v->value < n) && !(n < v->value))) {
            return;
        }
        deleteNode(v);
    }

    ~RBTree() {
        if (root) {
            delet(root);
        }
    }

    Iterator lowerBound(const ValueType& value) const {
        for (auto i = this->begin(); i != this->end(); ++i) {
            if (!(*i < value) && !(value < *i)) {
                return i;
            }
            if (value < *i) {
                return i;
            }
        }
        return (*this).end();
    }
    Iterator find(const ValueType& key) const {
        auto t = find(root, key);
        if (t == nullptr) {
            return end();
        }
        return Iterator(t);
    }

    Node<ValueType>* find(Node<ValueType>* rt, const ValueType key) const {
        if (rt == nullptr) {
            return nullptr;
        }
        if (key == rt->value) {
            return rt;
        }
        if (rt->value < key) {
            return find(rt->right, key);
        }
        return find(rt->left, key);
    };
    Node<ValueType>* findmin(Node<ValueType>* rt) const {
        if (rt->left) {
            findmin(rt->left);
        } else {
            return rt;
        }
    }
    size_t size() const {
        return sizef_;
    };
    bool empty() const {
        return sizef_ == 0;
    };
    Iterator begin() const {
        if (empty()) {
            return end();
        }
        auto p = root;
        while (p->left) {
            p = p->left;
        }
        return Iterator(p);
    }
    Iterator end() const {
        Iterator it(nullptr);
        return it;
    }

    Node<ValueType>* root;

private:
    void leftRotate(Node<ValueType>* x) {
        Node<ValueType>* n_parent = x->right;
        if (x == root) {
            root = n_parent;
        }
        moveDown(x, n_parent);
        x->right = n_parent->left;
        if (n_parent->left != nullptr) {
            n_parent->left->parent = x;
        }
        n_parent->left = x;
    }

    void rightRotate(Node<ValueType>* x) {
        Node<ValueType>* n_parent = x->left;
        if (x == root) {
            root = n_parent;
        }
        moveDown(x, n_parent);
        x->left = n_parent->right;
        if (n_parent->right != nullptr) {
            n_parent->right->parent = x;
        }
        n_parent->right = x;
    }

    void swapColors(Node<ValueType>* x1, Node<ValueType>* x2) {
        Color temper;
        temper = x1->color;
        x1->color = x2->color;
        x2->color = temper;
    }

    void swapValues(Node<ValueType>* u, Node<ValueType>* v) {
        ValueType temper;
        temper = u->value;
        u->value = v->value;
        v->value = temper;
    }

    void fixRedRed(Node<ValueType>* x) {
        if (x == root) {
            x->color = BLACK;
            return;
        }
        Node<ValueType>*parent = x->parent, *grandparent = parent->parent, *uncle = uncled(x);
        if (parent->color != BLACK) {
            if (uncle != nullptr && uncle->color == RED) {
                parent->color = BLACK;
                uncle->color = BLACK;
                grandparent->color = RED;
                fixRedRed(grandparent);
            } else {
                if (isOnLeft(parent)) {
                    if (isOnLeft(x)) {
                        swapColors(parent, grandparent);
                    } else {
                        leftRotate(parent);
                        swapColors(x, grandparent);
                    }
                    rightRotate(grandparent);
                } else {
                    if (isOnLeft(x)) {
                        rightRotate(parent);
                        swapColors(x, grandparent);
                    } else {
                        swapColors(parent, grandparent);
                    }
                    leftRotate(grandparent);
                }
            }
        }
    }

    Node<ValueType>* successor(Node<ValueType>* x) {
        Node<ValueType>* temper = x;
        while (temper->left != nullptr) {
            temper = temper->left;
        }
        return temper;
    }

    Node<ValueType>* bsTreplace(Node<ValueType>* x) {
        if (x->left != nullptr && x->right != nullptr) {
            return successor(x->right);
        }
        if (x->left == nullptr && x->right == nullptr) {
            return nullptr;
        }
        if (x->left != nullptr) {
            return x->left;
        } else {
            return x->right;
        }
    }

    void deleteNode(Node<ValueType>* v) {
        Node<ValueType>* u = bsTreplace(v);
        bool uv_black = ((u == nullptr || u->color == BLACK) && (v->color == BLACK));
        Node<ValueType>* parent = v->parent;

        if (u == nullptr) {
            if (v == root) {
                root = nullptr;
            } else {
                if (uv_black) {
                    fixDoubleBlack(v);
                } else {
                    if (siblingg(v) != nullptr) {
                        siblingg(v)->color = RED;
                    }
                }
                if (isOnLeft(v)) {
                    parent->left = nullptr;
                } else {
                    parent->right = nullptr;
                }
            }
            delete v;
            sizef_--;
            return;
        }

        if (v->left == nullptr || v->right == nullptr) {
            if (v == root) {
                v->value = u->value;
                v->left = v->right = nullptr;
                delete u;
                sizef_--;
            } else {
                if (isOnLeft(v)) {
                    parent->left = u;
                } else {
                    parent->right = u;
                }
                delete v;
                sizef_--;
                u->parent = parent;
                if (uv_black) {
                    fixDoubleBlack(u);
                } else {
                    u->color = BLACK;
                }
            }
            return;
        }
        swapValues(u, v);
        deleteNode(u);
    }

    void fixDoubleBlack(Node<ValueType>* x) {
        if (x == root) {
            return;
        }
        Node<ValueType>*sibling = siblingg(x), *parent = x->parent;
        if (sibling == nullptr) {
            fixDoubleBlack(parent);
        } else {
            if (sibling->color == RED) {
                parent->color = RED;
                sibling->color = BLACK;
                if (isOnLeft(sibling)) {
                    rightRotate(parent);
                } else {
                    leftRotate(parent);
                }
                fixDoubleBlack(x);
            } else {
                if (hasRedChild(sibling)) {
                    if (sibling->left != nullptr && sibling->left->color == RED) {
                        if (isOnLeft(sibling)) {
                            sibling->left->color = sibling->color;
                            sibling->color = parent->color;
                            rightRotate(parent);
                        } else {
                            sibling->left->color = parent->color;
                            rightRotate(sibling);
                            leftRotate(parent);
                        }
                    } else {
                        if (isOnLeft(sibling)) {
                            sibling->right->color = parent->color;
                            leftRotate(sibling);
                            rightRotate(parent);
                        } else {
                            sibling->right->color = sibling->color;
                            sibling->color = parent->color;
                            leftRotate(parent);
                        }
                    }
                    parent->color = BLACK;
                } else {
                    sibling->color = RED;
                    if (parent->color == BLACK) {
                        fixDoubleBlack(parent);
                    } else {
                        parent->color = BLACK;
                    }
                }
            }
        }
    }

    Node<ValueType>* fill(Node<ValueType>* ls) {
        if (ls == nullptr) {
            return nullptr;
        } else {
            Node<ValueType>* lhs = new Node(ls->value);
            lhs->color = ls->color;
            lhs->left = fill(ls->left);
            if (lhs->left) {
                lhs->left->parent = lhs;
            }
            lhs->right = fill(ls->right);
            if (lhs->right) {
                lhs->right->parent = lhs;
            }
            return lhs;
        }
    }
    Node<ValueType>* search(const ValueType& n) {
        Node<ValueType>* temper = root;
        while (temper != nullptr) {
            if (n < temper->value) {
                if (temper->left == nullptr) {
                    break;
                } else {
                    temper = temper->left;
                }
            } else if (!(n < temper->value) && !(temper->value < n)) {
                break;
            } else {
                if (temper->right == nullptr) {
                    break;
                } else {
                    temper = temper->right;
                }
            }
        }

        return temper;
    }

    void delet(Node<ValueType>* node) {
        if (node) {
            delet(node->left);
            delet(node->right);
            delete node;
        }
    }
    Node<ValueType>* uncled(Node<ValueType>* t) {
        if (t->parent == nullptr || t->parent->parent == nullptr) {
            return nullptr;
        }
        if (isOnLeft(t->parent)) {
            return t->parent->parent->right;
        } else {
            return t->parent->parent->left;
        }
    }

    bool isOnLeft(Node<ValueType>* t) {
        return t == t->parent->left;
    }

    Node<ValueType>* siblingg(Node<ValueType>* t) {
        if (t->parent == nullptr) {
            return nullptr;
        }

        if (isOnLeft(t)) {
            return t->parent->right;
        }

        return t->parent->left;
    }

    void moveDown(Node<ValueType>* t, Node<ValueType>* n_parent) {
        if (t->parent != nullptr) {
            if (isOnLeft(t)) {
                t->parent->left = n_parent;
            } else {
                t->parent->right = n_parent;
            }
        }
        n_parent->parent = t->parent;
        t->parent = n_parent;
    }

    bool hasRedChild(Node<ValueType>* t) {
        return (t->left != nullptr && t->left->color == RED) or
               (t->right != nullptr && t->right->color == RED);
    }
    size_t sizef_;
};
