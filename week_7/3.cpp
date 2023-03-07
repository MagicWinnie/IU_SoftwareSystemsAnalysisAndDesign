#include <iostream>
#include <vector>

using namespace std;

template <typename T>
T dot(vector<T> a, vector<T> b)
{
    if (a.size() != b.size())
        throw runtime_error("Vector sizes do not match!");
    T res = T();
    for (int i = 0; i < a.size(); i++)
        res += a[i] * b[i];
    return res;
}

int main(void)
{
    vector<int> a_1 = {1, 2, 3};
    vector<int> a_2 = {4, 5, 6};
    cout << dot(a_1, a_2) << endl;

    vector<double> b_1 = {1.1, 2.2, 3.3};
    vector<double> b_2 = {4.4, 5.5, 6.6};
    cout << dot(b_1, b_2) << endl;

    return 0;
}