#include "empty_class.hpp"
#include <chrono>
using namespace std;

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        cerr<< "Test number shall be passed to the program"<< endl;
        return -1;
    }
    string test = argv[1];
    auto start = chrono::high_resolution_clock::now();
    vector<ProjectLibrary::Vertex> ciro;
    vector<ProjectLibrary::Edge> marco;
    vector<ProjectLibrary::Triangle> cosimo;
    unsigned int n= 1000;
    ProjectLibrary::Refine(cosimo, marco, ciro, n, test);
    ofstream outputFile("C:/Users/Alberto/Dropbox (Politecnico Di Torino Studenti)/PC/Desktop/foto_progetto/outputPunti.csv");
    for (unsigned int i=0; i<ciro.size(); i++)
    {
        outputFile<<ciro[i].x<<";"<<ciro[i].y<<"\n";
    }
    outputFile.close();

    ofstream outputFile1("C:/Users/Alberto/Dropbox (Politecnico Di Torino Studenti)/PC/Desktop/foto_progetto/outputLati.csv");
    for (unsigned int i=0; i<marco.size(); i++)
    {
        if (marco[i].active)
            outputFile1<<marco[i].start.x<<";"<<marco[i].start.y<<";"<<marco[i].finish.x<<";"<<marco[i].finish.y<<"\n";
    }

    auto end = std::chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout << "Tempo di esecuzione: " << duration.count() << "s" << endl;
    return 0;
}
