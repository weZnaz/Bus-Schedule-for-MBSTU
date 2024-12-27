#include<bits/stdc++.h>
#define ll long long int
#define pb push_back
#define ld long double
#define cy cout<<"YES\n"
#define cn cout<<"NO\n"
#define vc vector
#define srt(v) sort(v.begin(),v.end())
#define rsrt(v) sort(v.rbegin(),v.rend())
#define all(v) v.begin(),v.end()
#define lwb lower_bound
#define upb upper_bound
#define pq priority_queue
#define uos unordered_set
#define mod 1000000007
#define stp setprecision
#define endl "\n"
#define input0(v,n) for(ll x=0;x<n;x++)cin>>v[x];
#define input1(v,n) for(ll x=1;x<=n;x++)cin>>v[x];
using namespace std;
//#include <ext/pb_ds/assoc_container.hpp>
//#include <ext/pb_ds/tree_policy.hpp>
//typedef __gnu_pbds::tree<int, __gnu_pbds::null_type, less<int>, __gnu_pbds::rb_tree_tag, __gnu_pbds::tree_order_statistics_node_update> ordered_set;

void solve()
{
    ll i,n,m;
    cin>>n>>m;
    vc<ll>v(n);
    input0(v,n);
    ll cur=0,ans=0;
    for(i=0;i<n;i++)
    {
        cur=max(cur+v[i],v[i]);
        ans=max(ans,cur);
    }
    cout<<ans<<endl;
}

int main()
{
ios_base::sync_with_stdio(false);
cin.tie(nullptr);

solve();
}