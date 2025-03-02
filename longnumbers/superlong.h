#pragma once

#include <cstring>
#include <cstdlib>
#include <string>

/// функция сложения с переносом (вне класса)
inline uint32_t add_CF(const uint32_t a, const uint32_t b, uint32_t & CF)
{
    uint32_t res;
    res = a + b + CF;
    CF = res < a ? 1 : 0;
    return res;
}

/// функция представления 32-х разрядного числа в виде строки из восьми шестнадцатеричных цифр
inline char * hex_digit(uint32_t u)
{
    const char* d = "0123456789abcdef";
    static char hex_digit_str[8];
    for (int i = 7; i >= 0; i--) {
        hex_digit_str[i] = d[u & 0x0000000f];
        u >>= 4;
    }
    return hex_digit_str;
}

/// класс для работы с беззнаковыми длинными числами
template <size_t N> class superlong
{
private:
    /// размер массива для внутреннего представления числа, где N - разрядность длинного числа
    static constexpr size_t SIZE = N / 32 + (N % 32 != 0);
    /// указатель на массив для хранения числа
    uint32_t * value;

public:
	/// Коструктор по умолчанию, инициализация нулем
    superlong():value(new uint32_t[SIZE]{}) {}
    /// Конструктор копирования
    superlong(const superlong & n);
    /// Конструктор инициализации массивом 32-х битных беззнаковых
    superlong(const uint32_t* v);
    /// Конструктор инициализации шестнадцатеричным числом в виде строки
    superlong(const char* s);
    /// Деструктор
    ~superlong() { delete[] value; }
    /// метод для представления длинного числа в виде строки шестнадцатеричных цифр с ведущими нулями
    std::string toHex();
    /// Оператор присваивания. Числа должны быть одинаковой длины
    superlong<N>& operator=(const superlong<N>& n);
    /// Оператор сложения. Числа должны быть одинаковой длины
    superlong<N> operator+(const superlong<N>& n)const;
    /// Оператор умножения. Числа должны быть одинаковой длины. Результат двйного размера
    superlong<N*2> operator*(const superlong<N>& n)const;
    /// Оператор вывода в поток (сдвига)
    friend  std::ostream& operator<< (std::ostream& stream, superlong<N>& n){
        return stream << n.toHex();
    }
};


// реализация методов класса superlong

template <size_t N>
superlong<N>::superlong(const superlong & n)
{
    value = new uint32_t[SIZE];
    memcpy(value, n.value, SIZE * sizeof(uint32_t));
}

template <size_t N>
superlong<N>::superlong(const uint32_t * v)
{
    value = new uint32_t[SIZE];
    memcpy(value, v, SIZE * sizeof(uint32_t));
}

template <size_t N>
superlong<N>::superlong(const char* s)
{
    value = new uint32_t[SIZE] {};
    int len = strlen(s);
    int delta = 8;
    char digit[8] = {};
    for (unsigned i = 0; i < SIZE; i++) {
        if (len < delta) {
            delta = len;
        }
        len -= delta;
        strncpy(digit, s+len, delta);
        digit[delta] = '\0';
        value[i] = strtoul(digit, nullptr, 16);
        if (len == 0)
            break;
    }
}

template <size_t N>
superlong<N>& superlong<N>::operator=(const superlong<N>& n)
{
    if (this != &n) {
        value = new uint32_t[SIZE];
        memcpy(value, n.value, SIZE * sizeof(uint32_t));
    }
    return *this;
}

template <size_t N>
superlong<N> superlong<N>::operator+(const superlong<N>& n)const
{
    superlong<N> sum;
    uint32_t CF = 0;
    for (size_t i = 0; i < SIZE; i++)
        sum.value[i] = add_CF(value[i], n.value[i], CF);
    return sum;
}

template <size_t N>
superlong<N*2> superlong<N>::operator*(const superlong<N>& n)const
{
    uint32_t prod[SIZE * 2] = {};
    uint32_t CF = 0;
    for (size_t i = 0; i < SIZE; i++) {
        for (size_t j = 0; j < SIZE; j++) {
            uint64_t tmp = uint64_t(value[j]) * uint64_t(n.value[i]);
            for (int k = 0; i + j + k < 2 * SIZE; k++){
                prod[i + j + k] = add_CF(prod[i + j + k], tmp, CF);
                tmp >>= 32;
            }
        }
    }
    return superlong<N*2>(prod);
}

template <size_t N>
std::string superlong<N>::toHex()
{
	std::string s;
    for (unsigned i=0; i<SIZE; i++) 
    	s += hex_digit(value[SIZE - i - 1]);
    return s;
}




