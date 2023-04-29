// Dmitriy Okoneshnikov
// d.okoneshnikov@innopolis.university
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

int main(void)
{
    cout << fixed << setprecision(2);

    double V_0, K_0;
    cin >> V_0 >> K_0;

    double alpha_1, beta_1, alpha_2, beta_2;
    cin >> alpha_1 >> beta_1 >> alpha_2 >> beta_2;

    double T;
    cin >> T;

    int N;
    cin >> N;

    double v_0 = V_0 - alpha_2 / beta_2;
    double k_0 = K_0 - alpha_1 / beta_1;
    double a = sqrt(alpha_1 * alpha_2);

    // Create arrays
    double *T_arr = new double[N + 1];
    double *V_arr = new double[N + 1];
    double *K_arr = new double[N + 1];
    // Calculate
    double t_i = 0;
    for (int i = 0; i <= N; i++)
    {
        T_arr[i] = t_i;

        // Eq. 23-24, p. 312, Matrix methods of approximating classical predator-prey problems, E. Y. Rodin et al.
        V_arr[i] = v_0 * cos(a * t_i) - k_0 * (sqrt(alpha_2) * beta_1 / (beta_2 * sqrt(alpha_1))) * sin(a * t_i) + alpha_2 / beta_2;
        K_arr[i] = k_0 * cos(a * t_i) + v_0 * (sqrt(alpha_1) * beta_2 / (beta_1 * sqrt(alpha_2))) * sin(a * t_i) + alpha_1 / beta_1;

        t_i += T / N;
    }

    // Print arrays
    cout << "t:" << '\n';
    for (int i = 0; i <= N; i++)
        cout << T_arr[i] << ' ';
    cout << '\n'
         << "v:" << '\n';
    for (int i = 0; i <= N; i++)
        cout << V_arr[i] << ' ';
    cout << '\n'
         << "k:" << '\n';
    for (int i = 0; i <= N; i++)
        cout << K_arr[i] << ' ';
    cout << '\n';

    // Delete arrays
    delete[] T_arr;
    delete[] V_arr;
    delete[] K_arr;

    return 0;
}