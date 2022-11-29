#include <iostream>

class Node {
public:
    int data{};
    Node* next;
    Node* previous;
    Node() : data(0), next(nullptr), previous(nullptr){};
};

class List {
public:
    List() : size(0), head(nullptr), tail(nullptr){};
    List(int* values, size_t usize) : size(usize){
        if (size == 0) {
            head = tail = nullptr;
            return;
        }
        if (std::abs(values[0]) > 2000000000 || std::abs(values[size - 1]) > 2000000000) {
            throw std::runtime_error("Wrong Value!");
        }
        if (size == 1) {
            Node* newn = new Node();
            newn->data = values[0];
            head = tail = newn;
            head->previous = tail;
            tail->next = head;
            return;
        }
        Node* newn = new Node();
        newn->data = values[0];
        head = newn;
        Node* current_node = head;
        for (size_t i = 1; i < size - 1; ++i) {
            if (std::abs(values[i]) > 2000000000) {
                throw std::runtime_error("Wrong Value!");
            }
            newn = new Node();
            newn->data = values[i];
            std::cout << newn->data << '\n';
            current_node->next = newn;
            newn->previous = current_node;
            current_node = current_node->next;
        }
        newn = new Node();
        newn->data = values[size - 1];
        tail = newn;
        tail->previous = current_node;
        current_node->next = tail;
        if (size != 0) {
            head->previous = tail;
            tail->next = head;
        }
    };
    ~List() {
        if (size == 0) {
            return;
        }
        Node* current_node = head;
        while (size != 0) {
            Node* next_node = current_node->next;
            delete current_node;
            current_node = next_node;
            --size;
        }
    };

    void pushBack(int value) {
        if (std::abs(value) > 2000000000) {
            throw std::runtime_error("Wrong Value!");
        }
        Node* newn = new Node;
        newn->data = value;
        if (size != 0) {
            newn->previous = tail;
            tail->next = newn;
            tail = newn;
        } else {
            head = tail = newn;
        }
        head->previous = tail;
        tail->next = head;
        ++size;
    };
    void pushFront(int value) {
        if (std::abs(value) > 2000000000) {
            throw std::runtime_error("Wrong Value!");
        }
        Node* newn = new Node();
        newn->data = value;
        if (size != 0) {
            head->previous = newn;
            newn->next = head;
            newn->previous = tail;
            head = newn;
        } else {
            head = tail = newn;
            head->previous = tail;
        }
        tail->next = head;
        ++size;
    }
    int pop() {
        if (size == 0) {
            throw std::runtime_error("Can not pop such element!");
        }
        Node* del = head;
        int ret = head->data;
        if (size == 1) {
            head = nullptr;
            tail = nullptr;
        } else {
            head->next->previous = tail;
            tail->next = head->next;
            head = head->next;
        }
        delete del;
        --size;
        return ret;
    };
    int pop(size_t position) {
        if (position > size - 2 || size < 2) {
            throw std::runtime_error("Wrong Position!");
        }
        int ret;
        Node* del = head->next;
        for (size_t i = 0; i < position; i++) {
            del = del->next;
        }
        del->next->previous = del->previous;
        del->previous->next = del->next;
        if (position == size - 2) {
            tail = del->previous;
        }
        tail->next = head;
        head->previous = tail;
        ret = del->data;
        --size;
        delete del;
        return ret;
    };
    void push(int value, size_t position) {
        if (std::abs(value) > 2000000000) {
            throw std::runtime_error("Wrong Value!");
        }
        if (position > size - 1) {
            throw std::runtime_error("Wrong Position!");
        }
        Node* cur = head;
        Node* add = new Node();
        add->data = value;
        for (size_t i = 0; i < position; ++i) {
            cur = cur->next;
        }
        add->previous = cur;
        add->next = cur->next;
        cur->next->previous = add;
        cur->next = add;
        if (position == size - 1) {
            tail = add;
        }
        ++size;
    };
    size_t size = 0;
    Node* head{};
    Node* tail{};
};

int main() {
    int* b = new int[10] {1,2,3,4,5,6,7,8,9,10};
    List a;
    //std::cout<<a.tail->next->data << a.head->previous->data;
    List c(b,1);
    c.push(2,0);
    auto cur = c.tail;
    for(int i = 0; i < c.size; i++) {
        std::cout << cur->data << ' ';
        cur = cur->previous;
    }
}
