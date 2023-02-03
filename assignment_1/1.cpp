#include <iostream>
#include <fstream>

using namespace std;

int main()
{
    ifstream inp("input.txt");
    ofstream out("output.txt");

    long long a, b;
    char op;
    string tmp;
    
    getline(inp, tmp, ',');
    a = stoll(tmp);
    getline(inp, tmp, ',');
    b = stoll(tmp);
    getline(inp, tmp, ',');
    op = tmp.at(0);
    
    out << fixed;
    switch (op)
    {
    case '-':
        out << a - b << endl;
        break;
    case '+':
        out << a + b << endl;
        break;
    case '/':
        if (b == 0)
            out << "NAN" << endl;
        else
            out << a / b << endl;
        break;
    case '*':
        out << a * b << endl;
        break;

    default:
        out << "not implemented" << endl;
        break;
    }
    inp.close();
    out.close();

    return 0;
}
