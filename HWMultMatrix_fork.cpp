#include<iostream>
#include<unistd.h>
using namespace std;

int main() {
    //Create first matrix
    int row_a, col_a;
    cout << "Enter rows of first matrix:\n";
    cin >> row_a;
    cout << "Enter cols of first matrix:\n";
    cin >> col_a;
    int** matrix_a = new int* [row_a] {};
    for (int i = 0; i < row_a; ++i) {
        matrix_a[i] = new int[col_a];
    }
    cout << "Enter elements of first matrix:\n";
    for (int i = 0; i < row_a; ++i) {
        for (int j = 0; j < col_a; ++j) {
            cout << "Matrix A [" << i << "][" << j << "]= ";
            cin >> matrix_a[i][j];
        }
    }

    //Create second matrix
    int row_b,col_b;
    cout << "Enter rows of second matrix:\n";
    cin >> row_b;
    cout << "Enter cols of second matrix:\n";
    cin >> col_b;
    int** matrix_b = new int* [row_b] {};
    for (int i = 0; i < row_b; ++i) {
        matrix_b[i] = new int[col_b];
    }
    cout << "Enter elements of first matrix:\n";
    for (int i = 0; i < row_b; ++i) {
        for (int j = 0; j < col_b; ++j) {
            cout << "Matrix B [" << i << "][" << j << "]= ";
            cin >> matrix_b[i][j];
        }
    }

    //Chech of equal rows of first matrix and cols of second matrix
    if (col_a != row_b) {
        cout << "Error! The number of matrix row elements is not equal to the number of column elements of the second matrix. Multiplication is impossible\n";
        return 0;
    }

    int max_cols;
    max_cols = col_a;

    int** result = new int* [row_a] {};
    for (int i = 0; i < row_a; ++i) {
        result[i] = new int[col_b] {};
    }

    //Create Parent and Child process, and pipe
    int fd[2]{}; //Used to store two ends pipe
    pid_t process;
    if (pipe(fd) == -1) {
        cout << "Error! Pipe Failed";
        return 0;
    }
    process = fork();
    if (process < 0) {
        cout << "Error! Failed enter process";
        return 0;
    }
    else if (process > 0) { //Parent process
        close(fd[0]);
        for (int row = 0; row < row_a; row++) {
            if (row%2 != 1) {
                for (int col = 0; col < col_b; col++) {
                    for (int i = 0; i < max_cols; i++) {
                        result[row][col] += matrix_a[row][i] * matrix_b[i][col];
                    }
                }
            }
        }
        cout << "************************************\n";
        if (write(fd[1], result, sizeof(int)*row_a*10*col_b*10) < 0) {
            cout << "Error! Do not write to fd\n";
            return 0;
        };
        close(fd[1]);
    }
    else if (process == 0) {
        close(fd[1]);
        if (read(fd[0], result, sizeof(int)*row_a*10*col_b*10) < 0) {
            cout << "Error! Do not read from fd\n";
            return 0;
        };
        for (int row = 0; row < row_a; row++) {
            if (row%2 == 1) {
                for (int col = 0; col < col_b; col++) {
                    for (int i = 0; i < max_cols; i++) {
                        result[row][col] += matrix_a[row][i] * matrix_b[i][col];
                    }
                }
            }
        }
        cout << "Result of matrix multiplication:\n";
        for (int row = 0; row < row_a; row++) {
            for (int col = 0; col < col_b; col++) {
                cout << result[row][col] << "\t";
            }
            cout << "\n";
        }
        close(fd[0]);
    }

    //Delete dynamic arrays
    for (int i = 0; i < row_a; ++i) {
        delete[] matrix_a[i];
    }
    delete[] matrix_a;
    for (int i = 0; i < row_b; ++i) {
        delete[] matrix_b[i];
    }
    delete[] matrix_b;
    for (int i = 0; i < row_a; ++i) {
        delete[] result[i];
    }
    delete[] result;
    return 0;
}