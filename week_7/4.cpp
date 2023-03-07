#include <iostream>

using namespace std;

template <typename T>
class Stack
{
private:
    int _size;
    struct list
    {
        T val;
        struct list *next = nullptr;
    };
    struct list *head;

public:
    Stack()
    {
        _size = 0;
        head = nullptr;
    }
    ~Stack()
    {
        struct list *curr = head;
        while (head != nullptr)
        {
            head = head->next;
            delete[] curr;
            curr = head;
        }
        delete[] curr;
    }
    int size()
    {
        return _size;
    }
    bool empty()
    {
        return _size == 0;
    }
    void push(T e)
    {
        struct list *temp = new struct list();
        if (_size == 0)
        {
            temp->val = e;
            temp->next = nullptr;
            head = temp;
            _size++;
            return;
        }
        temp->val = e;
        temp->next = head;
        head = temp;
        _size++;
    }
    T pop()
    {
        struct list *temp = head->next;
        T val = head->val;
        head = temp;
        _size--;
        return val;
    }
    T peek()
    {
        return head->val;
    }
    friend ostream &operator<<(std::ostream &os, Stack const &s)
    {
        struct list *temp = s.head;
        while (temp != nullptr)
        {
            os << temp->val << " ";
            temp = temp->next;
        }
        return os;
    }
};

int main(void)
{
    Stack<bool> stack;
    stack.push(true);
    stack.push(false);
    stack.push(true);
    stack.push(true);
    stack.push(false);
    cout << stack << endl;
    return 0;
}