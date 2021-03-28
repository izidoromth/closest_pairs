#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>  
#include <string>
#include <sstream>
#include <math.h>
#include <time.h>
#include <bits/stdc++.h>

using namespace std;

class Ponto
{
    public:
    double x, y;
};

Ponto* ObterPares(char* nome_do_arquivo, int* num_pontos);
void ResolverForcaBruta(Ponto pontos[], int num_pontos);
void ForcaBruta(Ponto pontos[], int e, int d, Ponto& p1, Ponto& p2, double *distancia);
void ResolverDivisaoConquista(Ponto pontos[], int num_pontos);
double DivisaoConquista(Ponto px[], Ponto py[], int e, int d, Ponto& p1, Ponto& p2);
void DivideY(Ponto py[], Ponto px[], Ponto pe[], Ponto pd[], int e, int d, int p_e);
void MontaY2Delta(Ponto pontos[], Ponto y[], int e, int d, int meio, double delta);
void MergeSort(Ponto pontos[], int l, int r, bool order_x);
void Merge(Ponto pontos[], int l, int m, int r, bool order_x);
double Menor(double a, double b);

int main(int argc, char* argv[])    
{    
    if(!argv[0])
    {
        cout << "Eh preciso informar um arquivo de entrada" << endl;
        return -1;
    }
    int num_pontos = 0;
    Ponto* pontos = ObterPares(argv[1], &num_pontos);  
    ResolverForcaBruta(pontos, num_pontos);
    ResolverDivisaoConquista(pontos, num_pontos);
    return 0;    
}

Ponto* ObterPares(char* nome_do_arquivo, int* num_pontos)
{
    ifstream arquivo;
    Ponto* pontos;
    string line;
    arquivo.open(nome_do_arquivo);
    if(!arquivo)
    {
        cout << "Arquivo nao encontrado!" << endl;
    }
    else
    {
        getline(arquivo, line);
        *num_pontos = stoi(line);
        pontos = new Ponto[*num_pontos];
        int i = 0;
        while(i < *num_pontos)
        {
            getline(arquivo, line);  
            Ponto ponto;   
            stringstream ss(line);
            string valor;
            getline(ss, valor, ' ');
            ponto.x = stod(valor);
            getline(ss, valor, ' ');
            ponto.y = stod(valor);
            pontos[i] = ponto;
            i++;
        }
    }
    return pontos;
}

void ResolverForcaBruta(Ponto pontos[], int num_pontos)
{
    Ponto p1, p2;
    double distancia = 4297967296;
    clock_t t;
    
    t = clock();

    ForcaBruta(pontos, 0, num_pontos-1, p1, p2, &distancia);

    t = clock() - t;

    cout << fixed << setprecision(6);
    cout << t*1.0/CLOCKS_PER_SEC << ' ' << distancia << ' ' << p1.x << ' ' << p1.y << ' ' << p2.x << ' ' << p2.y << ' ';
}

void ForcaBruta(Ponto pontos[], int e, int d, Ponto& p1, Ponto& p2, double *distancia)
{
    for(int i=e; i<=d; i++)
    {
        for(int j=i+1; j<=d; j++)
        {
            double dx = pontos[i].x-pontos[j].x;
            double dy = pontos[i].y-pontos[j].y;

            double dist = sqrt(pow(dx,2) + pow(dy,2));

            if(dist < *distancia)
            {
                *distancia = dist;
                p1.x = pontos[i].x;
                p1.y = pontos[i].y;
                p2.x = pontos[j].x;
                p2.y = pontos[j].y;
            }
        }
    }
}

void ResolverDivisaoConquista(Ponto pontos[], int num_pontos)
{
    Ponto p1, p2;

    Ponto* px = new Ponto[num_pontos];
    Ponto* py = new Ponto[num_pontos];

    memcpy(px, pontos, num_pontos*sizeof(Ponto));
    memcpy(py, pontos, num_pontos*sizeof(Ponto));

    MergeSort(px, 0, num_pontos-1, true);
    MergeSort(py, 0, num_pontos-1, false);

    double distancia = 4297967296;

    clock_t t;

    t = clock();

    distancia = DivisaoConquista(px, py, 0, num_pontos-1, p1, p2);

    t = clock() - t;

    cout << fixed << setprecision(6);
    cout << t*1.0/CLOCKS_PER_SEC << ' ' << distancia << ' ' << p1.x << ' ' << p1.y << ' ' << p2.x << ' ' << p2.y;

    delete px;
    delete py;
}

