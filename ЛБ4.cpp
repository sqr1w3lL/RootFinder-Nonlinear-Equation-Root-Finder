#include <iostream>
#include <cmath>
#include <iomanip>
#include <locale>
#include <windows.h>
using namespace std;

double f1(double x) {
    return 2 * x * x * x - 9 * x * x - 60 * x + 1;
}

double df1(double x) {
    return 6 * x * x - 18 * x - 60;
}

double d2f1(double x) {
    return 12 * x - 18;
}

double bisection(double (*f)(double), double a, double b, double eps, double* xs, double* fs, int& n) {
    if (f(a) * f(b) > 0) return NAN;
    double c;
    n = 0;
    while ((b - a) / 2 > eps && n < 1000) {
        c = (a + b) / 2;
        xs[n] = c;
        fs[n] = f(c);
        n++;
        if (f(a) * f(c) < 0) b = c; else a = c;
    }
    return (a + b) / 2;
}

double chord(double (*f)(double), double (*d2f)(double), double a, double b, double eps, double* xs, double* fs, int& n) {
    if (f(a) * f(b) > 0) return NAN;

    double fa = f(a);
    double fb = f(b);
    double d2fa = d2f(a);
    double d2fb = d2f(b);

    double fixed, moving;
    bool fixedIsA;

    if (fa * d2fa > 0) {
        fixed = a;
        moving = b;
        fixedIsA = true;
    }
    else if (fb * d2fb > 0) {
        fixed = b;
        moving = a;
        fixedIsA = false;
    }
    else {
        fixed = a;
        moving = b;
        fixedIsA = true;
    }

    cout << "Нерухомий кінець: x = " << fixed << " ("
        << (fixedIsA ? "a" : "b") << ")\n";

    double c;
    n = 0;
    do {
        c = fixed - f(fixed) * (moving - fixed) / (f(moving) - f(fixed));
        xs[n] = c;
        fs[n] = f(c);
        n++;

        if (f(fixed) * f(c) < 0) {
            moving = c;
        }
        else {
            moving = c;
        }
    } while (fabs(f(c)) > eps && n < 1000);

    return c;
}

double newton(double (*f)(double), double (*df)(double), double x0, double eps, double* xs, double* fs, int& n) {
    double x1;
    n = 0;
    do {
        x1 = x0 - f(x0) / df(x0);
        xs[n] = x1;
        fs[n] = f(x1);
        n++;
        if (fabs(x1 - x0) < eps || n >= 1000) break;
        x0 = x1;
    } while (true);
    return x1;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    setlocale(LC_ALL, "ukr");
    cout << fixed << setprecision(6);
    int again = 1;

    while (again == 1) {
        cout << "Рівняння:\nx * 2^x= 1\n";
        cout << "Оберіть метод:\n1) Половинного розподілу\n2) Хорд\n3) Ньютона–Рафсона\nВибір: ";
        int method;
        cin >> method;

        double a, b, eps = 1e-6, root = NAN;
        cout << "Введіть межі або початкове наближення (a b): ";
        cin >> a >> b;

        double* xs = new double[1000];
        double* fs = new double[1000];
        int n = 0;

        if (method == 1) {
            root = bisection(f1, a, b, eps, xs, fs, n);
        }
        else if (method == 2) {
            root = chord(f1, d2f1, a, b, eps, xs, fs, n);
        }
        else if (method == 3) {
            root = newton(f1, df1, (a + b) / 2, eps, xs, fs, n);
        }

        if (isnan(root)) {
            cout << "Не знайдено кореня на вказаному інтервалі.\n";
        }
        else {
            cout << "\nТаблиця ітерацій:\n";
            cout << setw(8) << "k" << setw(15) << "x" << setw(15) << "f(x)\n";
            for (int i = 0; i < n; i++)
                cout << setw(8) << i + 1 << setw(15) << xs[i] << setw(15) << fs[i] << endl;
            cout << "\nЗнайдений корінь: x = " << root << endl;
        }

        delete[] xs;
        delete[] fs;

        cout << "\n1 — повторити, 0 — вийти: ";
        cin >> again;
    }

    cout << "Роботу завершено.\n";
    return 0;
}