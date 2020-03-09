#pragma once

#include <cstring>
#include <cstdlib>

/// функция сложения с переносом (вне класса)
uint32_t add_CF(const uint32_t a, const uint32_t b, uint32_t & CF);

/** класс для работы с беззнаковыми длинными числами
 *  длинные числа представляются в виде массива 32-х разрядных беззнаковых чисел
 *  superlong() - коструктор по умолчанию, инициализация нулем
 *  superlong(superlong&) - конструктор
 *  superlong(uint32_t *) - инициализация массивом 32-х битных беззнаковых
 *  superlong(string&) - инициализация шестнадцатеричным числом в виде строки
 *  operator= - присваивание чисел равной длины
 *  operator+ - сложение чисел одинаковой длины
 *  operator* - умножение чисел одинаковой длины, результат двйного размера
 *  operator<< - вывод числа в шестнадцатеричнов виде с ведущими нулями
 */

template <size_t N> class superlong
{
private:
    /// размер массива для внутреннего представления числа, константа
    static constexpr size_t SIZE = N / 32 + (N % 32 != 0);
    /// массив для представления числа
    uint32_t * value;
    /// статический внутренний метод представления 32-х разрядного числа в виде строки из восьми шестнадцатеричных цифр
    static char* toHex(uint32_t u);
    /// внутренний метод для представления длинного числа в виде строки шестнадцатеричных цифр с ведущими нулями
    char* toHex();
public:
    superlong():value(new uint32_t[SIZE]{0}) {}
    superlong(const uint32_t * v);
    superlong(const char * s);
    superlong(const superlong & n);
    superlong<N>& operator=(const superlong<N>& n);
    superlong<N> operator+(const superlong<N>& n)const;
    superlong<N*2> operator*(const superlong<N>& n)const;
    template <size_t X>
    friend  std::ostream& operator<< (std::ostream& stream, superlong<X>& n);
};


// реализация
template <size_t N>
superlong<N>::superlong(const uint32_t * v)
{
    value = new uint32_t[SIZE];
    memcpy(value, v, SIZE * sizeof (uint32_t));
}

template <size_t N>
superlong<N>::superlong(const char* s)
{
    value = new uint32_t[SIZE] {0};
    int len = strlen(s);
    int delta = 8;
    char digit[9]= {0};
    for (unsigned i=0; i<SIZE; i++) {
        if (len < delta) {
            delta = len;
        }
        len -= delta;
        strncpy(digit, s+len, delta);
        digit[delta] = '\0';
        value[i] = strtoul(digit,nullptr,16);
        if (len == 0)
            break;
    }
}

template <size_t N>
superlong<N>::superlong(const superlong & n)
{
    value = new uint32_t[SIZE];
    memcpy(value, n.value, SIZE * sizeof (uint32_t));
}

template <size_t N>
superlong<N>& superlong<N>::operator=(const superlong<N>& n)
{
    if (this != &n) {
        value = new uint32_t[SIZE];
        memcpy(value, n.value, SIZE * sizeof (uint32_t));
    }
    return *this;
}

template <size_t N>
superlong<N> superlong<N>::operator+(const superlong<N>& n)const
{
    superlong<N> sum;
    uint32_t CF=0;
    for (size_t i=0; i<SIZE; i++)
        sum.value[i] = add_CF(value[i], n.value[i], CF);
    return sum;
}

template <size_t N>
superlong<N*2> superlong<N>::operator*(const superlong<N>& n)const
{
    uint32_t prod[SIZE*2] = {0};
    uint32_t CF=0;
    for (size_t i=0; i<SIZE; i++) {
        for (size_t j=0; j<SIZE; j++) {
            uint64_t tmp = uint64_t(value[j]) * uint64_t(n.value[i]);
            for (int k = 0; i+j+k < 2*SIZE; k++){
                prod[i+j+k] = add_CF(prod[i+j+k], tmp, CF);
                tmp >>= 32;
            }
        }
    }
    return superlong<N*2>(prod);
}

template <size_t N>
char* superlong<N>::toHex(uint32_t u)
{
    const char d[]="0123456789abcdef";
    char * s = new char[9] {0};
    for (int i=7; i>=0; i--) {
        s[i] = d[u & 0xf];
        u >>=4;
    }
    return s;
}

template <size_t N>
char* superlong<N>::toHex()
{
    char* s =new char[SIZE +1] {0};
    for (unsigned i=0; i<SIZE; i++) {
        strcpy(s+i*8,superlong<N>::toHex(value[SIZE-i-1]));
    }
    return s;
}


template <size_t X>
std::ostream& operator<<(std::ostream& stream, superlong<X>& n)
{
    stream<<n.toHex();
    return stream;
}

inline uint32_t add_CF(const uint32_t a, const uint32_t b, uint32_t & CF)
{
    uint32_t res;
    res = a + b + CF;
    if (res < a)
        CF = 1;
    else
        CF = 0;
    return res;
}
