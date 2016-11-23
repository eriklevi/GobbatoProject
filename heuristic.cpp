#include <iostream>
#include <random>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <random>
#include <ctime>
#include "heuristic.h"


Heuristic::Heuristic(string path){
    this->hasSolution = false;
    string line;
    string word;

    ifstream iffN(path.c_str());

    if (!iffN.is_open()) {
        cout << "Impossible to open" << path << endl;
        cin.get();
        exit(1);
    }

    getline(iffN, line);
    std::replace(line.begin(), line.end(), ';', ' ');
    istringstream iss(line);
    iss >> word;
    this->nCells = atoi(word.c_str());
    iss >> word;
    this->nTimeSteps = atoi(word.c_str());
    iss >> word;
    this->nCustomerTypes = atoi(word.c_str());

    // Memory allocation
    solution = new int***[nCells];
    problem.costs = new double***[nCells];
    for (int i = 0; i < this->nCells; i++) {
        problem.costs[i] = new double**[nCells];
        solution[i] = new int**[nCells];
        for (int j = 0; j < this->nCells; j++) {
            problem.costs[i][j] = new double*[nCustomerTypes];
            solution[i][j] = new int*[nCustomerTypes];
            for (int m = 0; m < this->nCustomerTypes; m++) {
                problem.costs[i][j][m] = new double[nTimeSteps];
                solution[i][j][m] = new int[nTimeSteps];
            }
        }
    }
    problem.n = new int[nCustomerTypes];
    problem.activities = new int[nCells];
    problem.usersCell = new int**[nCells];
    for (int i = 0; i < this->nCells; i++) {
        problem.usersCell[i] = new int*[nCustomerTypes];
        for (int m = 0; m < this->nCustomerTypes; m++) {
            problem.usersCell[i][m] = new int[nTimeSteps];
        }
    }

    getline(iffN, line);
    getline(iffN, line);
    std::replace(line.begin(), line.end(), ';', ' ');
    istringstream issN(line);
    for (int m = 0; m < nCustomerTypes; m++) {
        issN >> word;
        problem.n[m] = atoi(word.c_str());
    }

    getline(iffN, line);
    for (int m = 0; m < nCustomerTypes; m++) {
        for (int t = 0; t < nTimeSteps; t++) {
            getline(iffN, line);// linea con m e t
            for (int i = 0; i < nCells; i++) {
                getline(iffN, line);// linea della matrice c_{ij} per t ed m fissati
                istringstream issC(line);
                for (int j = 0; j < nCells; j++) {
                    issC >> word;
                    problem.costs[i][j][m][t] = atoi(word.c_str());
                }
            }
        }
    }

    getline(iffN, line);
    getline(iffN, line);
    std::replace(line.begin(), line.end(), ';', ' ');
    istringstream issA(line);
    for (int i = 0; i < nCells; i++) {
        issA >> word;
        problem.activities[i] = atoi(word.c_str());
    }

    getline(iffN, line);
    for (int m = 0; m < nCustomerTypes; m++) {
        for (int t = 0; t < nTimeSteps; t++) {
            getline(iffN, line);
            getline(iffN, line);
            std::replace(line.begin(), line.end(), ';', ' ');
            istringstream issU(line);
            for (int i = 0; i < nCells; i++) {
                issU >> word;
                problem.usersCell[i][m][t] = atoi(word.c_str());
            }
        }
    }

}

