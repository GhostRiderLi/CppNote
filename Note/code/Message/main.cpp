// #include "Message.h"

// using namespace std;

// int main(){

//     Folder folder;
//     Message msg1("message1");
//     Message msg2("message2");
//     Message msg3("message3");
//     Message msg4("message4");
//     Message msg5("message5");
//     Message msg6("message6");

//     msg1.save(folder);
//     msg2.save(folder);
//     msg3.save(folder);
//     msg4.save(folder);
//     msg5.save(folder);
//     msg6.save(folder);

//     print(cout, folder);

//     return 0;
// }
// 

// #include <iostream>
// #include <string>
// #include <set>
// using namespace std;

// int main(){
//     string str;
//     int n;
//     while(cin >> str >> n){
//         string res;
//         int cnt = 0;
//          for(size_t i = 0; i < str.size(); ++i){
//              while(res.size() > 0 && str[res.size() - 1] < str[i] && cnt < n){
//                  cnt++;
//                  res.pop_back();
//              }
//              res.push_back(str[i]);
//          }
//         cout << res.substr(0, str.size() - n) << endl;;
//     }
// }
// 

