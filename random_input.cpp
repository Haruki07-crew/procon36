#include <bits/stdc++.h>
using namespace std;

int main(){
    int n;
    cin >> n;               // サイズ指定（偶数前提）

    int m = n * n / 2;
    vector<int> a;

    for(int i = 1; i <= m; i++){
        a.push_back(i);
        a.push_back(i);
    }

    random_device rd;
    mt19937 rng(rd());
    shuffle(a.begin(), a.end(), rng);

    int idx = 0;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cout << a[idx++] << " ";
        }
        cout << '\n';
    }
}
