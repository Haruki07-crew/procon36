#include<bits/stdc++.h>
using namespace std;
using vvi = vector<vector<int>>;
using vi = vector<int>;
using tpi = tuple<int,int,int>;
int dx[4]={1,-1,0,0};
int dy[4]={0,0,1,-1};


//BFS関係
vvi kaiten_bfs(vvi&field,int y,int x,int size){
  vvi returnfield=field;
  vvi tmp(size,vi(size));
  for(int i=0;i<size;i++){
    for(int j=0;j<size;j++){
      tmp[i][j]=field[y+i][x+j];
    }
  }
  for(int i=0;i<size;i++){
    for(int j=0;j<size;j++){
      returnfield[y+j][x+(size-1-i)]=tmp[i][j];
    }
  }
  return returnfield;
}
bool seikai(vvi&field,int size){
  for(int i=0;i<size;i++){
    for(int j=0;j<size;j++){
      int num=field[i][j];
      bool ok=false;
      for(int d=0;d<4;d++){
        int ny=i+dy[d];
        int nx=j+dx[d];
        if(ny>=0&&ny<size&&nx>=0&&nx<size){
          if(field[ny][nx]==num){
            ok=true;
            break;
          }
        }
      }
      if(!ok)return false;
    }
  }
  return true;
}
vector<tuple<int,int,int>>bfs(vvi&field,int size){
  map<vvi,vvi>maenoyatu;
  map<vvi,tuple<int,int,int>>sousa;
  queue<vvi>q;
  if(seikai(field,size))return{};
  q.push(field);
  maenoyatu[field]=field;
  vvi goal;
  bool ok=false;
  while(!q.empty()){
    vvi cur=q.front();
    q.pop();
    for(int sizer=2;sizer<=size;sizer++){
      for(int i=0;i<=size-sizer;i++){
        for(int j=0;j<=size-sizer;j++){
          vvi nex=kaiten_bfs(cur,i,j,sizer);
          if(maenoyatu.find(nex)==maenoyatu.end()){
            maenoyatu[nex]=cur;
            sousa[nex]=make_tuple(sizer,i,j);
            q.push(nex);
            if(seikai(nex,size)){
              goal=nex;
              ok=true;
              goto END;
            }
          }
        }
      }
    }
  }
  END:
  if(!ok)return{};
  vector<tuple<int,int,int>>sousajun;
  vvi cur=goal;
  while(cur!=field){
    sousajun.push_back(sousa[cur]);
    cur=maenoyatu[cur];
  }
  reverse(sousajun.begin(),sousajun.end());
  return sousajun;
}
void yonyon(vvi&field,vvi&tmp,int size){
  for(int i=0;i<4;i++){
    for(int j=0;j<4;j++){
      tmp[i][j]=field[size-4+i][size-4+j];
    }
  }
}
vector<tpi>main_bfs(vvi&field,int size){
  vvi tmp_field(4,vi(4));
  yonyon(field,tmp_field,size);
  vector<tpi>ret=bfs(tmp_field,4);
  return ret;
}






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


// size-4までのやつで端に来た時の回転
vector<tpi>special_kaiten(vvi&field,int size,int y,int x,int anoy,int anox){
  vector<tpi>ret;
  if(x==anox){//もう一方の位置が同じ列にある場合
    int diffy=anoy-y-1;
    while(diffy--){
      ret.push_back({anoy-1,anox,2});
      kaiten(field,anoy-1,anox,2);
      anoy--;
    }
    kaiten(field,anoy-1,anox,2);
    ret.push_back({anoy-1,anox,2});
  }else if(x<anox){// もう一方の位置が一番端にある場合
    if(anoy==size-1){
      kaiten(field,anoy-1,anox-1,2);
      ret.push_back({anoy-1,anox-1,2});
      int diffy=anoy-y-1;
      while(diffy--){
        ret.push_back({anoy-1,anox,2});
        kaiten(field,anoy-1,anox,2);
        anoy--;
      }
      kaiten(field,anoy-1,anox,2);
      ret.push_back({anoy-1,anox,2});
    }else if(anoy!=y){
      for(int i=0;i<3;i++){
        kaiten(field,anoy,anox-1,2);
        ret.push_back({anoy,anox-1,2});
      }
      anox--;
      int diffy=anoy-y;
      while(diffy--){
        ret.push_back({anoy-1,anox,2});
        kaiten(field,anoy-1,anox,2);
        anoy--;
      }
    }
  }else{//もう一方がsize-2より左にある場合
    if(anoy==size-1){//もう一方が一番下にある場合
      ret.push_back({anoy-1,anox,2});
      kaiten(field,anoy-1,anox,2);
      ret.push_back({anoy-1,anox,2});
      anoy--;
      int diffx=x-anox;
      while(diffx--){
        ret.push_back({anoy,anox,2});
        kaiten(field,anoy,anox,2);
        anox++;
      }
      int diffy=y-anoy;
      while(diffy--){
        ret.push_back({anoy-1,anox,2});
        kaiten(field,anoy-1,anox,2);
        anoy--;
      }
    }
  }
  return ret;
}


