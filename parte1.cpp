#include<iostream>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <utility>
#include<cmath>

using namespace std;

double distance_between_two_points(pair<int,int> a, pair<int,int> b){
    int x_axis = (b.first - a.first)*(b.first - a.first);
    int y_axis = (b.second - a.second)*(b.second - a.second);
    double resp = sqrt(x_axis+y_axis);
    return resp;
}

void swap(int vetor[], int i, int j){
	int aux = vetor[i];
	vetor[i] = vetor[j];
	vetor[j] = aux;
}

void exchange(int vetor[], int inf, int sup){
	if(inf == sup){
		for(int i = 0; i <= sup; i++)
			printf("%d ", vetor[i]);
		printf("\n");
	}
	else{
		for(int i = inf; i <= sup; i++){
			swap(vetor, inf, i);
			exchange(vetor, inf + 1, sup);
			swap(vetor, inf, i); // backtracking
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

    //lines for debug
    // cout<<" ponits \n";
    // for(int i=0; i<points.size(); i++){
    //     cout<<points[i].first <<" "<<points[i].second<<"\n";
    // }
    //    cout<< distance_between_two_points(make_pair(2,-6),make_pair(7,3))<<endl;

    //TODO: organizar permutação
	//int v[n+1];
    // for(int i=1; i<n; i++){
    //     v[i]
    // }
	// int tam_v = sizeof(v) / sizeof(int);

	// exchange(v, 0, tam_v - 1);

    return 0;
}