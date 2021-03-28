#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>  
#include <string>
#include <sstream>
#include <math.h>
#include <time.h>
#include <string.h>

using namespace std;

class Ponto
{
    public:
    double x, y;
};

Ponto* ObterPares(char* nome_do_arquivo, int& num_pontos);
void ResolverForcaBruta(Ponto pontos[], int num_pontos);
void ForcaBruta(Ponto pontos[], int e, int d, Ponto& p1, Ponto& p2, double *distancia);
void ResolverDivisaoConquista(Ponto pontos[], int num_pontos);
double DivisaoConquista(Ponto px[], Ponto py[], int e, int d, Ponto& p1, Ponto& p2);
Ponto* DivideY(Ponto py[], Ponto px[], bool esq, int num_pontos, int tam, double meio);
Ponto* MontaY2Delta(Ponto pontos[], int num_pontos, double pxe, double delta);
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
    Ponto* pontos = ObterPares(argv[1], num_pontos);  
    ResolverForcaBruta(pontos, num_pontos);
    ResolverDivisaoConquista(pontos, num_pontos);
    return 0;    
}

Ponto* ObterPares(char* nome_do_arquivo, int& num_pontos)
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
        num_pontos = stoi(line);
        pontos = (Ponto*)malloc(num_pontos*sizeof(Ponto));
        int i = 0;
        while(i < num_pontos)
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

    Ponto* px = (Ponto*)malloc(num_pontos*sizeof(Ponto));
    Ponto* py = (Ponto*)malloc(num_pontos*sizeof(Ponto));

    for(int i = 0; i < num_pontos; i++)
    {
        px[i].x = pontos[i].x;
        px[i].y = pontos[i].y;
        py[i].x = pontos[i].x;
        py[i].y = pontos[i].y;
    }

    MergeSort(px, 0, num_pontos-1, true);
    MergeSort(py, 0, num_pontos-1, false);

    double distancia = 4297967296;

    clock_t t;

    t = clock();

    distancia = DivisaoConquista(px, py, 0, num_pontos-1, p1, p2);

    t = clock() - t;

    cout << fixed << setprecision(6);
    cout << t*1.0/CLOCKS_PER_SEC << ' ' << distancia << ' ' << p1.x << ' ' << p1.y << ' ' << p2.x << ' ' << p2.y;

    free(px);
    free(py);
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

    int pe = num_pontos%2 == 0 ? (int)(num_pontos/2 - 1 + e) : (int)(num_pontos/2 + e);
    int pd = pe + 1;

    Ponto* pye = DivideY(py, px, true, num_pontos, pe-e+1, px[pe].x);
    Ponto* pyd = DivideY(py, px, false, num_pontos, d-pd+1, px[pe].x);

    double de = DivisaoConquista(px, pye, e, pe, pe1, pe2);
    double dd = DivisaoConquista(px, pyd, pd, d, pd1, pd2);

    double delta = Menor(de, dd);
    
    Ponto* yd = MontaY2Delta(py, num_pontos, px[pe].x, delta);

    int k;
    for(k=0; k<8; k++)
    {
        if(yd[k].x == 0 && yd[k].y == 0)
            break;
    }

    double ded = delta;
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
    else if(distancia == dd)
    {
        p1.x = pd1.x;
        p1.y = pd1.y;
        p2.x = pd2.x;
        p2.y = pd2.y;
    }
    else
    {
        p1.x = p1er.x;
        p1.y = p1er.y;
        p2.x = p2er.x;
        p2.y = p2er.y;
    }  

    //free(pye);
    //free(pyd);
    free(yd);

    return distancia;
}

Ponto* DivideY(Ponto py[], Ponto px[], bool esq, int num_pontos, int tam, double meio)
{
    Ponto* y = (Ponto*)malloc(num_pontos*sizeof(Ponto));
    int j = 0;
    if(esq)
    {
        for(int i=0; i<num_pontos; i++)
        {
            if(py[i].x <= meio)
            {
                y[j].x = py[i].x;
                y[j].y = py[i].y;
                j++;
            }
        }
    }
    else
    {        
        for(int i=0; i<num_pontos; i++)
        {
            if(py[i].x > meio)
            {
                y[j].x = py[i].x;
                y[j].y = py[i].y;
                j++;
            }
        }
    }
    return y;
}

Ponto* MontaY2Delta(Ponto pontos[], int num_pontos, double pxe, double delta)
{
    Ponto* y2delta = (Ponto*)malloc(8*sizeof(Ponto));
    int i;
    for(i = 0; i < 8; i++)
    {
        y2delta[i].x = 0;
        y2delta[i].y = 0;
    }
    i = 0;  
    for(int j=0; j<num_pontos; j++)
    {
        if(fabs(pontos[j].x-pxe) < delta)
        {
            y2delta[i].x = pontos[j].x;
            y2delta[i].y = pontos[j].y;
            i++;
        }
    }
    return y2delta;
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