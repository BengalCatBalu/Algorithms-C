#include <iterator>
#include <initializer_list>
#include <stdexcept>

class Vector {
public:
    struct Iterator {
        using IteratorCategory = std::random_access_iterator_tag;
        using DifferenceType = std::ptrdiff_t;

        explicit Iterator(int* ptr) : m_ptr_(ptr){};
        Iterator(const Iterator& rhs) : m_ptr_(rhs.m_ptr_) {
        }

        int& operator*() const {
            return *m_ptr_;
        };
        int* operator->() {
            return m_ptr_;
        };

        Iterator& operator++() {
            m_ptr_++;
            return *this;
        }

        Iterator operator++(int) {
            auto help = m_ptr_;
            m_ptr_++;
            return Iterator(help);
        }

        Iterator& operator--() {
            m_ptr_--;
            return *this;
        }

        Iterator operator--(int) {
            auto help = m_ptr_;
            m_ptr_--;
            return Iterator(help);
        }

        Iterator operator+(const DifferenceType& movement) {
            return Iterator(m_ptr_ + movement);
        };
        Iterator operator-(const DifferenceType& movement) {
            return Iterator(m_ptr_ - movement);
        };

        Iterator& operator+=(const DifferenceType& movement) {
            *this = *this + movement;
            return *this;
        };
        Iterator& operator-=(const DifferenceType& movement) {
            *this = *this - movement;
            return *this;
        };

        friend bool operator==(const Iterator& a, const Iterator& b) {
            return a.m_ptr_ == b.m_ptr_;
        };
        friend bool operator!=(const Iterator& a, const Iterator& b) {
            return !(a == b);
        };
        bool operator>(const Iterator& rhs) const {
            return m_ptr_ > rhs.m_ptr_;
        }
        bool operator<(const Iterator& rhs) const {
            return m_ptr_ < rhs.m_ptr_;
        }
        bool operator>=(const Iterator& rhs) const {
            return m_ptr_ >= rhs.m_ptr_;
        }
        bool operator<=(const Iterator& rhs) const {
            return m_ptr_ <= rhs.m_ptr_;
        }

    private:
        int* m_ptr_;
    };

    Vector() : size_(0), capacity_(10), arr_(new int[capacity_]){};

    explicit Vector(size_t n_size)
        : size_(n_size), capacity_(2 * n_size), arr_(new int[capacity_]){};

    Vector(const int* vals, size_t n_size)
        : size_(n_size), capacity_(2 * n_size), arr_(new int[capacity_]) {
        for (size_t i = 0; i < size_; i++) {
            arr_[i] = vals[i];
        }
    };

    Vector(const Vector& vec)
        : size_(vec.size_), capacity_(vec.capacity_), arr_(new int[capacity_]) {
        for (size_t i = 0; i < size_; i++) {
            arr_[i] = vec.arr_[i];
        }
    };

    Vector(std::initializer_list<int> vals)
        : size_(vals.size()), capacity_(vals.size() * 2), arr_(new int[capacity_]) {
        for (size_t i = 0; i < size_; i++) {
            arr_[i] = *(vals.begin() + i);
        }
    };

    ~Vector() {
        delete[] arr_;
    };

    size_t getSize() const {
        return size_;
    };

    size_t getCapacity() const {
        return capacity_;
    };

    bool isEmpty() const {
        return size_ == 0;
    };

    void reserve(size_t n) {
        if (n <= capacity_) {
            return;
        }
        int* newarr = new int[n];
        for (size_t i = 0; i < size_; i++) {
            newarr[i] = arr_[i];
        }
        delete[] arr_;
        arr_ = newarr;
        capacity_ = n;
    }
    void resize(size_t n_size) {
        if (n_size >= capacity_) {
            reserve(2 * n_size);
        }
        if (n_size > size_) {
            for (auto i = size_; i < n_size; i++) {
                pushBack(0);
            }
        }
        if (n_size < size_) {
            while (size_ != n_size) {
                popBack();
            }
        }
    };

