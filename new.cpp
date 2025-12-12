#include<bits/stdc++.h>
using namespace std;
using vvi = vector<vector<int>>;
using vi = vector<int>;
using tpi = tuple<int,int,int>;
int dx[4]={1,-1,0,0};
int dy[4]={0,0,1,-1};

// 同じ数の位置を探す
pair<int,int>find_pos(vvi&field,int y,int x,int size){
  for(int i=0;i<size;i++){
    for(int j=0;j<size;j++){
      if((i!=y||j!=x)&&field[y][x]==field[i][j]){
        return make_pair(i,j);
      }
    }
  }
}
// 通常の回転
void kaiten(vvi&field,int y,int x,int size){
  vvi tmp(size,vi(size));
  for(int i=0;i<size;i++){
    for(int j=0;j<size;j++){
      tmp[i][j]=field[y+i][x+j];
    }
  }
  for(int i=0;i<size;i++){
    for(int j=0;j<size;j++){
      field[x+j][y+(size-1-i)]=tmp[i][j];
    }
  }
}
// 端に来た時の回転
vector<tpi>special_kaiten(vvi&field,int size,int y,int x,int anoy,int anox){
  vector<tpi>ret;
  if(x==anox){
    int diffy=anoy-y-1;
    while(diffy--){
      ret.push_back({y-1,x,2});
      kaiten(field,y-1,x,2);
      y--;
    }
    kaiten(field,y-1,x,2);
    ret.push_back({y-1,x,2});
  }else if(x<anox){
    if(anoy==size-1){
      kaiten(field,y-1,x-1,2);
      ret.push_back({y-1,x-1,2});
    }
  }else{

  }
}

vector<tpi>solve(vvi&field,int size){
  vector<vector<tpi>>ans,ans1,ans2;
  for(int i=0;i<size-4;i++){
    for(int j=0;j<size;j++){
      int y,x;
      tie(y,x)=find_pos(field,i,j,size);
      if(j==size-2){
        ans1.push_back(special_kaiten(field,size,i,j,y,x));
      }else{
        ans1.push_back(normal_kaiten(field,size,i,j,y,x));
      }
    }
  }
}

int main(){
  int n;
  cin>>n;
  vvi field(n,vi(n));
  for(int i=0;i<n;i++){
    for(int j=0;j<n;j++){
      cin>>field[i][j];
    }
  }
  vector<tpi>ans=solve(field,n);
  cout<<"操作回数: "<<(int)ans.size()<<endl;
  for(int i=0;i<ans.size();i++){
    int y,x,l;
    tie(y,x,l)=ans[i];
    cout<<"("<<y<<","<<x<<") "<<l<<endl;
  }
}