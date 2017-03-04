#include <cmath>

double FitFunc(double a, double b, double c, double x){
    return pow(10.0,(a-b/(x+c)));
}

double FitDev(double a, double b, double c, double* x, double* y, int n){
    double sum = 0;
    for(int i=0; i<n; i++){
        sum += (y[i]-FitFunc(a,b,c,x[i]))*(y[i]-FitFunc(a,b,c,x[i]));
    }
    return sum;
}
