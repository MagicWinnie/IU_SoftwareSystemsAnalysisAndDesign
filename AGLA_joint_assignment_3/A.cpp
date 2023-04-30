// Dmitriy Okoneshnikov
// d.okoneshnikov@innopolis.university
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#define USE_GNUPLOT 0
#define GNUPLOT_NAME "gnuplot -persist"

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

#if (defined(WIN32) || defined(_WIN32)) && USE_GNUPLOT
    FILE *pipe = _popen(GNUPLOT_NAME, "w");
#elif USE_GNUPLOT
    FILE *pipe = popen(GNUPLOT_NAME, "w");
#endif
#if USE_GNUPLOT
    // Draw v(t), k(t)
    fprintf(pipe, "%s\n", "set terminal png size 1920, 1080 font \"Calibri,24\"");
    fprintf(pipe, "%s\n", "set output 'v_t__k_t.png'");
    fprintf(pipe, "%s\n", "set title \"Predator-Prey Model\"");
    fprintf(pipe, "%s\n", "set key noautotitle");
    fprintf(pipe, "%s\n", "set autoscale xy");
    fprintf(pipe, "%s\n", "set offsets 0.05, 0.05, 0.05, 0.05");
    fprintf(pipe, "%s\n", "set xlabel \"t\"");
    fprintf(pipe, "%s\n", "set ylabel \"v(t), k(t)\"");
    ofstream out("v_t__k_t.txt");
    for (int i = 0; i <= N; i++)
        out << T_arr[i] << ' ' << V_arr[i] << ' ' << K_arr[i] << '\n';
    out.close();
    fprintf(pipe, "%s\n", "plot \"v_t__k_t.txt\" using 1:2 title 'v(t)' w linespoints pointtype 7, \"v_t__k_t.txt\" using 1:3 title 'k(t)' w linespoints pointtype 7");
    fflush(pipe);
    // Draw v(k)
    fprintf(pipe, "%s\n", "set output 'v_k.png'");
    fprintf(pipe, "%s\n", "set xlabel \"k\"");
    fprintf(pipe, "%s\n", "set ylabel \"v\"");
    out.open("v_k.txt");
    for (int i = 0; i <= N; i++)
        out << V_arr[i] << ' ' << K_arr[i] << '\n';
    out.close();
    fprintf(pipe, "%s\n", "plot \"v_k.txt\" using 1:2 title 'v(k)' w linespoints pointtype 7");
    fflush(pipe);
#endif
#if (defined(WIN32) || defined(_WIN32)) && USE_GNUPLOT
    _pclose(pipe);
#elif USE_GNUPLOT
    pclose(pipe);
#endif

    // Delete arrays
    delete[] T_arr;
    delete[] V_arr;
    delete[] K_arr;

    return 0;
}