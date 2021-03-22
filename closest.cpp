#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>  
#include <string>
#include <sstream>
#include <math.h>
#include <time.h>

using namespace std;

double** ObterPares(char* nome_do_arquivo, int* num_pontos);

void ResolverForcaBruta(double** pares, int num_pontos);

int main(int argc, char* argv[])    
{    
    if(!argv[0])
    {
        cout << "Eh preciso informar um arquivo de entrada" << endl;
        return -1;
    }
    int *num_pontos;
    double** pares = ObterPares(argv[1], num_pontos);  
    ResolverForcaBruta(pares, *num_pontos);
    return 0;    
}

double** ObterPares(char* nome_do_arquivo, int* num_pontos)
{
    ifstream arquivo;
    double** pares;
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
        pares = (double**)malloc(count*sizeof(double*));
        int i = 0;
        while(i < count)
        {
            getline(arquivo, line);  
            double *par = (double*)malloc(2*sizeof(double));       
            stringstream ss(line);
            string valor;
            int j = 0;
            while(getline(ss, valor, ' '))
            {
                par[j] = stod(valor);
                j++;
            }
            pares[i] = par;
            i++;
        }
    }
    return pares;
}

void ResolverForcaBruta(double** pares, int num_pontos)
{
    double *par1, *par2;
    double distancia = 4297967296;
    clock_t t;
    
    t = clock();

    for(int i=0; i<num_pontos; i++)
    {
        for(int j=0; j<num_pontos; j++)
        {
            if(i==j) continue;

            double dx = pares[i][0]-pares[j][0];
            double dy = pares[i][1]-pares[j][1];

            double dist = sqrt(pow(dx,2) + pow(dy,2));

            if(dist < distancia)
            {
                distancia = dist;
                par1 = pares[i];
                par2 = pares[j];
            }
        }
    }

    t = clock() - t;

    cout << fixed << setprecision(6);
    cout << t*1.0/CLOCKS_PER_SEC << ' ' << distancia << ' ' << par1[0] << ' ' << par1[1] << ' ' << par2[0] << ' ' << par2[1];
}
