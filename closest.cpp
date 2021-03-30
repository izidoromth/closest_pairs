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
    Ponto(){ x = 0.0; y = 0.0; }
    ~Ponto(){}
};

Ponto* ObterPares(char* nome_do_arquivo, int& num_pontos);
double DistanciaEuclidiana(Ponto p1, Ponto p2);
void ResolverForcaBruta(Ponto* pontos, int num_pontos);
double ForcaBruta(Ponto* pontos, int e, int d, Ponto& p1, Ponto& p2);
void ResolverDivisaoConquista(Ponto* pontos, int num_pontos);
double DivisaoConquista(Ponto* p, int e, int d, Ponto& p1, Ponto& p2);
double DistanciaEsquerdaDireita(Ponto p[], int n, double delta, Ponto& p1, Ponto& p2);
void MergeSort(Ponto* pontos, int l, int r, bool order_x);
void Merge(Ponto* pontos, int l, int m, int r, bool order_x);

int CompararX(const void* a, const void* b)
{
    Ponto *p1 = (Ponto *)a, *p2 = (Ponto *)b;
    return (p1->x - p2->x);
}

int CompararY(const void* a, const void* b)
{
    Ponto *p1 = (Ponto *)a, *p2 = (Ponto *)b;
    return (p1->y - p2->y);
}

int main(int argc, char* argv[])    
{    
    if(!argv[0])
    {
        cout << "Informe um arquivo de entrada" << endl;
        return -1;
    }
    int num_pontos = 0;
    Ponto* pontos = ObterPares(argv[1], num_pontos);
    if(!pontos)
    {
        cout << "Ocorreu algo errado ao obter os pontos de entrada" << endl;
        return -1;
    }
    ResolverForcaBruta(pontos, num_pontos);
    ResolverDivisaoConquista(pontos, num_pontos);
    return 0;    
}

Ponto* ObterPares(char* nome_do_arquivo, int& num_pontos)
{
    ifstream arquivo;
    string line;
    Ponto* pontos = NULL;
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
            stringstream ss(line);
            string valor;
            getline(ss, valor, ' ');
            pontos[i].x = stod(valor);
            getline(ss, valor, ' ');
            pontos[i].y = stod(valor);
            i++;
        }
    }
    return pontos;
}


double DistanciaEuclidiana(Ponto p1, Ponto p2)
{
    double dx = p1.x-p2.x;
    double dy = p1.y-p2.y;

    return (double)sqrt(pow(dx,2) + pow(dy,2));
}

void ResolverForcaBruta(Ponto* pontos, int num_pontos)
{
    Ponto p1, p2;

    p1.x = 9999999.0;
    p1.y = 9999999.0;

    p2.x = 0.0;
    p2.y = 0.0;

    clock_t t;
    
    t = clock();

    double distancia = ForcaBruta(pontos, 0, num_pontos-1, p1, p2);

    t = clock() - t;

    cout << fixed << setprecision(6);
    cout << t*1.0/CLOCKS_PER_SEC << ' ' << distancia << ' ' << p1.x << ' ' << p1.y << ' ' << p2.x << ' ' << p2.y << ' ';
}

double ForcaBruta(Ponto* pontos, int e, int d, Ponto& p1, Ponto& p2)
{
    double distancia = DistanciaEuclidiana(p1, p2);
    for(int i=e; i<=d-1; i++)
    {
        for(int j=i+1; j<=d; j++)
        {
            if(DistanciaEuclidiana(pontos[i], pontos[j]) < distancia)
            {
                distancia = DistanciaEuclidiana(pontos[i], pontos[j]);
                p1.x = pontos[i].x;
                p1.y = pontos[i].y;
                p2.x = pontos[j].x;
                p2.y = pontos[j].y;
            }
        }
    }
    return distancia;
}

void ResolverDivisaoConquista(Ponto* pontos, int num_pontos)
{
    Ponto p1, p2;

    p1.x = 9999999.0;
    p1.y = 9999999.0;

    p2.x = 0.0;
    p2.y = 0.0;

    MergeSort(pontos, 0, num_pontos-1, true);

    clock_t t;

    t = clock();

    double distancia = DivisaoConquista(pontos, 0, num_pontos-1, p1, p2);

    t = clock() - t;

    cout << fixed << setprecision(6);
    cout << t*1.0/CLOCKS_PER_SEC << ' ' << distancia << ' ' << p1.x << ' ' << p1.y << ' ' << p2.x << ' ' << p2.y << endl;
}

double DivisaoConquista(Ponto* p, int e, int d, Ponto& p1, Ponto& p2)
{
    int num_pontos = d-e+1;

    if(num_pontos <= 3)
    {
        return ForcaBruta(p, e, d, p1, p2);
    }

    int meio = (d+e)/2;

    double de = DivisaoConquista(p, e, meio, p1, p2);
    double dd = DivisaoConquista(p, meio+1, d, p1, p2);

    double delta = min(de, dd);

    Ponto pontos_delta[num_pontos];
    
    int idelta = 0;
    for (int i = e; i < d; i++) 
    {
        if (fabsf(p[i].x - p[meio].x) < delta) 
        {
            pontos_delta[idelta] = p[i];
            idelta++;
        }
    } 

    MergeSort(pontos_delta, 0, idelta-1, false);

    double ded = DistanciaEsquerdaDireita(pontos_delta, idelta, delta, p1, p2);

    return min(delta, ded);
}

double DistanciaEsquerdaDireita(Ponto p[], int n, double delta, Ponto& p1, Ponto& p2)
{
    double ded = delta;

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n && (p[j].y - p[i].y) < ded; j++) 
        {
            if (DistanciaEuclidiana(p[i],p[j]) < ded) 
            {
                ded = DistanciaEuclidiana(p[i],p[j]);
                
                if(ded < DistanciaEuclidiana(p1, p2))
                { 
                    p1.x = p[i].x;
                    p1.y = p[i].y;
                    p2.x = p[j].x;
                    p2.y = p[j].y;
                }
            }
        }
    }
    return ded;
}

void MergeSort(Ponto* pontos,int l,int r, bool order_x)
{
    if(l>=r){
        return;
    }
    int m = l+(r-l)/2;
    MergeSort(pontos,l,m,order_x);
    MergeSort(pontos,m+1,r,order_x);
    Merge(pontos,l,m,r,order_x);
}

void Merge(Ponto* pontos, int l, int m, int r, bool order_x)
{
    int n1 = m - l + 1;
    int n2 = r - m;

    Ponto L[n1], R[n2]; 
    
    for (int i = 0; i < n1; i++)
        L[i] = pontos[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = pontos[m + 1 + j];

    int i = 0;
    int j = 0;
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

    while (i < n1) {
        pontos[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        pontos[k] = R[j];
        j++;
        k++;
    }
}