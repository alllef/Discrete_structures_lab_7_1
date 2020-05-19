#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include<Windows.h>

using namespace std;

struct Rib {
public:
    int start;
    int end;
};

void initializeGraph(int &picks, int &ribs, vector<Rib> &structRibs);

void sortRibs(int &picks, int &ribs, vector<Rib> &structRibs);

vector<int> codePrufer(int &picks, int &ribs, vector<Rib> &structRibs);

int main() {
    SetConsoleOutputCP(CP_UTF8);
    vector<Rib> ribsList;
    int n = 0, m = 0;
    initializeGraph(n, m, ribsList);
    sortRibs(n, m, ribsList);
    codePrufer(n, m, ribsList);
    return 0;
}

void initializeGraph(int &picks, int &ribs, vector<Rib> &structRibs) {
    Rib myRib{};
    ifstream inFile;
    inFile.open("myGraph.txt");

    if (!inFile.is_open()) cout << "It is not open" << endl;
    inFile >> picks >> ribs;

    for (int i = 0; i < ribs; i++) {
        inFile >> myRib.start >> myRib.end;

        structRibs.push_back(myRib);
    }
    inFile.close();
}

void sortRibs(int &picks, int &ribs, vector<Rib> &structRibs) {

    Rib tmp{};
    for (int i = 0; i < ribs - 1; i++) {
        for (int j = 0; j < ribs - 1; j++) {
            if ((structRibs[j].start + structRibs[j].end) > (structRibs[j + 1].start + structRibs[j + 1].end)) {

                tmp = structRibs[j];
                structRibs[j] = structRibs[j + 1];
                structRibs[j + 1] = tmp;

            }
        }
    }
}

vector<int> codePrufer(int &picks, int &ribs, vector<Rib> &structRibs) {
    vector<Rib> fictRib = structRibs;
    vector<int> pruferCode;
    while (pruferCode.size() != picks - 2) {

        int minLeaf = picks + 1;
        int ribNumber = 0;
        int adjacencyCounter = 0;
        int ribToDelete = 0;
        for (int i = 1; i <= picks; i++) {


            for (int j = 0; j < fictRib.size(); j++) {

                if (fictRib[j].start == i || fictRib[j].end == i) {
                    adjacencyCounter++;
                    ribNumber = j;
                }
                
            }

            if (adjacencyCounter == 1 && i < minLeaf) {
                minLeaf = i;
                ribToDelete = ribNumber;
            }

            adjacencyCounter = 0;
        }

        if (fictRib[ribToDelete].start == minLeaf) {
            pruferCode.push_back(fictRib[ribToDelete].end);
            fictRib.erase(fictRib.begin() + ribToDelete);
        }

        if (fictRib[ribToDelete].end == minLeaf) {
            pruferCode.push_back(fictRib[ribToDelete].start);
            fictRib.erase(fictRib.begin() + ribToDelete);
        }

    }
    return pruferCode;
}