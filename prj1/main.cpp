/*Главный модуль программы для шифрования алгоритмом Гронсфильда на рус. языке*/
#include <iostream>
#include <cctype>
#include <codecvt>
#include <locale>
#include "modAlphaCipher.h"
using namespace std;


void check(const string& Text, const string& key, const bool destructCipherText=false)
{
    try {
        locale loc("ru_RU.UTF-8"); // русская локаль для корректной смены регистра
        wstring_convert<codecvt_utf8<wchar_t>, wchar_t> codec; //кодек UTF-8
        string cipherText;
        string decryptedText;
        modAlphaCipher cipher(key);
        cipherText = cipher.encrypt(Text);
        cipherText = cipher.encrypt(Text);
        if (destructCipherText) {
            wstring ws = cipher.towstr(cipherText);
            ws.front() = tolower(ws.front(),loc);
            cipherText = cipher.fromwstr(ws);
        }
        decryptedText = cipher.decrypt(cipherText);
        cout<<"key="<<key<<endl;
        cout<<Text<<endl;
        cout<<cipherText<<endl;
        cout<<decryptedText<<endl;
    } catch (const cipher_error & e) {
        cerr<<"Error: "<<e.what()<<endl;
    }
}

int main(int argc, char **argv)
{
    check("АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ","Просто");
    check("АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ","");
    check("АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ","АБВГ123");
    check("АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ","СЕКРЕТНО",true);
    return 0;
}
