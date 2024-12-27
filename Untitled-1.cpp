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

const int N=1000000;
ll tree[4*N];
ll arr[N];


void build(ll node,ll st,ll en)
{
    if(st==en)
    {
        tree[node]=arr[st];
        return;
    }
    ll mid=(st+en)/2;
    build(2*node,st,mid);
    build(2*node+1,mid+1,en);
}

void update(ll node,ll st,ll en,ll in,ll x)
{
    if(st==en)
    {
        tree[node]=x;
        arr[st]=x;
        return;
    }
    ll mid=(st+en)/2;
    if(in<=mid)
    {
        update(2*node,st,mid,in,x);
    }
    else update(2*node+1,mid+1,en,in,x);
}

ll query(ll node,ll st,ll en,ll l,ll r)
{
    if(st>r or en<l)
    {
        return INT_MAX;
    }
    if(st<=l and en>=r)
    {
        return arr[st];
    }
    ll mid=(st+en)/2;
    ll left_query=query(2*node,st,mid,l,r);
    ll right_query=query(2*node+1,mid+1,en,l,r);
    return min(left_query, right_query);
}

void solve()
{
    ll n,i,m;
    cin>>n>>m;
    for(i=1;i<=n;i++)
    {
        cin>>arr[i];
    }
    build(1,1,n);
    while(m--)
    {
        ll x,l,r;
        cin>>x>>l>>r;
        if(x==1)
        {
            update(1,1,n,l+1,r);
        }
        else
        {
            ll ans=query(1,1,n,l+1,r);
            cout<<ans<<endl;
        }
    }

}

int main()
{
ios_base::sync_with_stdio(false);
cin.tie(nullptr);

solve();
}