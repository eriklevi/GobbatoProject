#include <list>
#include <iostream>
#include <string.h>
#include <algorithm>
#include "utils.h"
#include "heuristic.h"

using namespace std;

int main(int argc,char *argv[]){
	bool _test = false;
	string _inPath;
	string _outPath;
	string _solPath;

	// Read input parameters
	for(int i=1; i< argc;i++) {
		if(strcmp(argv[i], "-i") == 0)
			_inPath = argv[++i];
		else if(strcmp(argv[i], "-o") == 0)
			_outPath = argv[++i];
		else if(strcmp(argv[i], "-s") == 0)
			_solPath = argv[++i];
		else if(strcmp(argv[i], "-test") == 0)
			_test = true;
	}

	if(!_test && (_inPath.empty() || _outPath.empty())) {
		cout << "------------------------------ " << endl;
		cout << "CMD" << endl;
		cout << "------------------------------ " << endl;
		cout << "-i path of the instance file" << endl;
		cout << "-o path of the output file" << endl;
		cout << "-s path of the output solution file or of the solution to test(optional)" << endl;
		cout << "-test enable the feasibility test (optional)" << endl;
        return 1;
	}

	if(!_test) {
		// Read the instance file
		Heuristic _heuristic(_inPath);
		// Solve the problem
		vector<double> stat;
        bool tempo = true;
        std::vector<StartingSolutions>* SoluzioniIniziali = new std::vector <StartingSolutions> ();
        std::vector<GeneratedVector>* appVector = _heuristic.buildInitializationVector();
        int QuanteSoluzioniIniziali = 100;
        std::cout << "Inizializzazione finita!!" << std::endl;

        for (int i = 0 ; i<QuanteSoluzioniIniziali; i++)
            SoluzioniIniziali->push_back(_heuristic.GenerateStartingSolutions(appVector));

        std::sort(SoluzioniIniziali->begin(), SoluzioniIniziali->end(),
                  [] (const StartingSolutions& a, const StartingSolutions& b) -> bool {
                return a.ObjectiveFunction > b.ObjectiveFunction;
            } );
        std::vector<StartingSolutions>* VettoreFigli = new std::vector <StartingSolutions> ();

        while (tempo) {

            for (int i =0; i< QuanteSoluzioniIniziali/2; i++) {
                VettoreFigli->push_back(_heuristic.GenerateChildren(SoluzioniIniziali->at(i)));
                VettoreFigli->push_back(_heuristic.GenerateChildren(SoluzioniIniziali->at(i)));
            }
            SoluzioniIniziali = VettoreFigli;
            std::sort(SoluzioniIniziali->begin(), SoluzioniIniziali->end(),
                      [] (const StartingSolutions& a, const StartingSolutions& b) -> bool {
                          return a.ObjectiveFunction > b.ObjectiveFunction;
                      } );
            tempo = false;
        }

        StartingSolutions SoluzioneFinale = SoluzioniIniziali->at(0);


        return 1;

		_heuristic.solveFast(stat);
		_heuristic.getStatSolution(stat);
		// Write KPI of solution
        string instanceName = splitpath(_inPath);
        _heuristic.writeKPI(_outPath, instanceName, stat);
		// Write solution
		if(!_solPath.empty())
			_heuristic.writeSolution(_solPath);

	}
	else {
        // Read the instance file
		Heuristic _heuristic(_inPath);
		// Read the solution file
		eFeasibleState _feasibility = _heuristic.isFeasible(_solPath);
		switch(_feasibility) {
			case FEASIBLE:
				cout << "Solution is feasible" << endl;
				break;
			case NOT_FEASIBLE_DEMAND:
				cout << "Solution is not feasible: demand not satisfied" << endl;
				break;
			case NOT_FEASIBLE_USERS:
				cout << "Solution is not feasible: exceeded number of available users" << endl;
				break;
		}
	}

	return 0;
}

