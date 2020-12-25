//
// Created by arrias on 25.12.2020.
//
#include "matrix.h"
#include "random"
#include "chrono"
#include "bitset"
#include "cassert"
#include "map"

mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());

const matrix G({
                       {1, 0, 1, 1},
                       {1, 1, 0, 1},
                       {0, 0, 0, 1},
                       {1, 1, 1, 0},
                       {0, 0, 1, 0},
                       {0, 1, 0, 0},
                       {1, 0, 0, 0}
               });

const matrix H({
                       {0, 0, 0, 1, 1, 1, 1},
                       {0, 1, 1, 0, 0, 1, 1},
                       {1, 0, 1, 0, 1, 0, 1},
               });

// Получить из блока битов кодовое слово
matrix code(const matrix &block) {
    return G * block;
}

// Получить контрольную сумму
matrix getControlSum(const matrix &msg) {
    return H * msg;
}

// Найти, какой бит кодового слова был испорчен
int getValue(const matrix &control_sum) {
    int ret = 0;
    int cur_pow = 1;
    for (int i = control_sum.n - 1; i >= 0; --i) {
        ret += cur_pow * control_sum(i, 0);
        cur_pow <<= 1;
    }
    return ret - 1;
}

// Разбить строку на битовые блоки
vector<matrix> getBlocks(string &s) {
    vector<matrix> ret;
    for (char &c : s) {
        bitset<BLOCK_SIZE * 2> bi = (int) c;
        matrix f_half({
                              {bi[0]},
                              {bi[1]},
                              {bi[2]},
                              {bi[3]},
                      });
        matrix s_half({
                              {bi[4]},
                              {bi[5]},
                              {bi[6]},
                              {bi[7]},
                      });
        ret.push_back(f_half);
        ret.push_back(s_half);
    }
    return ret;
}

// Испортить один бит кодового слова
void messUp(vector<matrix> &msg) {
    int pos = (int) (rnd() % (int) msg.size());
    int row = (int) (rnd() % (int) msg[pos].n);
    msg[pos](row, 0) = add(msg[pos](row, 0), 1);
}

// Раскодировать маску в символ
char getSymbol(bitset<BLOCK_SIZE> f_half, bitset<BLOCK_SIZE> s_half) {
    bitset<8> mrg;
    for (int i = 0; i < BLOCK_SIZE; ++i)
        mrg[i] = f_half[i];
    for (int i = BLOCK_SIZE; i < BLOCK_SIZE * 2; ++i)
        mrg[i] = s_half[i - BLOCK_SIZE];
    return (char) mrg.to_ulong();
}

int main() {
    // Запомним, из каких блоков какие получаются кодовые слова для раскодирования
    map<int, bitset<BLOCK_SIZE>> decode;

    for (int mask = 0; mask < (1 << BLOCK_SIZE); ++mask) {
        auto now = bitset<BLOCK_SIZE>(mask);
        matrix ma(now);
        decode[getValue(code(ma))] = now;
    }

    string str = "I love linear algebra!";

    auto encrypt = getBlocks(str);
    assert("coded size not equal 2 * word_length" && encrypt.size() == 2 * str.size());

    for (auto &i : encrypt) {
        i = code(i);
    }

    messUp(encrypt);

    vector<bitset<BLOCK_SIZE>> masks;

    for (auto &i : encrypt) {
        auto test = getControlSum(i);
        if (!test.isNull()) {
            int num = getValue(test);
            i(num, 0) = add(i(num, 0), 1);
        }
        masks.push_back(decode[getValue(i)]);
    }

    string strDecode;
    for (int i = 0; i < masks.size(); i += 2) {
        strDecode += getSymbol(masks[i], masks[i + 1]);
    }
    cout << strDecode << "\n";

    return 0;
}
