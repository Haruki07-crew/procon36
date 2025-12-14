#include<bits/stdc++.h>
using namespace std;
using vvi = vector<vector<int>>;
using vi = vector<int>;
using tpi = tuple<int,int,int>;
int dx[4]={1,-1,0,0};
int dy[4]={0,0,1,-1};

// 安全な回転関数（範囲外なら終了）
void safe_kaiten(vvi &field, int y, int x, int size, const string& func_name="unknown") {
    int n = field.size();
    if (y < 0 || x < 0 || y + size > n || x + size > n) {
        cerr << "OUT OF RANGE in function " << func_name 
             << ": y=" << y << " x=" << x << " size=" << size << " field_size=" << n << endl;
        exit(1);  // 範囲外なら即終了
    }
    vvi tmp(size, vi(size));
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            tmp[i][j]=field[y+i][x+j];
        }
    }
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            field[y+j][x+(size-1-i)] = tmp[i][j];
        }
    }
    cout<<"[DEBUG] safe_kaiten executed in function " << func_name 
        << ": y=" << y << " x=" << x << " size=" << size << endl;
}

// BFS用回転（safe版）
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
  cerr<<"見つかりません y="<<y<<" x="<<x<<endl;
  exit(1);  // 見つからなければ終了
  return {-1,-1};
}

// size-4までのやつで端に来た時の回転
vector<tpi>special_kaiten(vvi&field,int size,int y,int x,int anoy,int anox){
  vector<tpi>ret;
  if(x==anox){
    int diffy=anoy-y-1;
    while(diffy--){
      ret.push_back({anoy-1,anox,2});
      safe_kaiten(field,anoy-1,anox,2,"special_kaiten");
      anoy--;
    }
    safe_kaiten(field,anoy-1,anox,2,"special_kaiten");
    ret.push_back({anoy-1,anox,2});
  } else if(x<anox){
    if(anoy==size-1){
      safe_kaiten(field,anoy-1,anox-1,2,"special_kaiten");
      ret.push_back({anoy-1,anox-1,2});
      int diffy=anoy-y-1;
      while(diffy--){
        ret.push_back({anoy-1,anox,2});
        safe_kaiten(field,anoy-1,anox,2,"special_kaiten");
        anoy--;
      }
      safe_kaiten(field,anoy-1,anox,2,"special_kaiten");
      ret.push_back({anoy-1,anox,2});
    } else if(anoy!=y){
      for(int i=0;i<3;i++){
        safe_kaiten(field,anoy,anox-1,2,"special_kaiten");
        ret.push_back({anoy,anox-1,2});
      }
      anox--;
      int diffy=anoy-y;
      while(diffy--){
        ret.push_back({anoy-1,anox,2});
        safe_kaiten(field,anoy-1,anox,2,"special_kaiten");
        anoy--;
      }
    }
  } else {
    if(anoy==size-1){
      ret.push_back({anoy-1,anox,2});
      safe_kaiten(field,anoy-1,anox,2,"special_kaiten");
      ret.push_back({anoy-1,anox,2});
      anoy--;
      int diffx=x-anox;
      while(diffx--){
        ret.push_back({anoy,anox,2});
        safe_kaiten(field,anoy,anox,2,"special_kaiten");
        anox++;
      }
      int diffy=anoy-y;
      while(diffy--){
        ret.push_back({anoy-1,anox,2});
        safe_kaiten(field,anoy-1,anox,2,"special_kaiten");
        anoy--;
      }
    }else{
        int diffx=x-anox;
        while(diffx--){
            safe_kaiten(field,anoy,anox,2,"special_kaiten");
            ret.push_back({anoy,anox,2});
            anox++;
        }
        int diffy=anoy-y;
        while(diffy--){
            safe_kaiten(field,anoy-1,anox,2,"special_kaiten");
            ret.push_back({anoy-1,anox,2});
        }
    }
  }
  return ret;
}

