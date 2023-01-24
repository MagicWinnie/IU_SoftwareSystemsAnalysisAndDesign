#include <iostream>

using namespace std;

int main()
{
    const int size = 10;
    int arr[size];
    for (int i = 0; i < size; i++)
        arr[i] = i + 1;
    for (int i = 0; i < size; i++)
        arr[i] *= arr[i];
    for (int i = 0; i < size; i++)
        cout << arr[i] << ' ';
    cout << endl;
    return 0;
}