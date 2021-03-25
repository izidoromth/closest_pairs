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
void ForcaBruta(Ponto pontos[], int num_pontos, Ponto* p1, Ponto* p2, double *distancia);
void ResolverDivisaoConquista(Ponto pontos[], int num_pontos);
void DivisaoConquista(Ponto* px, Ponto* py, Ponto* p1, Ponto* p2, double* distancia);
void MergeSort(Ponto pontos[], int l, int r, bool order_x);
void Merge(Ponto pontos[], int l, int m, int r, bool order_x);

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
        int count = stoi(line);
        pontos = (Ponto*)malloc(count*sizeof(Ponto));
        int i = 0;
        while(i < count)
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

    ForcaBruta(pontos, num_pontos, &p1, &p2, &distancia);

    t = clock() - t;

    cout << fixed << setprecision(6);
    cout << t*1.0/CLOCKS_PER_SEC << ' ' << distancia << ' ' << p1.x << ' ' << p1.y << ' ' << p2.x << ' ' << p2.y;
}

void ForcaBruta(Ponto pontos[], int num_pontos, Ponto* p1, Ponto* p2, double *distancia)
{
    for(int i=0; i<num_pontos; i++)
    {
        for(int j=0; j<num_pontos; j++)
        {
            if(i==j) continue;

            double dx = pontos[i].x-pontos[j].x;
            double dy = pontos[i].y-pontos[j].y;

            double dist = sqrt(pow(dx,2) + pow(dy,2));

            if(dist < *distancia)
            {
                *distancia = dist;
                *p1 = pontos[i];
                *p2 = pontos[j];
            }
        }
    }
}

void ResolverDivisaoConquista(Ponto pontos[], int num_pontos)
{
    Ponto p1, p2;

    Ponto* px;
    Ponto* py;

    memcpy(py, pontos, num_pontos*sizeof(Ponto));
    memcpy(px, pontos, num_pontos*sizeof(Ponto));

    // copy_n(pontos, num_pontos, px);
    // copy_n(pontos, num_pontos, py);     

    MergeSort(px, 0, num_pontos-1, true);
    MergeSort(py, 0, num_pontos-1, false);

    double distancia = 4297967296;

    clock_t t;

    t = clock();

    DivisaoConquista(px, py, &p1, &p2, &distancia);

    t = clock() - t;

    cout << fixed << setprecision(6);
    cout << t*1.0/CLOCKS_PER_SEC << ' ' << distancia << ' ' << p1.x << ' ' << p1.y << ' ' << p2.x << ' ' << p2.y;
}

void DivisaoConquista(Ponto* px, Ponto* py, Ponto* p1, Ponto* p2, double* distancia)
{

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