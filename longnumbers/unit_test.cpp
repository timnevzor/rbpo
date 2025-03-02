#include <iostream>
#include <sstream>
#include <cstring>
#include <cassert>
#include "superlong.h"
int main(int argc, char **argv)
{
	{
		// test 1 конструктор по умолчанию - проверка на 0 
    	superlong<128> n;
		assert(n.toHex() == "00000000000000000000000000000000"); 
		std::cout << "Test 1  Ok\n";
	}
	{
    	// test 2 конструктор инициализации массивом
    	uint32_t data[512/32] {1,2,3,0xabcfffff,0,0x36544ab,0,0xaaaa5555,0,0xbcdebcde,0,0xeeeeeeee,0,0xbadacafa,0,0x0fffffff};
    	superlong<512> n(data);
		assert(n.toHex() == "0fffffff00000000badacafa00000000eeeeeeee00000000bcdebcde00000000\
aaaa555500000000036544ab00000000abcfffff000000030000000200000001"); 
		std::cout << "Test 2  Ok\n";
		
		// test 3 конструктор копирования
    	superlong<512> n2 (n);
		assert(n2.toHex() == "0fffffff00000000badacafa00000000eeeeeeee00000000bcdebcde00000000\
aaaa555500000000036544ab00000000abcfffff000000030000000200000001");
        std::cout << "Test 3  Ok\n";
    }
    {
		// test 4 конструкторы иницициализации строкой
    	superlong<256> n("996749000dcccdeeeeeea003e9ef0123ae151d238bcbed3ddf00156de99ae2c6");
    	assert(n.toHex() == "996749000dcccdeeeeeea003e9ef0123ae151d238bcbed3ddf00156de99ae2c6");
        std::cout << "Test 4  Ok\n";
    	
    	superlong<256> n2("a33fffff0874a10ba6185295c9f7d4c076900d93551a0fc21d64dcfce6d71588");
        
		// test 5 оператор присваивания
    	n = n2;
    	assert(n.toHex() == "a33fffff0874a10ba6185295c9f7d4c076900d93551a0fc21d64dcfce6d71588");
        std::cout << "Test 5  Ok\n";
    	
    	// test 6 самоприсваивание
    	n = n;
    	assert(n.toHex() == "a33fffff0874a10ba6185295c9f7d4c076900d93551a0fc21d64dcfce6d71588");
        std::cout << "Test 6  Ok\n";
    	
    	// test 7 умножение
        superlong<512> m = n * superlong<256>("996749000dcccdeeeeeea003e9ef0123ae151d238bcbed3ddf00156de99ae2c6");
        assert(m.toHex() == "61d31d4cb476aee894663d08758514c4452622b1234935b68f5790a12e7649e3\
52c284f3b6ffe1929c5c214dd7627603a9fbf2791dc745ac4baf2248272cb730");
        std::cout << "Test 7  Ok\n";
        
        // test 8 сложение
        superlong<512> m2 = m + m;
        assert(m2.toHex() == "c3a63a9968ed5dd128cc7a10eb0a29888a4c456246926b6d1eaf21425cec93c6\
a58509e76dffc32538b8429baec4ec0753f7e4f23b8e8b58975e44904e596e60");
        std::cout << "Test 8  Ok\n";
        
        // test 9 вывод
		std::ostringstream oss;
		oss << n;
		assert(oss.str() == "a33fffff0874a10ba6185295c9f7d4c076900d93551a0fc21d64dcfce6d71588");
        std::cout << "Test 9  Ok\n";
    }
 
    return 0;
}
