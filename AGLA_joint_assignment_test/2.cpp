#include <iostream>
#include <vector>

using namespace std;

int main(void)
{
    int n;
    cin >> n;
    vector<int> vec(n);
    for (int i = 0; i < n; i++)
        cin >> vec[i];
    for (int i = n - 1; i >= 0; i--)
        cout << vec[i] << ' ';
    return 0;
}