#include <initializer_list>
#include <utility>
#include <iostream>
enum class Color { RED, BLACK };

template <typename T>
struct Node {
    T key;
    Node* left;
    Node* right;
    Node* parent;
    Color color;
    Node(T value) : key(value), left(nullptr), right(nullptr), parent(nullptr), color(Color::RED){};
};

template <typename T>
class RBTree {
public:
    RBTree() : root(nullptr), size_(0){};

    RBTree(std::initializer_list<T> list) : root(nullptr), size_(0) {
        for (auto x : list) {
            this->insert(x);
        }
    }

    ~RBTree() {
        if (root) {
            delet(root);
        }
    };

    void insert(T key) {
        if (this->find(key) != nullptr) {
            return;
        }
        auto* pt = new Node(key);
        // Do a normal BST insert
        root = insertion(root, pt);
        // fix Red Black Tree violations
        fixViolation(root, pt);
    };

    int size() const {
        return size_;
    };

    bool empty() const {
        return size_ == 0;
    };

    T* lowerBound(T value) {
        bool find = false;
        T* ret = nullptr;
        preOrderNode(root, value, find, ret);
        return ret;
    }
    T* find(T key) {
        return find(root, key);
    }
    T* find(Node<T>* rt,T key) const {
        if (rt == nullptr) {
            return nullptr;
        }
        if (key == rt->key) {
            return &(rt->key);
        }
        if (rt->key < key) {
            return find(rt->right, key);
        }
        return find(rt->left, key);
    };

    T* preOrderNode(Node<T>*& root, T searchval, bool& find, T*& ret) const {
        if (root != nullptr) {
            preOrderNode(root->left, searchval, find, ret);
            if (root->key >= searchval && !find) {
                find = true;
                ret = this->find(root->key);
            }
            preOrderNode(root->right, searchval, find, ret);
        }
        return ret;
    }

    Node<T>* insertion(Node<T>* rt, Node<T>* pt) {
        if (rt == nullptr) {
            size_++;
            return pt;
        }
        if (pt->key < rt->key) {
            rt->left = insertion(rt->left, pt);
            rt->left->parent = rt;
        } else if (pt->key > rt->key) {
            rt->right = insertion(rt->right, pt);
            rt->right->parent = rt;
        } else {
            return root;
        }
        return rt;
    }

    void rotateLeft(Node<T>*& root, Node<T>*& pt) {
        Node<T>* pt_right = pt->right;
        pt->right = pt_right->left;
        if (pt->right != nullptr) {
            pt->right->parent = pt;
        }
        pt_right->parent = pt->parent;
        if (pt->parent == nullptr) {
            root = pt_right;
        } else if (pt == pt->parent->left) {
            pt->parent->left = pt_right;
        } else {
            pt->parent->right = pt_right;
        }

        pt_right->left = pt;
        pt->parent = pt_right;
    }

    void rotateRight(Node<T>*& root, Node<T>*& pt) {
        Node<T>* pt_left = pt->left;
        pt->left = pt_left->right;
        if (pt->left != nullptr) {
            pt->left->parent = pt;
        }
        pt_left->parent = pt->parent;
        if (pt->parent == nullptr) {
            root = pt_left;
        } else if (pt == pt->parent->left) {
            pt->parent->left = pt_left;
        } else {
            pt->parent->right = pt_left;
        }
        pt_left->right = pt;
        pt->parent = pt_left;
    }

    void fixViolation(Node<T>*& root, Node<T>*& pt) {
        Node<T>* parent_pt = nullptr;
        Node<T>* grand_parent_pt = nullptr;
        // Если хотя бы одно верно (Мы дошли до корня, цвет не черный, цвет родителя не красный)
        while ((pt != root) && (pt->color != Color::BLACK) && (pt->parent->color == Color::RED)) {
            parent_pt = pt->parent;
            grand_parent_pt = pt->parent->parent;
            if (parent_pt == grand_parent_pt->left) {
                Node<T>* uncle_pt = grand_parent_pt->right;
                if (uncle_pt != nullptr && uncle_pt->color == Color::RED) {
                    grand_parent_pt->color = Color::RED;
                    parent_pt->color = Color::BLACK;
                    uncle_pt->color = Color::BLACK;
                    pt = grand_parent_pt;
                } else {
                    if (pt == parent_pt->right) {
                        rotateLeft(root, parent_pt);
                        pt = parent_pt;
                        parent_pt = pt->parent;
                    }
                    rotateRight(root, grand_parent_pt);
                    std::swap(parent_pt->color, grand_parent_pt->color);
                    pt = parent_pt;
                }
            } else {
                Node<T>* uncle_pt = grand_parent_pt->left;
                if ((uncle_pt != nullptr) && (uncle_pt->color == Color::RED)) {
                    grand_parent_pt->color = Color::RED;
                    parent_pt->color = Color::BLACK;
                    uncle_pt->color = Color::BLACK;
                    pt = grand_parent_pt;
                } else {
                    if (pt == parent_pt->left) {
                        rotateRight(root, parent_pt);
                        pt = parent_pt;
                        parent_pt = pt->parent;
                    }
                    rotateLeft(root, grand_parent_pt);
                    std::swap(parent_pt->color, grand_parent_pt->color);
                    pt = parent_pt;
                }
            }
        }

        root->color = Color::BLACK;
    }

    void delet(Node<T>* node) {
        if (node) {
            delet(node->left);
            delet(node->right);
            delete node;
        }
    }
    Node<T>* root;
    int size_;
};

int main()
{
    RBTree<int> tree;
    std::cout << tree.empty() << '\n';
    tree.insert(7);
    std::cout << tree.empty() << '\n';
    tree.insert(6);
    tree.insert(6);
    //std::cout << tree.size() << '\n';
    tree.insert(5);
    tree.insert(4);
    tree.insert(3);
    tree.insert(2);
    tree.insert(1);
    return 0;
}
