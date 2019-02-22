#include "Query_Base.h"

//#include "Query.h"
using namespace std;

int main(){

    ifstream file("text");
    TextQuery  pro(file);
    while(1){
        cout << "Enter word to look for, or q to quit: ";
        string s;
        if(!(cin >> s) || s == "q") break;
        //print(cout, pro.query(s) ) << endl;
        Query q = Query("infected") | Query("with");
        print(cout, q.eval(pro)) << endl;
    }
    return 0;
}