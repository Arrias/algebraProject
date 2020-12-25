//
// Created by arrias on 25.12.2020.
//
#pragma once

#include <vector>
#include <iostream>
#include <bitset>

using namespace std;

const int MOD = 2;
const int BLOCK_SIZE = 4;

/*
 * Сложение в нашем поле
 */
int add(int a, int b) {
    return (a + b + MOD) % MOD;
}

/*
 * Умножение в наше поле
 */
int mul(int a, int b) {
    return (a * b) % MOD;
}

class matrix {
private:
    vector<vector<int>> data;
public:
    int n{}, m{};

    matrix() = default;

    matrix(int nn, int mm) : n(nn), m(mm) {
        data.assign(n, vector<int>(m, 0));
    }

    explicit matrix(vector<vector<int>> init_vec) : n(init_vec.size()), m(init_vec[0].size()), data(init_vec) {}

    void init(int nn, int mm) {
        n = nn;
        m = mm;
        data.assign(n, vector<int>(m, 0));
    }

    explicit matrix(bitset<BLOCK_SIZE> a) {
        init(BLOCK_SIZE, 1);
        for (int i = 0; i < BLOCK_SIZE; ++i)
            data[i][0] = a[i];
    }

    int &operator()(int i, int j) {
        return data[i][j];
    }

    const int &operator()(int i, int j) const {
        return data[i][j];
    }

    void print() {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                cout << data[i][j] << " ";
            }
            cout << "\n";
        }
    }

    matrix operator*(const matrix &b) const {
        matrix c(n, b.m);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < b.m; ++j) {
                for (int k = 0; k < m; ++k) {
                    c(i, j) = add(c(i, j), mul(data[i][k], b(k, j)));
                }
            }
        }
        return c;
    }

    bool isNull() {
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < m; ++j)
                if (data[i][j])
                    return false;
        return true;
    }
};
