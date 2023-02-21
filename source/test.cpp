#include<iostream>
#include<vector>
using namespace std;
int main(){
    vector<int> v={1,2,3};
    v.clear();
    cout<<v.empty()<<endl;
    v.emplace_back(4);
    cout<<v[0]<<endl;
    return 0;
}