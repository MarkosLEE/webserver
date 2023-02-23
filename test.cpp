#include<iostream>
#include<memory>
#include<cstdio>
#include<vector>
using namespace std;
class apple{
public:
    ~apple(){
        printf("aaa\n");
    }
};
int main(){
     unique_ptr<char[]>q(new char[8]);
    // for(int i=0; i<4; i++){
    //     q[i]='a';
    // }
    // printf("%s\n",q.get());
    // char *temp=q.get();
    // temp++;
    // for(int i=0; i<3; i++){
    //     (*temp)='b';
    // }
    // printf("%s\n",q.get());
    unique_ptr<vector<apple>>v(new vector<apple>);
    v->emplace_back(std::move(apple()));
}