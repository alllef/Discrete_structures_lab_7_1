#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include<Windows.h>
#include<iomanip>

using namespace std;

struct Rib {
public:
    int start;
    int end;
};

void initializeGraph(int &picks, int &ribs, vector <Rib> &structRibs);

void sortRibs(int &picks, int &ribs, vector <Rib> &structRibs);

vector<int> codePrufer(int &picks, int &ribs, vector <Rib> &structRibs);

int *decodePrufer(vector<int> pruferCode);

int findMin(vector<int> picks);

void printCode(vector<int> pruferCode);

void printAdjacenceMatrix(int *matrix, int picks);

void makeMenu(int &picks, int &ribs, vector <Rib> &structRibs);

int findMinNotExistingInPruferCode(vector<int> pruferCode, vector<int> picks);

int main() {
    SetConsoleOutputCP(CP_UTF8);
    vector <Rib> ribsList;
    int n = 0, m = 0;
    initializeGraph(n, m, ribsList);
    sortRibs(n, m, ribsList);
    makeMenu(n, m, ribsList);
    return 0;
}

void initializeGraph(int &picks, int &ribs, vector <Rib> &structRibs) {
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

void sortRibs(int &picks, int &ribs, vector <Rib> &structRibs) {

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

vector<int> codePrufer(int &picks, int &ribs, vector <Rib> &structRibs) {
    vector <Rib> fictRib = structRibs;
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

int *decodePrufer(vector<int> pruferCode) {
    int picksNumber = pruferCode.size() + 2;
    vector<int> picks(picksNumber);
    int *adjacenceMatrix = new int[picksNumber * picksNumber];

    for (int i = 0; i < picks.size(); i++) {
        picks[i] = i + 1;
    }

    for (int i = 0; i < picksNumber - 2; i++) {
        int uniquePickIndex = findMinNotExistingInPruferCode(pruferCode, picks);
        *(adjacenceMatrix + (pruferCode[0] - 1) * picksNumber + (picks[uniquePickIndex] - 1)) = 1;
        *(adjacenceMatrix + (picks[uniquePickIndex] - 1) * picksNumber + (pruferCode[0] - 1)) = 1;
        pruferCode.erase(pruferCode.begin());
        picks.erase(picks.begin() + uniquePickIndex);
    }
    *(adjacenceMatrix + (picks[0] - 1) * picksNumber + (picks[1] - 1)) = 1;
    *(adjacenceMatrix + (picks[1] - 1) * picksNumber + (picks[0] - 1)) = 1;
    return adjacenceMatrix;
}

int findMinNotExistingInPruferCode(vector<int> pruferCode, vector<int> picks) {
    vector<int> uniquePicks;
    for (int i = 0; i < picks.size(); i++) {
        bool isUnique = true;

        for (int j = 0; j < pruferCode.size(); j++) {
            if (picks[i] == pruferCode[j])isUnique = false;
        }
        if (isUnique)uniquePicks.push_back(picks[i]);
    }

    int uniqueMinPick = findMin(uniquePicks);
    for (int i = 0; i < picks.size(); i++) {
        if (picks[i] == uniqueMinPick)return i;
    }
    return -1;
}

int findMin(vector<int> picks) {
    int min = picks[0];

    for (int i = 0; i < picks.size() - 1; i++) {
        if (picks[i] > picks[i + 1])min = picks[i + 1];
    }

    return min;
}

void printCode(vector<int> pruferCode) {
    cout << "Код для цього дерева виглядає так\n";

    for (int i : pruferCode) {
        cout << i << setw(4);
    }
    cout << endl;
}

void printAdjacenceMatrix(int *matrix, int picks) {
    cout << "Матриця суміжності\n";
    cout << setw(8) << 1;
    for (int i = 2; i <= picks; i++) {

        cout << setw(4) << i << setw(4);
    }

    cout << endl;

    for (int i = 0; i < picks; i++) {
        cout << setw(4) << i + 1 << setw(4);

        for (int j = 0; j < picks; j++) {
            if (*(matrix + i * picks + j) != 1)cout << 0 << setw(4);
            else cout << *(matrix + i * picks + j) << setw(4);
        }

        cout << endl;
    }
}

void makeMenu(int &picks, int &ribs, vector <Rib> &structRibs) {
    bool isDecoding = false;
    cout << "Введіть 0 якщо хочете побачити код для дерева файлі та 1 якщо хочете декодувати код в дерево";
    cin >> isDecoding;
    if (isDecoding) {
        cout << "Введіть кількість вершин у дерева, яке хочете декодувати\n";

        int pickNumber;
        cin >> pickNumber;
        vector<int> pruferCode;
        cout << "Вводьте код Прюфера\n";

        for (int i = 0; i < pickNumber - 2; i++) {
            int tmpPick;
            cin >> tmpPick;
            pruferCode.push_back(tmpPick);
        }

        printAdjacenceMatrix(decodePrufer(pruferCode), pickNumber);
    } else {
        printCode(codePrufer(picks, ribs, structRibs));
    }
}