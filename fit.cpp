#include <iostream>
#include <fstream>
#include <cmath>
#include "FitRoutine.h"
#include "sort.h"
using namespace std;
int main(){
    ifstream is;
    is.open("samp.csv");
    
    if(is.bad()){
        return -1;
    }
    double X,Y;
    int N = 0;
    is.ignore(256,'\n');
    while(!(is.fail())){
        is>>X>>Y;
        N++;
    }
    is.close(); is.clear();
    int n = N - 1;
    
    double* x = new double[n];
    double* y = new double[n];
    
    is.open("samp.csv");
    is.ignore(256,'\n');
    for(int j=0; j<n; j++){
        is>>x[j]>>y[j];
    }
    is.close(); is.clear();
    
    double a0,b0,c0;
    cout<<"Fit is designed to fit data into the function\n";
    cout<<"y=10**(a-b/(x+c))\n";
    cout<<"Enter the initial value of a,b,c\n";
    cin>>a0>>b0>>c0;
    double R=1; double K=0.5; double E=2; double S=0.5;
    double* a = new double[4];
    double* b = new double[4];
    double* c = new double[4];
    double* F = new double[4];
    double lambda;
    cout<<"Enter the lambda\n";
    cin>>lambda;
    double ao,bo,co;
    double ar,br,cr,Fr;
    double ae,be,ce,Fe;
    double aoc,boc,coc,Foc;
    double aic,bic,cic,Fic;
    a[0]=a0; b[0]=b0; c[0]=c0;
    a[1]=a0+lambda; b[1] = b0; c[1]=c0;
    a[2]=a0; b[2]=b[0]+lambda;c[2]=c0;
    a[3]=a0;b[3]=b[0];c[3]=c0+lambda;
    for(int i=0; i<4;i++){
        F[i]=FitDev(a[i],b[i],c[i],x,y,n);
    }
    sort(a,b,c,F,4);
    int itrMax;
    double tol;
    cout<<"Enter the maximum iteration number\n"; cin>>itrMax;
    cout<<"Enter the tolerance error\n"; cin>>tol;
    double resid;
    cout.precision(10);
    ofstream out;
    out.open("log.txt");
    out<<"#itr"<<'\t'<<'a'<<'\t'<<'b'<<'\t'<<'c'<<'\t'<<"resid"<<endl;
    for(int itr=1; itr<=itrMax; itr++){
        ao = (1./3)*(a[0]+a[1]+a[2]);
        bo = (1./3)*(b[0]+b[1]+b[2]);
        co = (1./3)*(c[0]+c[1]+c[2]);
        ar = ao + R*(ao-a[3]);
        br = bo + R*(bo-b[3]);
        cr = co + R*(co-c[3]);
        Fr = FitDev(ar,br,cr,x,y,n);
        //reflection
        if(F[0]<=Fr && Fr<F[2]){
            a[3] = ar;
            b[3] = br;
            c[3] = cr;
        }
        //expansion
        else if(Fr<F[0]){
            ae = ao + E*(ar-ao);
            be = bo + E*(br-bo);
            ce = co + E*(cr-co);
            Fe = FitDev(ae,be,ce,x,y,n);
            if(Fe<Fr){
                a[3]=ae;
                b[3]=be;
                c[3]=ce;
            }
            else
            {
                a[3]=ar;
                b[3]=br;
                c[3]=cr;
            }
        }
        //shrinkage
        else{
            if(Fr<F[3]){
                aoc = ao + K*(ar-ao);
                boc = bo + K*(br-bo);
                coc = co + K*(cr-co);
                Foc = FitDev(aoc,boc,coc,x,y,n);
                if(Foc<Fr){
                    a[3]=aoc; b[3]=boc; c[3]=coc;
                }
                else{
                    for(int i=1; i<4; i++){
                        a[i] = a[i] + S*(a[i]-a[0]);
                        b[i] = b[i] + S*(b[i]-b[0]);
                        c[i] = c[i] + S*(c[i]-c[0]);
                    }
                }
            }
            else{
                aic = ao - K*(ao-a[3]);
                bic = bo - K*(bo-b[3]);
                cic = co - K*(co-c[3]);
                Fic = FitDev(aic,bic,cic,x,y,n);
                if(Fic<Fr){
                    a[3]=aic; b[3]=bic; c[3]=cic;
                }
                else{
                    for(int i=1; i<4; i++){
                        a[i] = a[i] + S*(a[i]-a[0]);
                        b[i] = b[i] + S*(b[i]-b[0]);
                        c[i] = c[i] + S*(c[i]-c[0]);
                    }
                }
            }
        }
        for(int i=0; i<4; i++){
            F[i]=FitDev(a[i],b[i],c[i],x,y,n);
        }
        sort(a,b,c,F,4);
        resid = (a[0]-a[3])*(a[0]-a[3])+(b[0]-b[3])*(b[0]-b[3])+(c[0]-c[3])*(c[0]-c[3]);
        resid = sqrt(resid);
        cout<<"itr: "<<itr<<", a="<<a[0]<<", b="<<b[0]<<", c="<<c[0]<<", resid="<<resid<<endl;
        out<<itr<<'\t'<<a[0]<<'\t'<<b[0]<<'\t'<<c[0]<<'\t'<<resid<<endl;
        if(resid<=tol){
            cout<<"Convergence succeeded\n";
            break;
        }
    }
    out.close();
    double mean = 0;
    for(int i=0; i<n; i++){
        mean += y[i];
    }
    mean /= n;
    double SSTot = 0;
    for(int i=0; i<n; i++){
        SSTot += (y[i]-mean)*(y[i]-mean);
    }
    double SSRes = FitDev(a[0],b[0],c[0],x,y,n);
    double R2 = 1- (SSRes/SSTot);
    cout<<"R2="<<R2<<endl;
    return 0;
}
