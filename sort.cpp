void sort(double* a, double* b, double* c, double* F, int n){
    for(int i=0; i<n-1; i++){
        for(int j=i+1;j<n;j++){
            if(F[i]>F[j]){
                double tmpF = F[i];
                F[i]=F[j];
                F[j]=tmpF;
                double tmpA = a[i];
                a[i]=a[j];
                a[j]=tmpA;
                double tmpB = b[i];
                b[i]=b[j];
                b[j]=tmpB;
                double tmpC = c[i];
                c[i]=c[j];
                c[j]=tmpC;
            }
        }
    }
}
