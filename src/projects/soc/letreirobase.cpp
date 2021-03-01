#include <iostream>
#include <string>
using namespace std;

#define T 'T'
#define A 'A'
#define D 'D'
#define S 'S'
#define U 'U'
#define F 'F'
#define R 'R'
#define N 'N'

int main() {
    string str = "TADS UFRN";
    char strC[8] = {T,A,D,S,U,F,R,N};
    for (int i = 0; i < str.length(); i++) {
        cout << str[i] << ' ';
    }
    cout << '\n';
    for (int i = 0; i < 8; i++) {
        printf("%c ", strC[i]); // ou alt_printf
    }
}