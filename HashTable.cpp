#include <iostream>
#include <stdexcept>
#include <vector>

// linked list
template <class KeyType, class ValueType>
struct Node {
    KeyType key;
    ValueType value;
    Node* next;
    Node(KeyType key, ValueType value) : key(key), value(value) {
        next = nullptr;
    };
};

template <class KeyType, class ValueType, class Func = std::hash<KeyType>>
class HashTable {
public:
    HashTable() {
        arr_ = std::vector<Node<KeyType, ValueType>*>(cap_, nullptr);
    };
    explicit HashTable(Func func) {
        arr_ = std::vector<Node<KeyType, ValueType>*>(cap_, nullptr);
        hf_ = func;
    };
    HashTable(size_t max_size, double coeff, Func func = std::hash<KeyType>()) {
        if (coeff > 1 || coeff <= 0) {
            koef_ = 0.5;
        } else {
            koef_ = coeff;
        }
        hf_ = func;
        cap_ = max_size;
        arr_ = std::vector<Node<KeyType, ValueType>*>(cap_, nullptr);
    };

    ~HashTable() {
        for (auto x : arr_) {
            delet(x);
        }
    };

    void insert(KeyType key, ValueType data) {
        int64_t bucketindex = hf_(key) % cap_;
        if (arr_[bucketindex] == nullptr) {
            arr_[bucketindex] = new Node<KeyType, ValueType>(key, data);
            numofel_++;
        } else {
            Node<KeyType, ValueType>* kt = arr_[bucketindex];
            bool fl = false;
            while (kt->next != nullptr) {
                if (kt->key == key) {
                    kt->value = data;
                    fl = true;
                }
                kt = kt->next;
            }
            if (!fl && kt->key == key) {
                kt->value = data;
                fl = true;
            }
            if (!fl) {
                kt->next = new Node<KeyType, ValueType>(key, data);
                numofel_++;
            }
        }
        while (this->getLoadFactor() > koef_) {
            this->rehashing();
        }
    };
    void erase(KeyType key) {
        int64_t bucketindex = hf_(key) % cap_;
        auto k = arr_[bucketindex];
        if (!k) {
            return;
        }
        if (k->key == key) {
            arr_[bucketindex] = k->next;
            numofel_--;
            delete k;
        } else {
            while (k->next) {
                if (k->next->key == key) {
                    auto del = k->next->next;
                    numofel_--;
                    delete k->next;
                    k->next = del;
                    break;
                }
                k = k->next;
            }
        }
    };

    ValueType* find(KeyType key) {
        auto bucketindex = hf_(key) % cap_;
        auto kt = arr_[bucketindex];
        if (!kt) {
            return nullptr;
        }
        while (kt) {
            if (kt->key == key) {
                return &kt->value;
            }
            kt = kt->next;
        }
        return nullptr;
    };

    Node<KeyType, ValueType>& operator[](uint64_t index) const {
        if (index > cap_ - 1 || cap_ < 0) {
            throw std::out_of_range("");
        }
        if (arr_[index] == nullptr) {
            throw std::runtime_error("");
        }
        return *arr_[index];
    };
    Node<KeyType, ValueType> at(uint64_t index) const {
        if (index > cap_ - 1 || cap_ < 0) {
            throw std::out_of_range("");
        }
        if (arr_[index] == nullptr) {
            throw std::runtime_error("");
        }
        return *arr_[index];
    };

    size_t size() const {
        return numofel_;
    };
    size_t capacity() const {
        return cap_;
    };

private:
    double getLoadFactor() {
        return static_cast<double>(numofel_) / static_cast<double>(cap_);
    }

    void delet(Node<KeyType, ValueType>* a) {
        if (a) {
            delet(a->next);
            numofel_--;
            delete a;
        }
    }
    void deletf(Node<KeyType, ValueType>* a) {
        if (a) {
            deletf(a->next);
            delete a;
        }
    }
    void rehashing() {
        auto oldcapacity = cap_;
        std::vector<Node<KeyType, ValueType>*> temp = this->arr_;

        cap_ = oldcapacity * 2;
        this->arr_ = std::vector<Node<KeyType, ValueType>*>(this->cap_, nullptr);
        numofel_ = 0;
        for (int64_t i = 0; i < oldcapacity; ++i) {
            Node<KeyType, ValueType>* currbuckethead = temp[i];
            while (currbuckethead != nullptr) {
                this->insert(currbuckethead->key, currbuckethead->value);
                currbuckethead = currbuckethead->next;
            }
        }
        for (auto x : temp) {
            deletf(x);
        }
    }
    uint64_t numofel_ = 0;
    uint64_t cap_ = 100;
    std::vector<Node<KeyType, ValueType>*> arr_;
    double koef_ = 0.5;
    Func hf_ = Func();
};
