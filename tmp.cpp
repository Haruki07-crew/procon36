#include<bits/stdc++.h>
using namespace std;
using vvi = vector<vector<int>>;
using vi = vector<int>;
using tpi = tuple<int,int,int>;
int dd[5]={1,0,-1,0,1};
//同じ数の位置を探す
pair<int,int>find_pos(vvi&field,int x,int y,int size){
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            if(i!=x&&j!=y&&field[x][y]==field[i][j]){
                return make_pair(i,j);
            }
        }
    }
}
//4*4のBFS用
vvi kaiten(vvi&field,int y,int x,int size){
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
                int ny=i+dd[d];
                int nx=j+dd[d+1];
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
                    vvi nex=kaiten(cur,i,j,sizer);
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
void kaiten2(vvi&field,int x,int y,int side_len){
    //side_lenは回転するブロックの辺の長さで今は2のみ
    vvi tmp(side_len,vi(side_len));
    for(int i=0;i<side_len;i++){
        for(int j=0;j<side_len;j++){
            tmp[i][j]=field[x+i][y+j];
        }
    }
    for(int i=0;i<side_len;i++){
        for(int j=0;j<side_len;j++){
            field[y+j][x+(side_len-1-i)]=tmp[i][j];
        }
    }
}
vector<tpi>special_kaiten(vvi&field,int size,int x,int y,int diffx,int diffy){
    vector<tpi>ret;
    if(diffy>0){
        while(diffy--){
            ret.push_back({x,y,2});
            kaiten2(field,x,y,2);
        }
    }else if(diffy<0){
        diffy=abs(diffy);
        while(diffy--){
            ret.push_back({x-1,y-1,2});
            kaiten(field,x-1,y-1,2);
        }
    }
}

vector<tpi>normal_kaiten(vvi&field,int size,int x,int y,int diffx,int diffy){
    vector<tpi>ret;
    //列の移動
    int retuy=y;
    if(diffy>0){
        diffy++;
        while(diffy--){
            ret.push_back({x,y,2});
            kaiten(field,x,y,2);
            y++;
        }
    }else if(diffy<0){
        if(diffx==0){
            for(int i=0;i<3;i++){
                kaiten(field,x,y,2);
                ret.push_back({x,y,2});
            }
        }else{
            diffy=abs(diffy);
            while(diffy--){
                ret.push_back({x-1,y-1,2});
                kaiten(field,x-1,y-1,2);
                y--;
            }
        }
    }else{
        ret.push_back({x-1,y,2});
        kaiten(field,x-1,y,2);
    }
    //行の移動
    while(diffx--){
        ret.push_back({x-1,y,2});
        kaiten(field,x-1,y,2);
        x--;
    }
    return ret;
}
vector<tpi>solve(vvi&field,int size){
    vector<vector<tpi>>ans1;
    vector<tpi>ans,ans1,ans2,ans3;
    for(int i=0;i<size-4;i++){
        for(int j=0;j<size;j+=2){
            int x,y;
            tie(x,y)=find_pos(field,i,j,size);
            int diffx=i-x;
            int diffy=j-y;
            if(j==size-2){
                ans1.push_back(special_kaiten(field,size,i,j,diffx,diffy));
            }else{
                ans1.push_back(normal_kaiten(field,size,i,j,diffx,diffy));
            }
        }
    }
    for(int i=size-4;i<size;i++){
        for(int j=0;j<size-4;j+=2){

        }
    }
    ans2=main_bfs(field,size);
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
        int x,y,l;
        tie(x,y,l)=ans[i];
        cout<<"("<<x<<","<<y<<") "<<l<<endl;
    }
}
