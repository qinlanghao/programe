#include <iostream>
using namespace std;
 
void foo(){
    int a=1;
    if(a>0){cout<<"Positive"<<endl;}
    else{cout<<"Non-positive"<<endl;}
}
 
int main(){
  foo();
  return 0;
}
