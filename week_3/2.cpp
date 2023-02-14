#include <iostream>
#include <vector>

using namespace std;

int binary_search(vector<int> vec, int x, int left, int right)
{
    int mid = (left + right) / 2;
    if (left > right)
        return -1;
    if (vec[mid] == x)
        return mid;
    if (vec[mid] > x)
        return binary_search(vec, x, left, mid - 1);
    if (vec[mid] < x)
        return binary_search(vec, x, mid + 1, right);
}

int main()
{
    int n;
    cin >> n;
    vector<int> vec(n);
    for (int i = 0; i < n; i++)
        cin >> vec[i];
    int x;
    cin >> x;
    cout << binary_search(vec, x, 0, n) << endl;
    return 0;
}