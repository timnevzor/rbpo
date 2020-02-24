
#include <iostream>
#include <cctype>
#include "Cipher.h"
using namespace std;



int main(int argc, char **argv)
{
    int key;
    string text;
    unsigned op;
    try {
        do {
            cout<<"Input operation (0-exit, 1-encrypt, 2-decrypt): ";
            cin>>op;
            if (op > 2)
                cout<<"Illegal operation\n";
            else if (op > 0) {
                cout<<"Cipher ready. Input text: ";
                cin>>text;
                cout<<"Input key: ";
                cin>>key;
                Cipher cipher(key, text);
                if (op = 1) {
                    cout<<"Encrypted text: "<<cipher.encrypt(text)<<endl;
                } else {
                    cout<<"Decrypted text: "<<cipher.decrypt(text)<<endl;
                }
            }
        } while (op != 0);
    } catch (const Error & e) {
        cerr<<"Error: "<<e.what()<<endl;
    }
    return 0;
}
