#include "empty_class.hpp"
#include <chrono>
using namespace std;
using namespace ProjectLibrary;

// Il seguente programma raffina una mesh triangolare, sono da inserire 2 oppure 3 input sotto command line argument
// separati da uno spazio. Il primo valore può essere 1 oppure 2 ed è il dataset che si utilizzerà. Il secondo valore
// è 1 oppure 2 ed è la modalità di raffinamento: in base all'area oppure in base al gradiente di una funzione a scelta.
// Nel caso si utilizzi la modalità 2, è necessario inserire un numero che sarà il numero della funzione che utilizzeremo
// per il calcolo dell'integrale sotteso.
int main(int argc, char **argv)
{
    if (argc < 2)
    {
        cerr << "Test number shall be passed to the program" << endl;
        return -1;
    }
    string test = argv[1];
    if (argc < 2)
    {
        cerr << "Mode type shall be passed to the program" << endl;
        return -1;
    }
    Triangle::mode = argv[2];
    if (argc < 2)
    {
        cerr << "Function number shall be passed to the program" << endl;
        return -1;
    }
    Triangle::function = argv[3];
    string function1=argv[3];
    auto start = chrono::high_resolution_clock::now();
    vector<Vertex> ciro;
    vector<Edge> marco;
    vector<Triangle> cosimo;
    unsigned int n;
    if(Triangle::mode == "1")
        n = 15000;
    else
        n = 0;
    ImportCell0Ds(ciro, n, test);
    ImportCell1Ds(marco, ciro, n, test);
    ImportCell2Ds(cosimo, marco, ciro, n, test);
    double exactarea= doubleIntegral(function1);
    double area=0.0;
    for (unsigned int i=0; i<cosimo.size(); i++)
    {
        area+=abs((cosimo[i].area*evaluateExpressionf0(cosimo[i].center.x,cosimo[i].center.y, cosimo[i].function)));
    }
    Refine(cosimo, marco, ciro, n, area, exactarea);
    ofstream outputFile("C:/Users/Alberto/Dropbox (Politecnico Di Torino Studenti)/PC/Desktop/foto_progetto/outputPunti.csv");
    for (unsigned int i = 0; i < ciro.size(); i++) {
        outputFile << ciro[i].x << ";" << ciro[i].y << "\n";
    }
    outputFile.close();

    ofstream outputFile1("C:/Users/Alberto/Dropbox (Politecnico Di Torino Studenti)/PC/Desktop/foto_progetto/outputLati.csv");
    for (unsigned int i = 0; i < marco.size(); i++) {
        if (marco[i].active)
            outputFile1 << marco[i].start.x << ";" << marco[i].start.y << ";"
                        << marco[i].finish.x << ";" << marco[i].finish.y << "\n";
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout << "Tempo di esecuzione: " << duration.count() << "s" << endl;
    return 0;
}
