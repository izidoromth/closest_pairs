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
double DivisaoConquista(Ponto* px, Ponto* py, int e, int d, Ponto& p1, Ponto& p2);
double DistanciaEsquerdaDireita(Ponto* p, int n, double delta, Ponto& p1, Ponto& p2);
int cmpx(const void *a, const void *b);
int cmpy(const void *a, const void *b);
void msort(Ponto** arr,int l,int r, int (*cmp)(const void*, const void*));
void merge(Ponto** arr, int l, int m, int r, int (*cmp)(const void*, const void*));

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

    Ponto* py = (Ponto*)malloc(num_pontos*sizeof(Ponto));

    for(int i = 0; i < num_pontos; i++)
    {
        py[i].x = pontos[i].x;
        py[i].y = pontos[i].y;
    }

    msort(&pontos, 0, num_pontos-1, cmpx);
    msort(&py, 0, num_pontos-1, cmpy);

    clock_t t;

    t = clock();

    double distancia = DivisaoConquista(pontos, py, 0, num_pontos-1, p1, p2);

    t = clock() - t;

    free(py);

    cout << fixed << setprecision(6);
    cout << t*1.0/CLOCKS_PER_SEC << ' ' << distancia << ' ' << p1.x << ' ' << p1.y << ' ' << p2.x << ' ' << p2.y << endl;
}

double DivisaoConquista(Ponto* px, Ponto* py, int e, int d, Ponto& p1, Ponto& p2)
{
    int num_pontos = d-e+1;

    if(num_pontos <= 3)
    {
        return ForcaBruta(px, e, d, p1, p2);
    }

    int meio = (d+e)/2;

    Ponto pye[meio-e+1];
    Ponto pyd[d-meio];

    int ei = 0, di = 0;
    for(int i=0; i<num_pontos;i++)
    {
        if(py[i].x <= px[meio].x && ei < meio-e+1)
        {
            pye[ei].x = py[i].x;
            pye[ei].y = py[i].y;
            ei++;
        }
        else
        {
            pyd[di].x = py[i].x;
            pyd[di].y = py[i].y;
            di++;
        }
    }

    double de = DivisaoConquista(px, pye, e, meio, p1, p2);
    double dd = DivisaoConquista(px, pyd, meio+1, d, p1, p2);

    double delta = min(de, dd);

    Ponto* pontos_delta = (Ponto*)malloc((num_pontos)*sizeof(Ponto));
    
    int idelta = 0;
    for (int i = 0; i < num_pontos; i++) 
    {
        if (fabsf(py[i].x - px[meio].x) < delta) 
        {
            pontos_delta[idelta].x = py[i].x;
            pontos_delta[idelta].y = py[i].y;
            idelta++;
        }
    }

    double ded = DistanciaEsquerdaDireita(pontos_delta, idelta, delta, p1, p2);

    free(pontos_delta);

    return min(delta, ded);
}

double DistanciaEsquerdaDireita(Ponto* p, int n, double delta, Ponto& p1, Ponto& p2)
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

int cmpx(const void *a, const void *b)
{
    Ponto *p1 = (Ponto*)a, *p2 = (Ponto*)b;
    return (p1->x - p2->x);
}

int cmpy(const void *a, const void *b)
{
    Ponto *p1 = (Ponto*)a, *p2 = (Ponto*)b;
    return (p1->y - p2->y);
}

void merge(Ponto** arr, int l, int m, int r, int (*cmp)(const void*, const void*))
{
    int n1 = m - l + 1;
    int n2 = r - m;

    Ponto* L = (Ponto*)malloc((n1)*sizeof(Ponto));
    Ponto* R = (Ponto*)malloc((n2)*sizeof(Ponto));

    for (int i = 0; i < n1; i++)
    {
        L[i].x = (*arr)[l + i].x;
        L[i].y = (*arr)[l + i].y;
    }
    for (int j = 0; j < n2; j++)
    {
        R[j].x = (*arr)[m + 1 + j].x;
        R[j].y = (*arr)[m + 1 + j].y;
    }

    int i = 0;

    int j = 0;

    int k = l;
 
    while (i < n1 && j < n2) {
        if (cmp(&L[i],&R[j]) < 0) {
            (*arr)[k].x = L[i].x;
            (*arr)[k].y = L[i].y;
            i++;
        }
        else {
            (*arr)[k].x = R[j].x;
            (*arr)[k].y = R[j].y;
            j++;
        }
        k++;
    }

    while (i < n1) {
        (*arr)[k].x = L[i].x;
        (*arr)[k].y = L[i].y;
        i++;
        k++;
    }

    while (j < n2) {
        (*arr)[k].x = R[j].x;
        (*arr)[k].y = R[j].y;
        j++;
        k++;
    }

    free(L);
    free(R);
}

void msort(Ponto** arr, int l, int r, int (*cmp)(const void*, const void*))
{
    if(l>=r){
        return;
    }
    int m =l+ (r-l)/2;
    msort(arr, l, m, cmp);
    msort(arr, m+1, r, cmp);
    merge(arr,l,m,r, cmp);
}