// normal_kaiten
vector<tpi>normal_kaiten(vvi&field,int size,int y,int x,int anoy,int anox){
  vector<tpi>ret;
  if(x==anox){
    int diffy=anoy-y;
    while(diffy--){
      ret.push_back({anoy-1,anox,2});
      safe_kaiten(field,anoy-1,anox,2,"normal_kaiten1");
      anoy--;
    }
    if(anoy!=y){
      cerr<<"error in normal_kaiten : x==anox"<<endl;
      exit(1);
    }
  }else if(x<anox){
    if(x+1==anox&&y+1==anoy){
      safe_kaiten(field,anoy-1,anox,2,"normal_kaiten2");
      ret.push_back({anoy-1,anox,2});
      
    }else if(y==anoy&&x+1!=anox){
      int diffx=anox-x-1;
      while(diffx--){
        for(int i=0;i<3;i++){
          safe_kaiten(field,anoy,anox-1,2,"normal_kaiten3");
          ret.push_back({anoy,anox-1,2});
        }
        anox--;
      }
    }else{
      int diffx=anox-x;
      while(diffx--){
        safe_kaiten(field,anoy-1,anox-1,2,"normal_kaiten4");
        ret.push_back({anoy-1,anox-1,2});
        anox--;
      }
      int diffy=anoy-y;
      while(diffy--){
        //  cerr << "[DEBUG normal_kaiten5] anoy=" << anoy 
        //  << " anox=" << anox 
        //  << " y=" << y 
        //  << " x=" << x << "diffy="<< diffy<<endl;
        safe_kaiten(field,anoy-1,anox,2,"normal_kaiten5");
        ret.push_back({anoy-1,anox,2});
        anoy--;
      }
      if(anoy!=y||anox!=x){
        cerr<<"error in normal_kaiten : x<anox"<<endl;
        exit(1);
      }
    }
    if(anoy!=y||anox!=x){
      cerr<<"error in normal_kaiten : x<anox"<<endl;
      exit(1);
    }
  }else{
    if(anoy==size-1){
      safe_kaiten(field,anoy-1,anox,2,"normal_kaiten6");
      ret.push_back({anoy-1,anox,2});
      anoy--;
    }
    int diffx=x-anox;
    while(diffx--){
      safe_kaiten(field,anoy,anox,2,"normal_kaiten7");
      ret.push_back({anoy,anox,2});
      anox++;
    }
    int diffy=anoy-y;
     cerr << "[DEBUG normal_kaiten8] anoy=" << anoy 
         << " anox=" << anox 
         << " y=" << y 
         << " x=" << x << " diffy="<< diffy<<endl;
    while(diffy--){
      safe_kaiten(field,anoy-1,anox,2,"normal_kaiten8");
      ret.push_back({anoy-1,anox,2});
      anoy--;
    }
    if(anoy!=y||anox!=x){
      cerr<<"error in normal_kaiten : else"<<endl;
      exit(1);
    }
  }
  return ret;
}

// specific_kaiten
vector<tpi>specific_kaiten(vvi&field,int size,int y,int x,int anoy,int anox){
  vector<tpi>ret;
  if(anoy==size-4&&y!=anoy){
    safe_kaiten(field,anoy,anox-1,2,"specific_kaiten1");
    ret.push_back({anoy,anox-1,2});
    anoy++;
  }
  if(y==anoy){
    int diffx=anox-x;
    while(diffx--){
      for(int i=0;i<3;i++){
        safe_kaiten(field,anoy,anox-1,2,"specific_kaiten2");
        ret.push_back({anoy,anox-1,2});
      }
      anox--;
    }
  }else if(x==anox){
    int diffy=anoy-y;
    while(diffy--){
      cout<<"[DEBUG specific_kaiten] anoy="<<anoy<<" anox="<<anox<<" y="<<y<<" x="<<x<<" diffy="<<diffy<<endl;
      safe_kaiten(field,anoy-1,anox,2,"specific_kaiten3");
      ret.push_back({anoy-1,anox,2});
      anoy--;
    }
  }else{
    if(x+1==anox&&y+1==anoy){
      safe_kaiten(field,anoy-1,anox,2,"specific_kaiten4");
      ret.push_back({anoy-1,anox,2});
      for(int i=0;i<3;i++){
        safe_kaiten(field,y,x,2,"specific_kaiten5");
        ret.push_back({y,x,2});
      }
    }else if(anoy==size-1){
      for(int i=0;i<2;i++){
        safe_kaiten(field,anoy-1,anox-1,2,"specific_kaiten6");
        ret.push_back({anoy-1,anox-1,2});
      }
      anoy--;
      anox--;
      int diffx=anox-x;
      while(diffx--){
        for(int i=0;i<3;i++){
          safe_kaiten(field,anoy,anox-1,2,"specific_kaiten7");
          ret.push_back({anoy,anox-1,2});
        }
        anox--;
      }
    }else{
      int diffx=anox-x-1;
      while(diffx--){
        cout<<"[DEBUG specific_kaiten] anoy="<<anoy<<" anox="<<anox<<" y="<<y<<" x="<<x<<" diffx="<<diffx<<endl;
        safe_kaiten(field,anoy-1,anox-1,2,"specific_kaiten8");
        ret.push_back({anoy-1,anox-1,2});
        anox--;
      }
      if(y<anoy){
        for(int i=0;i<3;i++){
          safe_kaiten(field,anoy,anox,2,"specific_kaiten9");
          ret.push_back({anoy,anox,2});
        }
        for(int i=0;i<3;i++){
          safe_kaiten(field,y,x,2,"specific_kaiten10");
          ret.push_back({y,x,2});
        }
      }else{
        safe_kaiten(field,anoy-1,anox,2,"specific_kaiten11");
        ret.push_back({anoy-1,anox,2});
        for(int i=0;i<3;i++){
          safe_kaiten(field,y,x,2,"specific_kaiten12");
          ret.push_back({y,x,2});
        }
      }
    }
  }
  return ret;
}

// main_solve
vector<tpi>solve(vvi&field,int size){
  vector<tpi>ans,ans1,ans2,ans3;
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
      tie(y,x)=find_pos(field,i,j,size);
      auto v=specific_kaiten(field,size,i,j,y,x);
      ans2.insert(ans2.end(),v.begin(),v.end());
    }
  }

  ans3=main_bfs(field,size);
  ans.insert(ans.end(),ans1.begin(),ans1.end());
  ans.insert(ans.end(),ans2.begin(),ans2.end());
  for(auto&g:ans3){
    int y,x,l;
    tie(l,y,x)=g;
    int ny=size-4+y;
    int nx=size-4+x;
    safe_kaiten(field,ny,nx,l,"solve main_bfs");
    ans.insert(ans.end(),make_tuple(ny,nx,l));
  }
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