void Heuristic::solveFast(vector<double>& stat, int timeLimit) {
    double objFun=0;
    clock_t tStart = clock();

    for (int i = 0; i < nCells; i++)
        for (int j = 0; j < nCells; j++)
            for (int m = 0; m < nCustomerTypes; m++)
                for (int t = 0; t < nTimeSteps; t++)
                    solution[i][j][m][t] = 0;

    for (int i = 0; i < nCells; i++) {
        int demand = problem.activities[i];

        bool notSatisfied = true;
        for (int j = 0; j < nCells && notSatisfied; j++) {
            for (int m = 0; m < nCustomerTypes && notSatisfied; m++) {
                for (int t = 0; t < nTimeSteps && notSatisfied; t++) {
                    if (i != j) {
                        if (demand > problem.n[m] * problem.usersCell[j][m][t]) {
                            solution[j][i][m][t] = problem.usersCell[j][m][t];
                            problem.usersCell[j][m][t] -= solution[j][i][m][t];
                        }
                        else {
                            solution[j][i][m][t] += floor(demand / problem.n[m]);
                            notSatisfied = false;
                        }
                        if (solution[j][i][m][t] != 0)
                            objFun += solution[j][i][m][t] * problem.costs[j][i][m][t];
                        demand -= problem.n[m]*solution[j][i][m][t];
                    }
                }
            }
        }
    }


    stat.push_back(objFun);
    stat.push_back((double)(clock() - tStart) / CLOCKS_PER_SEC);

    hasSolution=true;
}
std::vector<GeneratedVector>* Heuristic::buildInitializationVector () {
    std::vector<GeneratedVector>* appVector = new std::vector<GeneratedVector> ();
    int indexForApp = 0;

    for (int i=0; i<nTimeSteps; i++) {
        for (int j=0; j<nCustomerTypes; j++) {
            for (int k=0; k<nCells; k++) {
                if (problem.usersCell[k][j][i] != 0) {
                    GeneratedVector data;
                    data.value = problem.usersCell[k][j][i];
                    data.index = indexForApp;
                    appVector->push_back(data);
                }
                indexForApp ++;
            }
        }
    }

    return appVector;

}


struct StartingSolutions Heuristic::GenerateStartingSolutions(std::vector<GeneratedVector>* appVector) {
    std::vector<int>* myActivities = new std::vector<int> ();
    myActivities->assign(problem.activities, problem.activities + nCells);
    struct StartingSolutions mySolution;
    mySolution.startingSolution = new int***[nCells];
    for (int i = 0; i < this->nCells; i++) {
        mySolution.startingSolution[i] = new int**[nCells];
        for (int j = 0; j < this->nCells; j++) {
            mySolution.startingSolution[i][j] = new int*[nCustomerTypes];
            for (int m = 0; m < this->nCustomerTypes; m++) {
                mySolution.startingSolution[i][j][m] = new int[nTimeSteps];
            }
        }
    }

    std::cout << "Fino a qui finita" << std::endl;

    int sizeAppVector = appVector->size();

    try {
        for (int i = 0; i<nCells; i++) {
            // For each cell
            while (myActivities->at(i) != 0) {
                //Generate the random number that select who to send
                std::random_device rd; // obtain a random number from hardware
                std::mt19937 eng(rd()); // seed the generator
                sizeAppVector = appVector->size();
                std::uniform_int_distribution<> distr(0, sizeAppVector); // define the range
                int selected = distr(eng);
                int index = appVector->at(selected).index;

                std::cout << selected << " " << index << std::endl;
                // From index find j t m (starting cell, timestamp, user)
                int j = index%nCells;
                if (j==i) break;
                int t = index/(nCells)%nTimeSteps;
                int m = index/(nCells*nTimeSteps);

                std::cout << i << " " << j << " " << m << " " << t << " " << appVector->at(selected).value << std::endl;
                //Another Random number to decide how many to choose

                std::uniform_int_distribution<> distr1(1, myActivities->at(i)); // define the range
                int nPeople = distr1(eng);

                if (appVector->at(selected).value > (nPeople * (m+1))) {
                    appVector->at(selected).value -= myActivities->at(i);
                    myActivities->at(i) -= nPeople * (m+1);
                    mySolution.startingSolution[i][j][m][t] += nPeople * (m+1);
                    mySolution.ObjectiveFunction += problem.costs[i][j][m][t] * nPeople;
                    std::cout << "Debug" << std::endl;


                }
                else {
                    myActivities->at(i) -= appVector->at(selected).value * (m+1);
                    std::cout << appVector ->size() << std::endl;
                    mySolution.startingSolution[i][j][m][t] += appVector->at(selected).value * (m+1);
                    mySolution.ObjectiveFunction += problem.costs[i][j][m][t] * appVector->at(selected).value;
                    appVector->erase(appVector->begin() + selected);
                    std::cout << appVector -> size() << std::endl;

                }


            }
        }
    }
    catch (std::out_of_range& e) {
        std::cout << e.what() << std::endl;
    }

