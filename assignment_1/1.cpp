#include <iostream>
#include <fstream>

using namespace std;

int main()
{
    ifstream inp("input.txt");
    ofstream out("output.txt");

    long long a, b;
    char op;
    string line;

    getline(inp, line);
    int index_first_del = line.find(",");
    int index_last_del = line.find_last_of(",");
    a = stoll(line.substr(0, index_first_del));
    b = stoll(line.substr(index_first_del + 1, index_last_del - index_first_del - 1));
    op = line.at(index_last_del + 1);

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
        out << line << endl;
        break;
    }
    inp.close();
    out.close();

    return 0;
}
