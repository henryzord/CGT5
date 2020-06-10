// *********************************************
// Coloque o trecho de código a seguir, no
// lugar da main() do seu programa.
// *********************************************

#include <iostream>
#include <fstream>
using namespace std;

int nPontos;
int nVertices;
Vetor Poligono[10];
Vetor Pontos[10];
ifstream arq;

// *********************************************
void LePontos() {
    Vetor p;
    int x,y;
    string dummy;

    arq >> dummy;
    arq >> nPontos;
    cout << dummy << endl;
    for (int i= 0; i< nPontos;i++)
    {
        arq >> x >> y;
        Pontos[i].set(x,y);
        Pontos[i].imprime();
        cout << endl;
    }
    cout << "-------"<< endl;
}
// *********************************************
void LeVertices() {
    int x,y;
    string dummy;
    arq >> dummy;
    cout << "Poligono: " << dummy << endl;

    arq >> nVertices;
    for (int i= 0; i< nVertices;i++)
    {
        arq >> x >> y;
        Poligono[i].set(x,y);
        Poligono[i].imprime();
        cout << endl;
    }

}
// *********************************************
void TestaConvexo() {
    for(int i=0;i<nPontos;i++) {
        cout << "Testando: ";
        Pontos[i].imprime();
        cout << "-> ";
        if (estaDentroConvexo(nVertices, Poligono, Pontos[i]))
            cout << " Dentro.";
        else cout << " Fora.";
        cout << endl;
    }
}
void TestaConcavo() {
    for(int i=0;i<nPontos;i++) {
        cout << "Testando: ";
        Pontos[i].imprime();
        cout << "-> ";
        if (estaDentroConcavo(nVertices, Poligono, Pontos[i]))
            cout << " Dentro.";
        else cout << " Fora.";
        cout << endl;
    }
}
void ClassificaPoligono() {
    cout << "Classificação do Polígono: ";
    if (classificaPoligono(nVertices, Poligono))
        cout << "CONVEXO." << endl;
    else cout << "CONCAVO." << endl;
}
// *********************************************
int main() {

    arq.open("Poligonos.txt", ios::in);
    if(!arq)
    {
        cout << "Problemas na abertura do arquivo" << endl;
        exit(1);
    }

    // Le o conjunto de pontos a serem testados
    LePontos();

    // Le e testa o primeiro polígono
    LeVertices();  ClassificaPoligono(); TestaConvexo();

    // Le e testa o segundo polígono
    LeVertices(); ClassificaPoligono(); TestaConvexo();

    // Le e testa o terceiro polígono
    LeVertices(); ClassificaPoligono(); TestaConcavo();

    // Le e testa o quarto polígono
    LeVertices(); ClassificaPoligono(); TestaConcavo();

    arq.close();

    return 0;
}