#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <chrono>
#include <windows.h>
using namespace std;
template <typename T>
void printVector(T* vec, int size) {
    for (int i = 0; i < size; i++) {
        cout << setw(8) << vec[i];
    }
    cout << endl;
}

template <typename T>
void printMatrix(T** mat, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            cout << setw(8) << mat[i][j];
        }
        cout << endl;
    }
}
template <typename T>
void runExperiment(const string& typeName, T initVal) {
    cout << "\n--- ТИП ДАНИХ: " << typeName << " ---" << endl;
    cout << setw(7) << "N"
        << setw(24) << "Mat*Vec (мс)"
        << setw(26) << "Vec*Mat (повіл.)"
        << setw(28) << "Vec*Mat (оптиміз.)" << endl;
    cout << string(87, '-') << endl;
    for (int n = 1000; n <= 10000; n += 1000) {
        T* v = new T[n];
        T* r1 = new T[n];
        T* r2 = new T[n];
        T* r3 = new T[n];
        T** m = new T * [n];

        for (int i = 0; i < n; i++) {
            m[i] = new T[n];
            v[i] = initVal;
            r1[i] = 0;
            r2[i] = 0;
            r3[i] = 0;
            for (int j = 0; j < n; j++) {
                m[i][j] = initVal;
            }
        }
        auto s1 = chrono::high_resolution_clock::now();
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                r1[i] += m[i][j] * v[j];
            }
        }
        auto e1 = chrono::high_resolution_clock::now();
        double d1 = chrono::duration<double, milli>(e1 - s1).count();

        auto s2 = chrono::high_resolution_clock::now();
        for (int j = 0; j < n; j++) {
            for (int i = 0; i < n; i++) {
                r2[j] += v[i] * m[i][j];
            }
        }
        auto e2 = chrono::high_resolution_clock::now();
        double d2 = chrono::duration<double, milli>(e2 - s2).count();

        auto s3 = chrono::high_resolution_clock::now();
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                r3[j] += v[i] * m[i][j];
            }
        }
        auto e3 = chrono::high_resolution_clock::now();
        double d3 = chrono::duration<double, milli>(e3 - s3).count();
        cout << setw(8) << n
            << setw(20) << fixed << setprecision(3) << d1
            << setw(26) << d2
            << setw(28) << d3 << endl;
        delete[] v;
        delete[] r1;
        delete[] r2;
        delete[] r3;
        for (int i = 0; i < n; i++) {
            delete[] m[i];
        }
        delete[] m;
    }
}

int main() {
    SetConsoleOutputCP(65001);
    srand(static_cast<unsigned int>(time(0)));
    int n_demo = 5;
    cout << "=== Демонстрація на матриці розмірністю " << n_demo << "x" << n_demo << " (тип int) ===" << endl;

    int* vec = new int[n_demo];
    int* resVec1 = new int[n_demo];
    int* resVec2 = new int[n_demo];
    int* resVec3 = new int[n_demo];

    int** mat = new int* [n_demo];
    for (int i = 0; i < n_demo; i++) {
        mat[i] = new int[n_demo];
        resVec1[i] = 0;
        resVec2[i] = 0;
        resVec3[i] = 0;
    }

    for (int i = 0; i < n_demo; i++) {
        vec[i] = rand() % 20 - 10;
        for (int j = 0; j < n_demo; j++) {
            mat[i][j] = rand() % 20 - 10;
        }
    }

    cout << "\nВихідний вектор:" << endl;
    printVector(vec, n_demo);

    cout << "\nВихідна матриця:" << endl;
    printMatrix(mat, n_demo);

    for (int i = 0; i < n_demo; i++) {
        for (int j = 0; j < n_demo; j++) {
            resVec1[i] += mat[i][j] * vec[j];
        }
    }
    cout << "\nРезультат (Спосіб 1: Матриця * Вектор):" << endl;
    printVector(resVec1, n_demo);

    for (int j = 0; j < n_demo; j++) {
        for (int i = 0; i < n_demo; i++) {
            resVec2[j] += vec[i] * mat[i][j];
        }
    }
    cout << "\nРезультат (Спосіб 2: Вектор * Матриця - повільний):" << endl;
    printVector(resVec2, n_demo);

    for (int i = 0; i < n_demo; i++) {
        for (int j = 0; j < n_demo; j++) {
            resVec3[j] += vec[i] * mat[i][j];
        }
    }
    cout << "\nРезультат (Спосіб 3: Вектор * Матриця - оптимізований):" << endl;
    printVector(resVec3, n_demo);

    delete[] vec;
    delete[] resVec1;
    delete[] resVec2;
    delete[] resVec3;
    for (int i = 0; i < n_demo; i++) delete[] mat[i];
    delete[] mat;

    cout << "\n\n=== ЕКСПЕРИМЕНТ ДЛЯ ТИПІВ ДАНИХ ТА РОЗМІРІВ (N від 1000 до 10000) ===" << endl;

    runExperiment<int>("INT", 1);
    runExperiment<float>("FLOAT", 1.0f);
    runExperiment<double>("DOUBLE", 1.0);

    cout << "\nЕксперимент повністю завершено. Натисніть Enter для виходу...";
    cin.get();
    return 0;
}
