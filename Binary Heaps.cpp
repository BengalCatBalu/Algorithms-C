#include <iostream>
#include <vector>

template <class ValueType>
class HeapMax {
public:
    HeapMax() {
        heap_ = {};
    };

    template <class Iterator>
    HeapMax(Iterator begin, Iterator end) {
        heap_ = {};
        while (begin != end) {
            insert(*begin);
            begin++;
        }
    };

    HeapMax(std::initializer_list<ValueType> t) {
        heap_ = {};
        for (auto x : t) {
            insert(x);
        }
    };

    HeapMax(const HeapMax &t) {
        heap_ = {};
        for (auto x : t.heap_) {
            insert(x);
        }
    }
    HeapMax &operator=(const HeapMax &t) {
        heap_ = {};
        for (auto x : t.heap_) {
            insert(x);
        }
        return *this;
    };

    HeapMax(HeapMax &&t) {
        heap_ = t.heap_;
        t.heap_ = {};
    };
    HeapMax &operator=(HeapMax &&other) {
        if (*this != &other) {
            heap_ = {};
            heap_ = other.heap_;
            other.heap_ = {};
        }
        return *this;
    };

    ~HeapMax() {
        heap_ = {};
    };

    size_t size() const {
        return heap_.size();
    };
    bool empty() const {
        return heap_.empty();
    };

    void insert(const ValueType &t) {
        heap_.push_back(t);
        siftUp(size() - 1);
    };
    ValueType extract() {
        auto max = heap_[0];
        heap_[0] = heap_[size() - 1];
        heap_.erase(heap_.end() - 1);
        siftDown(0);
        return max;
    };
    

private:
    void swap(uint32_t i, uint32_t j) {
        auto t = heap_[i];
        heap_[i] = heap_[j];
        heap_[j] = t;
    }
    void siftDown(uint32_t i) {
        while (2 * i + 1 < size()) {
            auto left = 2 * i + 1;
            auto right = 2 * i + 2;
            auto j = left;
            if (right < size() && heap_[left] < heap_[right]) {
                j = right;
            }
            if (!(heap_[i] < heap_[j])) {
                break;
            }
            swap(i, j);
            i = j;
        }
    }

    void siftUp(uint32_t i) {
        if (size() < 2) {
            return;
        }
        while (i > 0 && heap_[(i - 1) / 2] < heap_[i]) {
            swap(i, (i - 1) / 2);
            i = (i - 1) / 2;
        }
    }

    std::vector<ValueType> heap_;
};

template <class ValueType>
class HeapMin {
public:
    HeapMin() {
        heap_ = {};
    };

    template <class Iterator>
    HeapMin(Iterator begin, Iterator end) {
        heap_ = {};
        while (begin != end) {
            insert(*begin);
            begin++;
        }
    };

    HeapMin(std::initializer_list<ValueType> t) {
        heap_ = {};
        for (auto x : t) {
            insert(x);
        }
    };

    HeapMin(const HeapMin &t) {
        heap_ = {};
        for (auto x : t.heap_) {
            insert(x);
        }
    }
    HeapMin &operator=(const HeapMin &t) {
        heap_ = {};
        for (auto x : t.heap_) {
            insert(x);
        }
        return *this;
    };

    HeapMin(HeapMin &&t) {
        heap_ = t.heap_;
        t.heap_ = {};
    };
    HeapMin &operator=(HeapMin &&other) {
        if (*this != &other) {
            heap_ = {};
            heap_ = other.heap_;
            other.heap_ = {};
        }
        return *this;
    };

    ~HeapMin() {
        heap_ = {};
    };

    size_t size() const {
        return heap_.size();
    };
    bool empty() const {
        return heap_.empty();
    };

    void insert(const ValueType &t) {
        heap_.push_back(t);
        siftUp(size() - 1);
    };
    ValueType extract() {
        auto max = heap_[0];
        heap_[0] = heap_[size() - 1];
        heap_.erase(heap_.end() - 1);
        siftDown(0);
        return max;
    };

private:
    void swap(uint32_t i, uint32_t j) {
        auto t = heap_[i];
        heap_[i] = heap_[j];
        heap_[j] = t;
    }
    void siftDown(uint32_t i) {
        while (2 * i + 1 < size()) {
            auto left = 2 * i + 1;
            auto right = 2 * i + 2;
            auto j = left;
            if (right < size() && heap_[right] < heap_[left]) {
                j = right;
            }
            if (heap_[j] >= heap_[i]) {
                break;
            }
            swap(i, j);
            i = j;
        }
    }

    void siftUp(uint32_t i) {
        if (size() < 2) {
            return;
        }
        while (i > 0 && heap_[(i - 1) / 2] > heap_[i]) {
            swap(i, (i - 1) / 2);
            i = (i - 1) / 2;
        }
    }

    std::vector<ValueType> heap_;
};