    return mySolution;

}

struct StartingSolutions Heuristic::GenerateChildren(struct StartingSolutions ss) {

    int Nvolte = nCells*nCells*0.4;
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 eng(rd()); // seed the generator

    for (int k = 0; k < Nvolte; k++) {
        std::uniform_int_distribution<> distr(0, nCells); // define the range
        int i = distr(eng);
        int j = distr(eng);
        if ( i == j ) break;

        for (int h = 0 ; h< 20; h++) {
            std::uniform_int_distribution<> distr1 (0, nCustomerTypes);
            std::uniform_int_distribution<> distr2 (0, nTimeSteps);
            // Genero gli indici m1 t1 e m2 t2 per i valori da swappare

            int m1 = distr1(eng);
            int t1 = distr2(eng);
            int m2 = distr1(eng);
            int t2 = distr2(eng);
            if (ss.startingSolution[i][j][m1][t1] == ss.startingSolution[i][j][m2][t2]) break;
            int Sum = 0;
            int variation = ss.startingSolution[i][j][m1][t1] - ss.startingSolution[i][j][m2][t2];

            if (variation>0) {
                //Swappo
                int app = ss.startingSolution[i][j][m2][t2] ;
                ss.startingSolution[i][j][m2][t2] = ss.startingSolution[i][j][m1][t1];
                ss.startingSolution[i][j][m1][t1] = app;
                //Controllo la nuova somma
                for (int j1=0 ; j1 < nCells ; j1++) {
                    Sum += ss.startingSolution[i][j1][m2][t2];
                }
                if (Sum > problem.usersCell[i][m2][t2]) {
                    // Non va bene, rollback
                    app = ss.startingSolution[i][j][m2][t2] ;
                    ss.startingSolution[i][j][m2][t2] = ss.startingSolution[i][j][m1][t1];
                    ss.startingSolution[i][j][m1][t1] = app;
                } else {
                    // Va bene, aggiorno l'objF
                    ss.ObjectiveFunction -= variation*problem.costs[i][j][m1][t1];
                    ss.ObjectiveFunction += variation*problem.costs[i][j][m2][t2];

                }
            } else { // m2 t2 contiene l'elemento maggiore
                int app = ss.startingSolution[i][j][m2][t2] ;
                ss.startingSolution[i][j][m2][t2] = ss.startingSolution[i][j][m1][t1];
                ss.startingSolution[i][j][m1][t1] = app;
                //Controllo la nuova somma
                for (int j1=0 ; j1 < nCells ; j1++) {
                    Sum += ss.startingSolution[i][j1][m1][t1];
                }
                if (Sum > problem.usersCell[i][m1][t1]) {
                    // Non va bene, rollback
                    app = ss.startingSolution[i][j][m2][t2] ;
                    ss.startingSolution[i][j][m2][t2] = ss.startingSolution[i][j][m1][t1];
                    ss.startingSolution[i][j][m1][t1] = app;
                } else {
                    // Va bene, aggiorno l'objF
                    ss.ObjectiveFunction -= variation*problem.costs[i][j][m2][t2];
                    ss.ObjectiveFunction += variation*problem.costs[i][j][m1][t1];

                }
            }

        }


    }


}


void Heuristic::writeKPI(string path, string instanceName, vector<double> stat){
    if (!hasSolution)
        return;

    ofstream fileO(path, ios::app);
    if(!fileO.is_open())
        return;

    fileO << instanceName << ";" << stat[0] << ";" << stat[1];
    for(int i=2; i<stat.size(); i++)
        fileO <<  ";" << stat[i];
    fileO << endl;

    fileO.close();

}

