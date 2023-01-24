#include <iostream>

using namespace std;

int main()
{
    double x = -3, y;
    cout << "x\ty" << endl;
    while (x <= 3)
    {
        y = -3 * x * x - 4 * x - 6.5;
        cout << x << '\t' << y << endl;
        x += 0.5;
    }
    return 0;
}
