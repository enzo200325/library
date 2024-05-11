#include <bits/stdc++.h>
using namespace std; 
#define endl '\n'

//debug
void __print(int x) {cerr << x;}
void __print(long x) {cerr << x;}
void __print(long long x) {cerr << x;}
void __print(unsigned x) {cerr << x;}
void __print(unsigned long x) {cerr << x;}
void __print(unsigned long long x) {cerr << x;}
void __print(float x) {cerr << x;}
void __print(double x) {cerr << x;}
void __print(long double x) {cerr << x;}
void __print(char x) {cerr << '\'' << x << '\'';}
void __print(const char *x) {cerr << '\"' << x << '\"';}
void __print(const string &x) {cerr << '\"' << x << '\"';}
void __print(bool x) {cerr << (x ? "true" : "false");}
template<typename T, typename V>
void __print(const pair<T, V> &x) {cerr << '{'; __print(x.first); cerr << ','; __print(x.second); cerr << '}';}
template<typename T>
void __print(const T &x) {int f = 0; cerr << '{'; for (auto &i: x) cerr << (f++ ? "," : ""), __print(i); cerr << "}";}
void _print() {cerr << "]\n";}
template <typename T, typename... V>
void _print(T t, V... v) {__print(t); if (sizeof...(v)) cerr << ", "; _print(v...);}
#ifndef ONLINE_JUDGE
#define debug(x...) cerr << "[" << #x << "] = ["; _print(x)
#else
#define debug(x...)
#endif
#ifndef ONLINE_JUDGE
#define debarr(arr, n) cerr << "arr: ["; forn(i, n) cerr << arr[i] << (i == n-1 ? "" : ", "); cerr << "]" << endl;
#else
#define debarr(x...)
#endif

// -------------------- = -----------------------

typedef long long ll; 
using ld = long double;

#define bg(x) begin(x)
#define all(x) bg(x), end(x)
#define rall(x) x.rbegin(), x.rend()
#define sor(x) sort(all(x))
#define rev(x) reverse(all(x))

#define forn(i, n) for (int i = 0; i < n; i++)
#define fora(i, a, b) for (int i = a; i <= b; i++) 
#define rofn(i, n) for (int i = n-1; i >= 0; i--) 
#define rofa(i, a, b) for (int i = a; i >= b; i--)

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

struct custom_hash {
    static uint64_t splitmix64(uint64_t x) {
        // http://xorshift.di.unimi.it/splitmix64.c
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }

    size_t operator()(uint64_t x) const {
        static const uint64_t FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(x + FIXED_RANDOM);
    }
};

// ====================== end of template ===========================

const int maxn = 2e5 + 50; 

struct st {
    ll seg[4*maxn]; 
    int n; 
    st() {} 

    void build_aux(int u, int l, int r, int* a, int* ord) {
        if (l == r) {
            seg[u] = a[ord[l]]; 
            return; 
        } 
        int mid = l + r >> 1; 
        build_aux(u<<1, l, mid, a, ord); 
        build_aux(u<<1|1, mid+1, r, a, ord); 
        seg[u] = seg[u<<1] + seg[u<<1|1]; 
    } 
    void build(int _n, int* a, int* ord) {
        n = _n; 
        build_aux(1, 0, n-1, a, ord); 
    } 

    ll query(int u, int l, int r, int L, int R) {
        if (l > R || r < L) return 0; 
        if (l >= L && r <= R) return seg[u]; 
        int mid = l + r >> 1; 
        return query(u<<1, l, mid, L, R) + 
            query(u<<1|1, mid+1, r, L, R); 
    } 
    ll query(int l, int r) { return query(1, 0, n-1, l, r); } 

    void update(int u, int l, int r, int i, int val) {
        if (l == r) {
            seg[u] = val; 
            return; 
        } 
        int mid = l + r >> 1; 
        if (i <= mid) update(u<<1, l, mid, i, val); 
        else update(u<<1|1, mid+1, r, i, val); 
        seg[u] = seg[u<<1] + seg[u<<1|1]; 
    } 
    void update(int i, int val) { update(1, 0, n-1, i, val); }
} seg; 

int a[maxn]; 
vector<int> adj[maxn]; 

struct hld {
    int tin[maxn], tout[maxn], par[maxn], head[maxn], eul[maxn], sz[maxn], T; 

    hld() {}
    void dfs(int u, int p = -1) {
        sz[u] = 1; 
        par[u] = p; 
        if (par[u] > -1) adj[u].erase(find(adj[u].begin(), adj[u].end(), par[u])); 
        for (int& v : adj[u]) {
            dfs(v, u); 
            sz[u] += sz[v]; 
            if (sz[v] > sz[adj[u][0]]) swap(v, adj[u][0]); 
        } 
    } 
    void dfs_hld(int u) {
        tin[u] = T++; 
        eul[tin[u]] = u; 
        for (int v : adj[u]) {
            head[v] = v == adj[u][0] ? head[u] : v; 
            dfs_hld(v); 
        } 
        tout[u] = T; 
    } 

    void build() {
        dfs(0); 
        dfs_hld(0); 
        seg.build(T, a, eul); 
    } 

    int lca(int u, int v) {
        while (head[u] != head[v]) {
            if (tin[u] < tin[v]) swap(u, v); 
            u = par[head[u]]; 
        } 
        if (tin[u] < tin[v]) swap(u, v); 
        return u; 
    } 

    ll query(int u, int v) {
        ll uans = 0, vans = 0; 
        while (head[u] != head[v]) {
            if (tin[u] > tin[v]) {
                uans += seg.query(tin[head[u]], tin[u]); 
                u = par[head[u]]; 
            } 
            else {
                vans += seg.query(tin[head[v]], tin[v]); 
                v = par[head[v]]; 
            } 
        } 
        if (tin[u] < tin[v]) vans += seg.query(tin[u], tin[v]); 
        else uans += seg.query(tin[v], tin[u]); 

        return uans + vans; 
    } 

    void update(int u, int val) {
        seg.update(tin[u], val); 
    } 
} hld;  

int main() {
    ios_base::sync_with_stdio(0); 
    cin.tie(0); 

    int n, q; cin >> n >> q; 
    for (int i = 0; i < n; i++) cin >> a[i]; 
    for (int i = 0; i < n-1; i++) {
        int u, v; cin >> u >> v; u--; v--;  
        adj[u].emplace_back(v); 
        adj[v].emplace_back(u); 
    } 
    hld.build(); 

    while (q--) {
        int id; cin >> id; 
        if (id == 1) {
            int u, x; cin >> u >> x; u--; 
            hld.update(u, x); 
        } 
        else {
            int u; cin >> u; u--; 
            cout << hld.query(u, 0) << endl;
        } 
    } 
}
// https://cses.fi/problemset/task/1138