void Heuristic::writeSolution(string path) {
    if (!hasSolution)
        return;

    ofstream fileO(path);
    if(!fileO.is_open())
        return;

    fileO << this->nCells << "; " << this->nTimeSteps << "; " << this->nCustomerTypes << endl;
    for (int m = 0; m < this->nCustomerTypes; m++)
        for (int t = 0; t < this->nTimeSteps; t++)
            for (int i = 0; i < this->nCells; i++)
                for (int j = 0; j < this->nCells; j++)
                    if (solution[i][j][m][t] > 0)
                        fileO << i << ";" << j << ";" << m << ";" << t << ";" << solution[i][j][m][t] << endl;

    fileO.close();
}

eFeasibleState Heuristic::isFeasible(string path) {

    string line;
    string word;
    int nCellsN;
    int nTimeStepsN;
    int nCustomerTypesN;
    int i, j, m, t;


    ifstream iffN(path.c_str());

    if (!iffN.is_open()) {
        cout << "Impossible to open" << path << endl;
        exit(1);
    }

    getline(iffN, line);
    std::replace(line.begin(), line.end(), ';', ' ');
    istringstream iss(line);
    iss >> word; // nCells
    nCellsN = atoi(word.c_str());
    iss >> word; // nTimeSteps
    nTimeStepsN = atoi(word.c_str());
    iss >> word; // nCustomerTypes
    nCustomerTypesN = atoi(word.c_str());

    int**** solutionN = new int***[nCells];
    for (i = 0; i < nCellsN; i++) {
        solutionN[i] = new int**[nCells];
        for (j = 0; j < nCellsN; j++) {
            solutionN[i][j] = new int*[nCustomerTypes];
            for (m = 0; m < nCustomerTypesN; m++) {
                solutionN[i][j][m] = new int[nTimeSteps];
                for ( t = 0; t < nTimeStepsN; t++) {
                    solutionN[i][j][m][t] = 0;
                }
            }
        }
    }

    while (getline(iffN, line)) {
        std::replace(line.begin(), line.end(), ';', ' ');
        istringstream iss(line);
        iss >> word; // i
        i = atoi(word.c_str());
        iss >> word; // j
        j = atoi(word.c_str());
        iss >> word; // m
        m = atoi(word.c_str());
        iss >> word; // t
        t = atoi(word.c_str());
        iss >> word; // value
        solutionN[i][j][m][t] = atoi(word.c_str());
    }

    // Demand
    bool feasible = true;
    int expr;
    for (int i = 0; i < nCells; i++) {
        expr = 0;
        for (int j = 0; j < nCells; j++)
            for (int m = 0; m < nCustomerTypes; m++)
                for (int t = 0; t < nTimeSteps; t++)
                    expr += problem.n[m] * solutionN[j][i][m][t];
        if (expr < problem.activities[i])
            feasible = false;
    }

    if (!feasible)
        return NOT_FEASIBLE_DEMAND;

    // Max Number of users
    for (int i = 0; i < nCells; i++)
        for (int m = 0; m < nCustomerTypes; m++)
            for (int t = 0; t < nTimeSteps; t++) {
                expr = 0;
                for (int j = 0; j < nCells; j++)
                    expr += solutionN[i][j][m][t];
                if (expr > problem.usersCell[i][m][t])
                    feasible = false;
            }

    if(!feasible)
        return NOT_FEASIBLE_USERS;

    return FEASIBLE;
}

void Heuristic::getStatSolution(vector<double>& stat) {
    if (!hasSolution)
        return;

    int* tipi = new int[nCustomerTypes];
    for (int m = 0; m < nCustomerTypes; m++)
        tipi[m] = 0;

    for (int i = 0; i < nCells; i++)
        for (int j = 0; j < nCells; j++)
            for (int t = 0; t < nTimeSteps; t++)
                for (int m = 0; m < nCustomerTypes; m++)
                    if (solution[i][j][m][t] > 0)
                        tipi[m] += solution[i][j][m][t];
    for (int m = 0; m < nCustomerTypes; m++)
        stat.push_back(tipi[m]);

}