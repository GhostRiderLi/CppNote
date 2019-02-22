#include "Query.h"
using namespace std;

int main(){

    ifstream file("text");
    TextQery  pro(file);
    while(1){
        cout << "Enter word to look for, or q to quit: ";
        string s;
        if(!(cin >> s) || s == "q") break;
        print(cout, pro.query(s) ) << endl;;
    }
    return 0;
}
