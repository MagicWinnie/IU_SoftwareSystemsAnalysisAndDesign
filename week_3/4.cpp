#include <iostream>

using namespace std;

struct Complex
{
    double real, imaginary;
};

Complex add(Complex c1, Complex c2)
{
    Complex c3 = {c1.real + c2.real, c1.imaginary + c2.imaginary};
    return c3;
}

Complex sub(Complex c1, Complex c2)
{
    Complex c3 = {c1.real - c2.real, c1.imaginary - c2.imaginary};
    return c3;
}

Complex func(Complex c1, Complex c2, Complex (*f)(Complex, Complex))
{
    return (*f)(c1, c2);
}

int main()
{
    Complex c1 = {1, 2};
    Complex c2 = {1, 3};
    Complex c3 = func(c1, c2, &add);
    cout << c3.real << ' ' << c3.imaginary << 'i' << endl;
    c3 = func(c1, c2, &sub);
    cout << c3.real << ' ' << c3.imaginary << 'i' << endl;

    return 0;
}