// size-4までのやつで通常の回転
vector<tpi>normal_kaiten(vvi&field,int size,int y,int x,int anoy,int anox){
  vector<tpi>ret;
  if(x==anox){//列が同じ場合
    int diffy=y-anoy;
    while(diffy--){
      ret.push_back({anoy-1,anox,2});
      kaiten(field,anoy-1,anox,2);
      anoy--;
    }
  }else if(x<anox){//もう一方が右にある場合
    if(x+1==anox&&anoy==y){

    }else if(x+1==anox&&y+1==anoy){
      kaiten(field,anoy-1,anox,2);
      ret.push_back({anoy-1,anox,2});
    }else if(y==anoy&&x+1!=anox){
      int diffx=anox-x-1;
      while(diffx--){
        for(int i=0;i<3;i++){
          kaiten(field,anoy,anox-1,2);
          ret.push_back({anoy,anox-1,2});
        }
        anox--;
      }
    }else{
      int diffx=anox-x;
      while(diffx--){
        kaiten(field,anoy-1,anox-1,2);
        ret.push_back({anoy-1,anox-1,2});
        anox--;
      }
      int diffy=y-anoy;
      while(diffy--){
        kaiten(field,anoy-1,anox,2);
        ret.push_back({anoy-1,anox,2});
        anoy--;
      }
    }
  }else{//もう一方が左にある場合
    if(anoy==size-1){
      kaiten(field,anoy-1,anox,2);
      ret.push_back({anoy-1,anox,2});
      anoy--;
    }
    int diffx=x-anox;
    while(diffx--){
      kaiten(field,anoy,anox,2);
      ret.push_back({anoy,anox,2});
      anox++;
    }
    int diffy=y-anoy;
    while(diffy--){
      kaiten(field,anoy-1,anox,2);
      ret.push_back({anoy-1,anox,2});
      anoy--;
    }
  }
  return ret;
}


// 下4行のやつ
vector<tpi>specific_kaiten(vvi&field,int size,int y,int x,int anoy,int anox){
  vector<tpi>ret;
  if(anoy==size-4&&y!=anoy){
    kaiten(field,anoy,anox-1,2);
    ret.push_back({anoy,anox-1,2});
    anoy++;
  }
  if(y==anoy){
    int diffx=anox-x;
    while(diffx--){
      for(int i=0;i<3;i++){
        kaiten(field,anoy,anox-1,2);
        ret.push_back({anoy,anox-1,2});
      }
      anox--;
    }
  }else if(x==anox){
    int diffy=y-anoy-1;
    while(diffy--){
      kaiten(field,anoy-1,anox,2);
      ret.push_back({anoy-1,anox,2});
      anoy--;
    }
  }else{
    if(x+1==anox&&y+1==anoy){
      kaiten(field,anoy-1,anox,2);
      ret.push_back({anoy-1,anox,2});
      for(int i=0;i<3;i++){
        kaiten(field,y,x,2);
        ret.push_back({y,x,2});
      }
    }else if(anoy==size-1){
      for(int i=0;i<2;i++){
        kaiten(field,anoy-1,anox-1,2);
        ret.push_back({anoy-1,anox-1,2});
      }
      anoy--;
      anox--;
      int diffx=anox-x;
      while(diffx--){
        for(int i=0;i<3;i++){
          kaiten(field,anoy,anox-1,2);
          ret.push_back({anoy,anox-1,2});
        }
        anox--;
      }
    }else{
      int diffx=anox-x-1;
      while(diffx--){
        kaiten(field,anoy-1,anox-1,2);
        ret.push_back({anoy-1,anox-1,2});
        anox--;
      }
      if(y<anoy){
        for(int i=0;i<3;i++){
          kaiten(field,anoy,anox,2);
          ret.push_back({anoy,anox,2});
        }
        for(int i=0;i<3;i++){
          kaiten(field,y,x,2);
          ret.push_back({y,x,2});
        }
      }else{
        kaiten(field,anoy-1,anox,2);
        ret.push_back({anoy-1,anox,2});
        for(int i=0;i<3;i++){
          kaiten(field,y,x,2);
          ret.push_back({y,x,2});
        }
      }
    }
  }
  return ret;
}

//main_solve 
vector<tpi>solve(vvi&field,int size){
  vector<tpi>ans,ans1,ans2,ans3;
  // 上からsize-4行を揃える
  for(int i=0;i<size-4;i++){
    for(int j=0;j<size;j+=2){
      int y,x;
      tie(y,x)=find_pos(field,i,j,size);
      if(j==size-2){
        auto v=special_kaiten(field,size,i,j,y,x);
        ans1.insert(ans1.end(),v.begin(),v.end());
      }else{
        auto v=normal_kaiten(field,size,i,j,y,x);
        ans1.insert(ans1.end(),v.begin(),v.end());
      }
    }
  }
  
  for(int j=0;j<size-4;j++){
    for(int i=size-4;i<size;i+=2){
      int y,x;
      tie(y,x)=find_pos(field,j,i,size);
      auto v=specific_kaiten(field,size,j,i,y,x);
      ans2.insert(ans2.end(),v.begin(),v.end());
    }
  }

  ans3=main_bfs(field,size);
  ans.insert(ans.end(),ans1.begin(),ans1.end());
  ans.insert(ans.end(),ans2.begin(),ans2.end());
  ans.insert(ans.end(),ans3.begin(),ans3.end());
  return ans;
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
  for(int i=0;i<n;i++){
    for(int j=0;j<n;j++){
      cout<<field[i][j]<<" ";
    }
    cout<<endl;
  }
  cout<<"操作回数: "<<(int)ans.size()<<endl;
  for(int i=0;i<ans.size();i++){
    int y,x,l;
    tie(y,x,l)=ans[i];
    cout<<"("<<y<<","<<x<<") "<<l<<endl;
  }
  cout<<endl;
}