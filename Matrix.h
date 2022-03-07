#ifndef UNTITLED1_MATRIX_H
#define UNTITLED1_MATRIX_H
#include <iostream>
#include <vector>
#include <cmath>

#define EXP 1.0e-5
template<typename T>
class Matrix {
private:
    std::vector<std::vector<T>> matrix;
    std::vector<std::vector<T>> adjM;
    std::vector<std::vector<T>> invM;
    int NIn = 0;

    T getDeterminant(std::vector<std::vector<T>> mat, int N);
    int adjoint(const std::vector<std::vector<T>> &mat, std::vector<std::vector<T>> &adj, int N);
    int inverseS(const std::vector<std::vector<T>> &mat, std::vector<std::vector<T>> &inv, int N);

public:
    explicit Matrix(int n);
    Matrix(std::vector<std::vector<T>> matrix, int n);
    ~Matrix()= default;
    int getN() const;
    T getDeterminant() {
        return this->getDeterminant(matrix, NIn);
    }

    std::vector<std::vector<T>> & org() {
        return this->matrix;
    }

    Matrix<T> * getAdjoint();
    Matrix<T> * getInverse();
};
template<typename T>
Matrix<T> * Matrix<T>::getInverse() {
    this->inverseS(matrix, invM, NIn);
    auto *tmp = new Matrix<T>(invM, NIn);
    return tmp;
}

template<typename T>
Matrix<T>* Matrix<T>::getAdjoint() {
    this->adjoint(matrix, adjM, NIn);
    return new Matrix<T>(adjM, NIn);
}

template<typename T>
Matrix<T>::Matrix(int n) {
    std::vector<std::vector<T>> m;
    this->matrix = m;
    this->NIn = n;
}

template<typename T>
Matrix<T>::Matrix(std::vector<std::vector<T>> matrix, int n) {
    this->matrix = matrix;
    this->NIn = n;
}
template<typename T>
T Matrix<T>::getDeterminant(std::vector<std::vector<T>> mat, int N) {
    if (mat.size() != N) {
        return -1;
    }
    for (auto &i : mat) {
        if (i.size() != N) {
            return -1;
        }
    }

    T ret{0};

    if (N == 1) return mat[0][0];

    if (N == 2) {
        return (mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0]);
    } else {
        for (int i = 0; i < N; ++i) {
            std::vector<std::vector<T>> m(N - 1);
            std::vector<int> m_rows;
            for (int t = 0; t < N; ++t) {
                if (i != t) m_rows.push_back(t);
            }
            for (int x = 0; x < N - 1; ++x) {
                m[x].resize(N - 1);
                for (int y = 0; y < N - 1; ++y) {
                    m[x][y] = mat[m_rows[x]][y + 1];
                }
            }
            int sign = (int) pow(-1, 1 + i + 1);
            ret += mat[i][0] * sign * getDeterminant(m, N - 1);
        }
    }

    return ret;
}

template<typename T>
int Matrix<T>::adjoint(const std::vector<std::vector<T>> &mat, std::vector<std::vector<T>> &adj, int N) {
    if (mat.size() != N) {
        return -1;
    }
    for (int i = 0; i < mat.size(); ++i) {
        if (mat[i].size() != N) {
            return -1;
        }
    }

    adj.resize(N);
    for (int i = 0; i < N; ++i) {
        adj[i].resize(N);
    }

    for (int y = 0; y < N; ++y) {
        std::vector<int> m_cols;
        for (int i = 0; i < N; ++i) {
            if (i != y) m_cols.push_back(i);
        }

        for (int x = 0; x < N; ++x) {
            std::vector<int> m_rows;
            for (int i = 0; i < N; ++i) {
                if (i != x) m_rows.push_back(i);
            }

            std::vector<std::vector<T>> m(N - 1);
            for (int i = 0; i < N - 1; ++i) {
                m[i].resize(N - 1);
            }
            for (int j = 0; j < N - 1; ++j) {
                for (int i = 0; i < N - 1; ++i) {
                    m[j][i] = mat[m_rows[j]][m_cols[i]];
                }
            }

            int sign = (int) pow(-1, x + y);
            adj[y][x] = sign * getDeterminant(m, N - 1);
        }
    }
    return 0;
}


template<typename T>
int Matrix<T>::inverseS(const std::vector<std::vector<T>> &mat, std::vector<std::vector<T>> &inv, int N) {
    if (mat.size() != N) {
        return -1;
    }
    for (int i = 0; i < mat.size(); ++i) {
        if (mat[i].size() != N) {
            return -1;
        }
    }

    T det = getDeterminant(mat, N);
    if (fabs(det) < EXP) {
        return -1;
    }

    inv.resize(N);
    for (int i = 0; i < N; ++i) {
        inv[i].resize(N);
    }

    double coef = 1.f / det;
    std::vector<std::vector<T>> adjss;
    if (adjoint(mat, adjss, N) != 0) return -1;

    for (int y = 0; y < N; ++y) {
        for (int x = 0; x < N; ++x) {
            inv[y][x] = (T) (coef * adjss[y][x]);
        }
    }

    return 0;
}

template<typename T>
int Matrix<T>::getN() const {
    return NIn;
}
//数字与矩阵相乘


template<typename T>
static void printMatrix(Matrix<T> & matrix) {
    int rows = matrix.org().size();
    for (int rowIn = 0; rowIn < rows; ++rowIn) {
        for (int each = 0; each < matrix.org()[rowIn].size(); ++each) {
            std::cout << matrix.org()[rowIn][each] << " ";
        }
        std::cout << std::endl;
    }
}

template<typename T>
static void inputMatrix(Matrix<T> * matrix) {
    for (int i = 0; i < matrix->getN(); ++i) {
        auto *rowIn = new std::vector<T>;
        for (int j = 0; j < matrix->getN(); ++j) {
            int tmp;std::cin >> tmp;
            rowIn->push_back((T)tmp);
        }
        matrix->org().push_back(*rowIn);
    }
}

#endif