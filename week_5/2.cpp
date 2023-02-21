#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Product
{
protected:
    int width, height, price;
    string color;

public:
    Product(int w, int h, int p, string c) : width(w), height(h), price(p), color(c){};
    virtual void printType()
    {
        cout << "I am " << color << " product" << endl;
    }
    virtual void printSize()
    {
        cout << "I am product with sizes: " << width << " " << height << endl;
    }
    virtual int getPrice()
    {
        return price;
    }
};

class TShirt : public Product
{
public:
    TShirt(int w, int h, int p, string c) : Product(w, h, p, c){};
    void printType() override
    {
        cout << "I am " << color << " t-shirt" << endl;
    }
    void printSize() override
    {
        cout << "I am t-shirt with sizes: " << width << " " << height << endl;
    }
    int getPrice() override
    {
        return price;
    }
};

class Pants : public Product
{
public:
    Pants(int w, int h, int p, string c) : Product(w, h, p, c){};
    void printType() override
    {
        cout << "I am " << color << " pants" << endl;
    }
    void printSize() override
    {
        cout << "I am pants with sizes: " << width << " " << height << endl;
    }
    int getPrice() override
    {
        return price;
    }
};

class Sweater : public Product
{
public:
    Sweater(int w, int h, int p, string c) : Product(w, h, p, c){};
    void printType() override
    {
        cout << "I am " << color << " sweater" << endl;
    }
    void printSize() override
    {
        cout << "I am sweater with sizes: " << width << " " << height << endl;
    }
    int getPrice() override
    {
        return price;
    }
};

int main(void)
{
    vector<Product *> cart;

    cart.push_back(new TShirt(10, 11, 12, "red"));
    cart.push_back(new Pants(20, 21, 22, "green"));
    cart.push_back(new Sweater(30, 31, 32, "blue"));

    int price = 0;
    for (auto el : cart)
    {
        el->printType();
        price += el->getPrice();
    }

    cout << price << endl;

    return 0;
}