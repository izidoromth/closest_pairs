#include <iostream>
#include <fstream>
#include <vector>  
#include <string>
#include <sstream>
 
using namespace std;

double** ObterPares(char* nome_do_arquivo);

int main(int argc, char* argv[])    
{    
    if(argv[0])
    double** pares = ObterPares(argv[1]);  
    return 0;    
}

double** ObterPares(char* nome_do_arquivo)
{
    ifstream arquivo;
    double** pares;
    string line;
    arquivo.open(nome_do_arquivo);
    if(!arquivo)
    {
        cout << "Arquivo nao encontrado!";
    }
    else
    {
        getline(arquivo, line);
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


