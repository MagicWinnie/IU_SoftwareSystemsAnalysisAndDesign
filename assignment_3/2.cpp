// Dmitriy Okoneshnikov
// d.okoneshnikov@innopolis.university
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Class for storing one cell.
class CellInfo
{
private:
    // Value.
    char value;
    // Number of copies.
    int copies;

public:
    // Constructor.
    CellInfo(char value, int copies) : value(value), copies(copies) {}
    // Setter for value.
    void setValue(char value) { this->value = value; }
    // Getter for value.
    char getValue() const { return value; }
    // Setter for number of copies.
    void setCopies(int copies) { this->copies = copies; }
    // Getter for number of copies.
    int getCopies() const { return copies; }
    // == operator overloading.
    bool operator==(const CellInfo &c) const
    {
        return this->value == c.value and this->copies == c.copies;
    }
    // < operator overloading.
    bool operator<(const CellInfo &c) const
    {
        return this->value < c.value;
    }
};

// Class for bag (multiset).
class Bag
{
private:
    // Vector that holds all our cells.
    vector<CellInfo> elements;

public:
    // Helper method to find an element in multiset.
    // Return iterator to end if not found.
    vector<CellInfo>::iterator find(char val)
    {
        for (int i = 0; i < elements.size(); i++)
        {
            if (elements[i].getValue() == val)
                return elements.begin() + i;
        }
        return elements.end();
    }
    // Increase number of copies of a cell or
    // add a cell if it does not exist.
    void insert(char val, int n)
    {
        vector<CellInfo>::iterator it = find(val);
        if (it == elements.end())
            elements.push_back(CellInfo(val, n));
        else
        {
            it->setCopies(it->getCopies() + n);
            if (it->getCopies() > 10000)
                throw runtime_error("too many elements in bag");
        }
    }
    // Decrease number of copies of a cell and
    // remove the cell if its number of copies is negative.
    void remove(char val, int n)
    {
        vector<CellInfo>::iterator it = find(val);
        if (it != elements.end())
        {
            it->setCopies(it->getCopies() - n);
            if (it->getCopies() <= 0)
                elements.erase(it);
        }
    }
    // Get the min value of multiset.
    // Returns \0 if it is empty.
    char min() const
    {
        vector<CellInfo>::const_iterator min_it = min_element(elements.begin(), elements.end());
        if (min_it == elements.end())
            return '\0';
        return min_it->getValue();
    }
    // Get the max value of multiset.
    // Returns \0 if it is empty.
    char max() const
    {
        vector<CellInfo>::const_iterator max_it = max_element(elements.begin(), elements.end());
        if (max_it == elements.end())
            return '\0';
        return max_it->getValue();
    }
    // Check if two bags are equal by comparing all elements.
    bool isEqual(const Bag &b) const
    {
        if (this->elements.size() != b.elements.size())
            return false;
        for (auto cell1 : this->elements)
        {
            bool cell1_equal = false;
            for (auto cell2 : b.elements)
                if (cell1 == cell2)
                {
                    cell1_equal = true;
                    break;
                }
            if (!cell1_equal)
                return false;
        }
        return true;
    }
};

int main(void)
{
    // Read n.
    int n;
    cin >> n;
    // Read and perform all operations.
    Bag bag1, bag2;
    for (int i = 0; i < n; i++)
    {
        char op, val;
        int where, n_copies;
        cin >> op >> where >> val >> n_copies;
        if (op == 'i')
        {
            if (where == 1)
                bag1.insert(val, n_copies);
            else if (where == 2)
                bag2.insert(val, n_copies);
        }
        else if (op == 'r')
        {
            if (where == 1)
                bag1.remove(val, n_copies);
            else if (where == 2)
                bag2.remove(val, n_copies);
        }
    }

    // Print max value of bag 1.
    char bag1_max = bag1.max();
    if (bag1_max == '\0')
        cout << -1 << ' ';
    else
        cout << bag1_max << ' ';

    // Print min value of bag 2.
    char bag2_min = bag2.min();
    if (bag2_min == '\0')
        cout << -1 << ' ';
    else
        cout << bag2_min << ' ';

    // Print if bag 1 == bag 2.
    cout << bag1.isEqual(bag2) << endl;

    return 0;
}