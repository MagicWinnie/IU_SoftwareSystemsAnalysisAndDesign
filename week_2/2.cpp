#include <iostream>

using namespace std;

int main()
{
    int i = 10;
    double d = 3.14;
    int &i_reference = i;
    double &d_reference = d;

    cout << i << ' ' << d << '\t' << i_reference << ' ' << d_reference << endl;
    i = 2;
    d = 2.71;
    cout << i << ' ' << d << '\t' << i_reference << ' ' << d_reference << endl;
    i_reference = 4;
    d_reference = 9.81;
    cout << i << ' ' << d << '\t' << i_reference << ' ' << d_reference << endl;

    return 0;
}