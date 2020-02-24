/*Модифицированный модуль "шифратора"*/
#include "modAlphaCipher.h"

modAlphaCipher::modAlphaCipher(const std::string& skey) // конструктор
{
    for (unsigned i=0; i<=numAlpha.size(); i++) {
        alphaNum[numAlpha[i]]=i;
    }
    std::wstring wskey = towstr(skey);
    key = convert(getValidKey(wskey));;
}

std::string modAlphaCipher::encrypt(const std::string& open_text) // зашифрование
{
    std::wstring open_wtext = towstr (open_text);
    std::vector<int> work = convert(getValidOpenText(open_wtext));
    for(unsigned i=0; i <= work.size(); i++) {
        work[i] = (work[i] + key[i % key.size()]) % alphaNum.size();
    }
    return convert(work);
}

std::string modAlphaCipher::decrypt(const std::string& cipher_text) // расшифрование
{
    std::wstring cipher_wtext = towstr (cipher_text);
    std::vector<int> work = convert(getValidCipherText(cipher_wtext));
    for(unsigned i=0; i <= work.size(); i++) {
        work[i] = (work[i] + alphaNum.size() - key[i % key.size()]) % alphaNum.size();
    }
    return convert(work);
}

inline std::vector<int> modAlphaCipher::convert(const std::wstring& ws) // из строки в вектор
{
    std::vector<int> result;
    for(auto wc:ws) {
        result.push_back(alphaNum[wc]);
    }
    return result;
}

inline std::string modAlphaCipher::convert(const std::vector<int>& v) // из вектора в строку
{
    std::wstring wresult;
    for(auto i:v) {
        wresult.push_back(numAlpha[i]);
    }
    std::string result = fromwstr(wresult);
    return result;
}

std::wstring modAlphaCipher::towstr(const std::string& s)
{
    std::locale loc("ru_RU.UTF-8"); // русская локаль для корректной смены регистра
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> codec; //кодек UTF-8
    std::wstring ws = codec.from_bytes(s);
    return ws;
}

std::string modAlphaCipher::fromwstr(const std::wstring& ws)
{
    std::locale loc("ru_RU.UTF-8"); // русская локаль для корректной смены регистра
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> codec; //кодек UTF-8
    std::string s = codec.to_bytes(ws);
    return s;
}

inline std::wstring modAlphaCipher::getValidKey(const std::wstring & ws)
{
    std::locale loc("ru_RU.UTF-8"); // русская локаль для корректной смены регистра
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> codec; //кодек UTF-8
    if (ws.empty())
        throw cipher_error("Empty key");
    std::wstring tmp(ws);
    for (auto & wc:tmp) {
        if (!isalpha(wc, loc)) {
            std::string s = fromwstr(ws);
            throw cipher_error(std::string("Invalid key ")+s);
        }
        if (islower(wc, loc))
            wc = toupper(wc, loc);
    }
    return tmp;
}

inline std::wstring modAlphaCipher::getValidOpenText(const std::wstring & ws)
{
    std::locale loc("ru_RU.UTF-8"); // русская локаль для корректной смены регистра
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> codec; //кодек UTF-8
    std::wstring tmp;
    for (auto wc:ws) {
        if (isalpha(wc, loc)) {
            if (islower(wc,loc))
                tmp.push_back(toupper(wc,loc));
            else
                tmp.push_back(wc);
        }
    }
    if (tmp.empty())
        throw cipher_error("Empty open text");
    return tmp;
}

inline std::wstring modAlphaCipher::getValidCipherText(const std::wstring & ws)
{
    std::locale loc("ru_RU.UTF-8"); // русская локаль для корректной смены регистра
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> codec; //кодек UTF-8
    if (ws.empty())
        throw cipher_error("Empty cipher text");
    for (auto wc:ws) {
        if (!isupper(wc, loc)) {
            std::string s = fromwstr(ws);
            throw cipher_error(std::string("Invalid cipher text ")+s);
        }
    }
    return ws;
}
