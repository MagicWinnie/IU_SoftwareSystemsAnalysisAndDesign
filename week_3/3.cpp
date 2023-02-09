#include <iostream>
#include <cmath>

using namespace std;

class Volume
{
public:
    double volume(double a, double b, double c)
    {
        return a * b * c;
    }
    double volume(double r, double h)
    {
        return M_PI * r * r * h;
    }
    double volume(double r)
    {
        return 4.0 / 3.0 * M_PI * r * r * r;
    }
};

int main()
{
    Volume vol;
    cout << vol.volume(1, 2, 3) << endl;
    cout << vol.volume(1, 2) << endl;
    cout << vol.volume(1) << endl;
    return 0;
}