double DivisaoConquista(Ponto px[], Ponto py[], int e, int d, Ponto& p1, Ponto& p2)
{
    int num_pontos = d - e + 1;
    double distancia = 4297967296;

    if(num_pontos <= 3)
    {
        ForcaBruta(px, e, d, p1, p2, &distancia);
        return distancia;
    }

    Ponto pd1, pd2, pe1, pe2;

    int pe = (int)num_pontos%2 == 0 ? (int)(num_pontos/2-1) + e : (int)(num_pontos/2) + e;
    int pd = pe + 1;

    Ponto pye[pe-e+1];
    Ponto pyd[d-pd+1];    

    DivideY(py, px, pye, pyd, e, d, pe);

    double de = DivisaoConquista(px, pye, e, pe, pe1, pe2);
    double dd = DivisaoConquista(px, pyd, pd, d, pd1, pd2);

    double delta = Menor(de, dd);

    Ponto* yd = new Ponto[8];

    MontaY2Delta(py, yd, e, d, pe, delta);

    int k;
    for(k=0; k<8; k++)
    {
        if(yd[k].x == 0 && yd[k].y == 0)
            break;
    }

    double ded = 4297967296;
    Ponto p1er, p2er;

    for(int z=0; z<k; z++)
    {
        for(int x=z+1; x<k; x++)
        {
            double dx = yd[z].x-yd[x].x;
            double dy = yd[z].y-yd[x].y;

            double dist = sqrt(pow(dx,2) + pow(dy,2));

            if(dist < ded)
            {
                ded = dist;
                p1er.x = yd[z].x;
                p1er.y = yd[z].y;
                p2er.x = yd[x].x;
                p2er.y = yd[x].y;
            }
        }
    }

    distancia = Menor(delta, ded);

    if(distancia == de)
    {
        p1.x = pe1.x;
        p1.y = pe1.y;
        p2.x = pe2.x;
        p2.y = pe2.y;
    }
    else if(distancia == ded)
    {
        p1.x = p1er.x;
        p1.y = p1er.y;
        p2.x = p2er.x;
        p2.y = p2er.y;
    }
    else
    {
        p1.x = pd1.x;
        p1.y = pd1.y;
        p2.x = pd2.x;
        p2.y = pd2.y;
    }

    delete yd;

    return distancia;
}

void DivideY(Ponto py[], Ponto px[], Ponto pe[], Ponto pd[], int e, int d, int p_e)
{
    int ei = 0, di = 0;
    for(int i=e; i<=d; i++)
    {
        if(py[i].x <= px[p_e].x)
        {
            pe[ei].x = py[i].x;
            pe[ei].y = py[i].y;
            ei++;
        }
        else
        {
            pd[di].x = py[i].x;
            pd[di].y = py[i].y;
            di++;
        }
    }
}

void MontaY2Delta(Ponto pontos[], Ponto y[], int e, int d, int meio, double delta)
{
    int iy = 0;
    for(int i = 0; i < 8; i++)
    {
        y[i].x = 0;
        y[i].y = 0;
    }
    for(int i=e; i<=d; i++)
    {
        if(fabs(pontos[i].x-pontos[meio].x) < delta)
        {
            y[iy].x = pontos[i].x;
            y[iy].y = pontos[i].y;
            iy++;
        }
    }
}

void MergeSort(Ponto pontos[],int l,int r, bool order_x)
{
    if(l>=r){
        return;//returns recursively
    }
    int m = l+(r-l)/2;
    MergeSort(pontos,l,m,order_x);
    MergeSort(pontos,m+1,r,order_x);
    Merge(pontos,l,m,r,order_x);
}

void Merge(Ponto pontos[], int l, int m, int r, bool order_x)
{
    int n1 = m - l + 1;
    int n2 = r - m;
 
    // Create temp arrays
    Ponto L[n1], R[n2];
 
    // Copy data to temp arrays L[] and R[]
    for (int i = 0; i < n1; i++)
        L[i] = pontos[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = pontos[m + 1 + j];
 
    // Merge the temp arrays back into arr[l..r]
 
    // Initial index of first subarray
    int i = 0;
 
    // Initial index of second subarray
    int j = 0;
 
    // Initial index of merged subarray
    int k = l;
 
    if(order_x)
    {
        while (i < n1 && j < n2) {
            if (L[i].x <= R[j].x) {
                pontos[k] = L[i];
                i++;
            }
            else {
                pontos[k] = R[j];
                j++;
            }
            k++;
        }
    }
    else
    {
        while (i < n1 && j < n2) {
            if (L[i].y <= R[j].y) {
                pontos[k] = L[i];
                i++;
            }
            else {
                pontos[k] = R[j];
                j++;
            }
            k++;
        }        
    }   
 
    // Copy the remaining elements of
    // L[], if there are any
    while (i < n1) {
        pontos[k] = L[i];
        i++;
        k++;
    }
 
    // Copy the remaining elements of
    // R[], if there are any
    while (j < n2) {
        pontos[k] = R[j];
        j++;
        k++;
    }
}

double Menor(double a, double b)
{
    return (a <= b) ? a : b;
}