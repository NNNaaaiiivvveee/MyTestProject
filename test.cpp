#include<stdio.h>
#include<math.h>
#include<iostream>
#include<windows.h>
#define eps 1e-3
#define meps (-0.0000001)
using namespace std;
struct vec3
{
    double x,y,z;
    vec3 operator + (const vec3& b) const
    {
        vec3 ret;
        ret.x = x + b.x; ret.y = y + b.y; ret.z = z + b.z;
        return ret;
    }
    vec3 operator - (const vec3& b) const
    {
        vec3 ret;
        ret.x = x - b.x; ret.y = y - b.y; ret.z = z - b.z;
        return ret;
    }
    double operator * (const vec3& b)
    {
        return x*b.x+y*b.y+z*b.z;
    }
};
inline vec3 cheng(vec3 A,double B)
{
    vec3 ret;
    ret.x = A.x*B;
    ret.y = A.y*B;
    ret.z = A.z*B;
    return ret;
}
inline vec3 calc(vec3 in ,vec3 out ,vec3 center ,double R)
{
    double r2 = R * R;
    double l=0,r=1,mid,gap2;
    vec3 ret;
    vec3 io = out - in;
    vec3 constgap = in - center;
    vec3 gap;
    while(l<r)
    {
        mid = (l+r)/2;
        gap = constgap + cheng(io,mid);
        gap2 = gap*gap - r2;
        if(abs(gap2)<=eps) break;
        if(gap2<0){l=mid;}
        else{r=mid;}
    }
    ret = in + cheng(io,mid);
    return ret;
}
int main()
{
    //////////////////////////
    vec3 in,out,center;
    double R;
    scanf("%lf%lf%lf",&in.x,&in.y,&in.z);
    scanf("%lf%lf%lf",&out.x,&out.y,&out.z);
    scanf("%lf%lf%lf",&center.x,&center.y,&center.z);
    scanf("%lf",&R);
    int n; scanf("%d",&n);
    vec3 ans;
    DWORD st,en;
    LARGE_INTEGER lg_int;
    double dff;
    __int64 c1,c2;
    QueryPerformanceFrequency(&lg_int);
    dff = lg_int.QuadPart;
    QueryPerformanceCounter(&lg_int);
    c1 = lg_int.QuadPart;
    st = GetTickCount();
    for(int i=1;i<=n;i+=2) ans = calc(in,out,center,R),ans = calc(in,out,center,R);
    en = GetTickCount();
    QueryPerformanceCounter(&lg_int);
    c2 = lg_int.QuadPart;
    printf("lilun %lf shiji %lf %lld %d %lf",R*R,(ans-center)*(ans-center),c2-c1,
           en-st,(c2-c1)*1000/dff);
    return 0;
}
