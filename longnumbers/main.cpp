#include <iostream>
#include "superlong.h"
int main(int argc, char **argv)
{
    constexpr size_t SZ = 512;
    // конструктор по умолчанию
    superlong<SZ> num1;
    // конструктор инициализации массивом
    superlong<SZ> num2(new uint32_t[SZ/32] {1,2,3,0xabcfffff,0,0x36544ab,0,0xaaaa5555,0,0xbcdebcde,0,0xeeeeeeee,0,0xbadacafa,0,0x0fffffff});
    // конструктор копирования
    superlong<SZ> num3 (num2);
    // конструкторы иницициализации строкой
    superlong<SZ/2> num4("996749000dcccdeeeeeea003e9ef0123ae151d238bcbed3ddf00156de99ae2c6");
    superlong<SZ/2> num5("a33fffff0874a10ba6185295c9f7d4c076900d93551a0fc21d64dcfce6d71588");
    // оператор присваивания
    num1 = num3;
    // самоприсваивание
    num2 = num2;
    // умножение
    num3 = num4 * num5;
    // сложение
    num1 = num1 + num3;
    
    // вывод
    
    std::cout<<num1<<std::endl;
    //71d31d4bb476aee94f410802758514c53415119f234935b74c364d7f2e7649e3fd6cda48b6ffe1929fc165f8d762760455cbf2781dc745af4baf224a272cb731

    std::cout<<num2<<std::endl;
    //0fffffff00000000badacafa00000000eeeeeeee00000000bcdebcde00000000aaaa555500000000036544ab00000000abcfffff000000030000000200000001

    std::cout<<num3<<std::endl;
    //61d31d4cb476aee894663d08758514c4452622b1234935b68f5790a12e7649e352c284f3b6ffe1929c5c214dd7627603a9fbf2791dc745ac4baf2248272cb730
    
    std::cout<<num4<<std::endl;
    //996749000dcccdeeeeeea003e9ef0123ae151d238bcbed3ddf00156de99ae2c6
    
    std::cout<<num5<<std::endl;
    //a33fffff0874a10ba6185295c9f7d4c076900d93551a0fc21d64dcfce6d71588
    
    return 0;
}