    void pushBack(int value) {
        if (capacity_ == size_) {
            reserve(2 * size_);
        }
        arr_[size_] = value;
        ++size_;
    };

    void popBack() {
        if (size_ == 0) {
            throw std::runtime_error("Empty Array!");
        }
        --size_;
        arr_[size_] = 0;
    };

    void clear() {
        size_ = 0;
        delete[] arr_;
        arr_ = new int[capacity_];
    };

    void insert(size_t pos, int value) {
        if (pos > size_) {
            throw std::runtime_error("Wrong Position!");
        }
        if (size_ == capacity_) {
            reserve(2 * size_);
        }
        int* newarr = new int[capacity_];
        for (size_t i = 0; i != pos; i++) {
            newarr[i] = arr_[i];
        }
        newarr[pos] = value;
        ++size_;
        for (auto i = pos + 1; i != size_; i++) {
            newarr[i] = arr_[i - 1];
        }
        delete[] arr_;
        arr_ = newarr;
    };

    void erase(size_t pos) {
        if (isEmpty()) {
            throw std::runtime_error("Empty Array!");
        }
        if (pos >= size_) {
            throw std::runtime_error("Wrong Position!");
        }
        for (auto i = pos; i != size_; ++i) {
            arr_[i] = arr_[i + 1];
        }
        popBack();
    };

    int at(size_t pos) {
        if (pos >= size_) {
            throw std::runtime_error("Wrong Position!");
        }
        return arr_[pos];
    };

    int front() {
        if (isEmpty()) {
            throw std::runtime_error("Empty Array!");
        }
        return arr_[0];
    };

    int back() {
        if (isEmpty()) {
            throw std::runtime_error("Empty Array!");
        }
        return arr_[size_ - 1];
    };

    Iterator begin() {
        return Iterator(&arr_[0]);
    };

    Iterator end() {
        return Iterator(&arr_[size_]);
    };

    int& operator[](size_t pos) const {
        if (pos >= size_) {
            throw std::runtime_error("Wrong Position!");
        }
        return arr_[pos];
    };

    Vector& operator=(const Vector& other) {
        resize(other.size_);
        reserve(other.capacity_);
        for (size_t i = 0; i < other.size_; i++) {
            arr_[i] = other.arr_[i];
        }
        return *this;
    };

private:
    size_t size_;
    size_t capacity_;
    int* arr_ = nullptr;
};

Vector merge(const Vector::Iterator begin, Vector::Iterator mid1, const Vector::Iterator end) {
    Vector v;
    Vector::Iterator it_l{begin}, it_r{mid1};
    Vector::Iterator it_mid{mid1}, it_end{end};
    while (it_l != it_mid && it_r != it_end) {
        if (*it_l < *it_r) {
            v.pushBack(*it_l);
            ++it_l;
        } else {
            v.pushBack(*it_r);
            ++it_r;
        }
    }
    while (it_l != it_mid) {
        v.pushBack(*it_l);
        ++it_l;
    }
    while (it_r != it_end) {
        v.pushBack(*it_r);
        ++it_r;
    }
    return v;
}

void mergeSort(Vector::Iterator begin, Vector::Iterator end) {
    auto i = begin;
    auto j = end;
    int32_t k = 0;
    while (i != j) {
        ++i;
        ++k;
    }
    if (k <= 1) {
        return;
    }
    Vector::Iterator middle = begin;
    for (int32_t a = 0; a < k / 2; a++) {
        ++middle;
    }
    mergeSort(begin, middle);
    mergeSort(middle, end);
    auto&& v = merge(begin, middle, end);
    for (auto i = v.begin(); i != v.end(); ++i) {
        *begin = *i;
        ++begin;
    }
}
void insertionSort(Vector::Iterator first, Vector::Iterator last) {
    if (!(first < last)) {
        return;
    }
    for (Vector::Iterator i = first + 1; i != last; ++i) {
        for (Vector::Iterator j = i; j != first && *j < *(j - 1); --j) {
            std::iter_swap(j - 1, j);
        }
    }
}
