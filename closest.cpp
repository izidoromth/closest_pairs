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
double DivisaoConquista(Ponto px[], Ponto py[], int e, int d, Ponto& p1, Ponto& p2);
double DistanciaEsquerdaDireita(Ponto p[], int n, double delta, Ponto& p1, Ponto& p2);
void MergeSort(Ponto* pontos, int l, int r, bool order_x);
void Merge(Ponto* pontos, int l, int m, int r, bool order_x);

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
    clock_t t;
    
    t = clock();

    double distancia = ForcaBruta(pontos, 0, num_pontos-1, p1, p2);

    t = clock() - t;

    cout << fixed << setprecision(6);
    cout << t*1.0/CLOCKS_PER_SEC << ' ' << distancia << ' ' << p1.x << ' ' << p1.y << ' ' << p2.x << ' ' << p2.y << ' ';
}

double ForcaBruta(Ponto* pontos, int e, int d, Ponto& p1, Ponto& p2)
{
    double distancia = 9999999.0;
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

    Ponto px[num_pontos];
    Ponto py[num_pontos];

    for(int i=0; i<num_pontos;i++)
    {
        px[i].x = pontos[i].x;
        px[i].y = pontos[i].y;
        py[i].x = pontos[i].x;
        py[i].y = pontos[i].y;
    }

    MergeSort(px, 0, num_pontos-1, true);
    MergeSort(py, 0, num_pontos-1, false);

    clock_t t;

    t = clock();

    double distancia = DivisaoConquista(px, py, 0, num_pontos-1, p1, p2);

    t = clock() - t;

    cout << fixed << setprecision(6);
    cout << t*1.0/CLOCKS_PER_SEC << ' ' << distancia << ' ' << p1.x << ' ' << p1.y << ' ' << p2.x << ' ' << p2.y << endl;
}

double DivisaoConquista(Ponto px[], Ponto py[], int e, int d, Ponto& p1, Ponto& p2)
{
    int num_pontos = d-e+1;

    if(num_pontos <= 3)
    {
        return ForcaBruta(px, e, d, p1, p2);
    }

    int meio = (d-e)/2;

    Ponto pye[meio]; 
    Ponto pyd[num_pontos-meio];

    int ei = 0, di = 0; 

    for (int i = 0; i < num_pontos; i++)
    {
        if(py[i].x <= px[meio].x && ei < meio)
        {
            pye[ei++] = py[i];
        }
        else
        {
            pyd[di++] = py[i];
        }
    }

    double de = DivisaoConquista(px, py, e, meio+e, p1, p2);
    double dd = DivisaoConquista(px, py, meio+e+1, d, p1, p2);

    double delta = min(de, dd);

    Ponto pontos_delta[num_pontos];
    int idelta = 0;
    for (int i = 0; i < num_pontos; i++) 
    {
        if (fabsf(py[i].x - px[meio].x) < delta) 
        {
            pontos_delta[idelta] = py[i];
            idelta++;
        }
    } 

    return DistanciaEsquerdaDireita(pontos_delta, idelta, delta, p1, p2);;
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
                
                p1.x = p[i].x;
                p1.y = p[i].y;
                p2.x = p[j].x;
                p2.y = p[j].y;
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