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
    Product(int w, int h, int p, string c) : width(w), height(h), price(p), color(c) {};
    virtual void printType() = 0;
    virtual void printSize() = 0;
    virtual int getPrice() = 0;
};

class TShirt : public Product
{
public:
    TShirt(int w, int h, int p, string c) : Product(w, h, p, c) {};
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
    Pants(int w, int h, int p, string c) : Product(w, h, p, c) {};
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
    Sweater(int w, int h, int p, string c) : Product(w, h, p, c) {};
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
    TShirt tshirt(10, 11, 12, "red");
    Pants pants(20, 21, 22, "green");
    Sweater sweater(30, 31, 32, "blue");

    tshirt.printType();
    tshirt.printSize();
    cout << tshirt.getPrice() << endl;

    pants.printType();
    pants.printSize();
    cout << pants.getPrice() << endl;

    sweater.printType();
    sweater.printSize();
    cout << sweater.getPrice() << endl;

    return 0;
}