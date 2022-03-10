#include<iostream>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <utility>
#include <cmath>
#include <string>
#include <cfloat>
#include <chrono>

using namespace std::chrono;
using namespace std;

chrono::time_point<chrono::high_resolution_clock> start, stop;


double distance_between_two_points(pair<int,int> a, pair<int,int> b){
    int x_axis = (b.first - a.first)*(b.first - a.first);
    int y_axis = (b.second - a.second)*(b.second - a.second);
    double resp = sqrt(x_axis+y_axis);
    return resp;
}

void swap_position(int vetor[], int i, int j){
	int aux = vetor[i];
	vetor[i] = vetor[j];
	vetor[j] = aux;
}

double total_distance(vector<int> hamiltonian, vector<pair<int,int>> points){
    double total = 0;
    for(int i=0; i<hamiltonian.size()-1;i++){
        double distance = distance_between_two_points(points[hamiltonian[i]-1], points[hamiltonian[i+1]-1]);
        // //lines for debug
        //     cout<<"distance between "<<points[hamiltonian[i]-1].first<<","<<points[hamiltonian[i]-1].second
        //     <<" and "<<points[hamiltonian[i+1]-1].first<<","<<points[hamiltonian[i+1]-1].second<<"="<< distance<<endl;
        total += distance;
    }
    // cout<<"total distance: "<<total<<endl;


    return total;
}

void exchange(int vetor[], int inf, int sup, vector<pair<int,int>> p, double &lower_distance, string &lower_hamiltonian_cicle,int &cicles){
	
    stop = chrono::high_resolution_clock::now();
    double duration =chrono::duration<double, ratio<1, 1000>>(stop-start).count();
        if (duration>=7200000)
            return;
            
    if(inf == sup){
        //getting the hamiltonian cicle of this permutation
        vector<int> hamiltonian;
        string hamiltonian_cicle = "";
        double total = 0;
        hamiltonian.push_back(1);
        hamiltonian_cicle += to_string(1);        
		for(int i = 1; i <= sup; i++){
            hamiltonian.push_back(vetor[i]);
            hamiltonian_cicle += to_string(vetor[i]);  
        }
		// printf("\n");
        hamiltonian.push_back(1);
        hamiltonian_cicle += to_string(1);  
        
        //lines for debug
                // cout<<"ciclo hamiltonian: ";
                // for(int i=0; i<hamiltonian.size(); i++){
                //     cout<<hamiltonian[i];
                // }
                // cout<<"\n";
        // cout<<hamiltonian_cicle<<endl;
        total = total_distance(hamiltonian,p);
        cicles++;
        if (total < lower_distance){
            // cout<<"antes\n";
            // cout<<"ciclo: "<<lower_hamiltonian_cicle<<" com distancia: "<<lower_distance<<endl;
            lower_distance = total;
            lower_hamiltonian_cicle = hamiltonian_cicle;
            // cout<<"depois \n";
            // cout<<"ciclo: "<<lower_hamiltonian_cicle<<" com distancia: "<<lower_distance<<endl;
        }
        
	}
	else{
		for(int i = inf; i <= sup; i++){
			swap_position(vetor, inf, i);
			exchange(vetor, inf + 1, sup, p, lower_distance, lower_hamiltonian_cicle,cicles);
			swap_position(vetor, inf, i); 
		}
	}
}

int main(int argc, char** argv){
    ifstream pointsFile;
    int n = 0;
    int value;
    vector<int> aux;
    vector<pair<int,int>> points; //saves all points

    pointsFile.open(argv[1]);    
    pointsFile >> n; //getting the number of points in file (first line of each file must contais this information)

    cout << n << endl;
    while(pointsFile >> value){
        aux.push_back(value);
    }
    for(int i=0; i<aux.size(); i=i+2){
        points.push_back(make_pair(aux[i],aux[i+1]));
    }

    // lines for debug
            // cout<<" ponits \n";
            // for(int i=0; i<points.size(); i++){
            //     cout<<points[i].first <<" "<<points[i].second<<"\n";
            // }


    int v[n];
    for(int i=0; i<n;i++){
        v[i] = i+1;
    }

    // lines for debug
            // cout<<"v \n";
            // for(int i=0; i<n;i++){
            //     cout<<v[i]<<" ";
            // }
	int tam_v = sizeof(v) / sizeof(int);

    double lower_distance = DBL_MAX;
    // cout<<lower_distance<<endl;
    string lower_hamiltonian_cicle = "";
    int cicles = 0;

    start = chrono::high_resolution_clock::now();

	exchange(v, 1, tam_v - 1, points, lower_distance, lower_hamiltonian_cicle, cicles);

    stop = chrono::high_resolution_clock::now();
    double duration =chrono::duration<double, ratio<1, 1000>>(stop-start).count();

    cout<< "hamiltonian cicle with lower distance : "<< lower_hamiltonian_cicle <<"\n";
    cout<< "distance of this cicle = "<<lower_distance<<"\n";
    cout<< cicles << " clicles were generated in "<< duration << "ms \n";
    

    return 0;
}