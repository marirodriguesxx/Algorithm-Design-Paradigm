#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <cmath>
#include <cfloat>
#include <algorithm>
#include <cstdlib>
#include <iterator>
#include <bits/stdc++.h>
#include <chrono>


using namespace std;
using namespace std::chrono;

double distance_between_points( pair<int, int> p1, pair<int, int> p2 )
{
   double delta_x = p2.first - p1.first;
   double delta_y = p2.second - p1.second;
   double distance = sqrt( (delta_x*delta_x) + (delta_y*delta_y) );
   return distance; 
}

bool is_in( vector<pair<int,int>> &vetor, pair<int,int> elem )
{
    for( int i = 0 ; i < vetor.size() ; i++ )
    {
        if( vetor[i].first == elem.first && vetor[i].second == elem.second  )
        {
            return true;
        }
    }
    return false;
}

void insert_points( vector< pair<int, int> > & convex_hull, set< pair<int, int> > & points )
{
    vector< pair<int, int> >::iterator it;
    set< pair<int, int> >::iterator it2;
    double lower_distance = DBL_MAX;
    pair<int, int> lower_point;

    pair<int,int> point_i;
    pair<int,int> point_j;
    pair<int,int> point_p;


    while( points.size() > 0 )
    {        
        vector< pair<int,int> > hamiltonian_cycle;
        for( int i = 0 ; i < convex_hull.size() - 1 ; i++ )
        {
            lower_distance = DBL_MAX;
            point_i = make_pair( convex_hull[i].first, convex_hull[i].second );
            
            for( it2 = points.begin() ; it2 != points.end() ; it2++ )
            {
                point_p = make_pair( it2->first, it2->second );

                point_j = make_pair( convex_hull[i+1].first, convex_hull[i+1].second );

                double distance_points_Pi_Pj = distance_between_points( point_i, point_j );
                double distance_points_Pi_Pp = distance_between_points( point_i, point_p );
                double distance_points_Pp_Pj = distance_between_points( point_p, point_j );

                double distance = ( distance_points_Pi_Pp + distance_points_Pp_Pj ) - distance_points_Pi_Pj;

                if( distance < lower_distance )
                {
                    lower_distance = distance;
                    lower_point = make_pair( it2->first, it2->second );
                }
            }

            points.erase( lower_point );

            if( hamiltonian_cycle.size() == 0 )
            {
                hamiltonian_cycle.push_back( point_i );
                hamiltonian_cycle.push_back( lower_point );
                hamiltonian_cycle.push_back( point_j );
            }
            else
            {
                if( !is_in( hamiltonian_cycle, point_i ) )
                {
                    hamiltonian_cycle.push_back( point_i );
                } 

                if( !is_in( hamiltonian_cycle, lower_point ))
                {
                    hamiltonian_cycle.push_back( lower_point );
                } 

                if( !is_in( hamiltonian_cycle, point_j ))
                {
                    hamiltonian_cycle.push_back( point_j );
                }
            }            
        }

        // --- updanting convex_hull --- //
        convex_hull.clear();

        for( int j = 0 ; j <= hamiltonian_cycle.size() ; j++ )
        {
            convex_hull.push_back( make_pair( hamiltonian_cycle[j].first, hamiltonian_cycle[j].second ) );
        }
        // --- --- //
    }
}


int main(int argc, char **argv )
{
    // --- Instantiating variables --- //
    chrono::time_point<chrono::high_resolution_clock> start, stop;
    ifstream convex_hull_data;
    ifstream points_data;
    vector< pair<int, int> >::iterator it;

    int convex_hull_size;
    vector< pair<int, int> > convex_hull;
    vector<int> aux; 

    int points_size;
    set< pair<int, int> > points;
    vector<pair<int,int>> points_index; //saves all points to get they index later

    vector<int> hamiltonian;
    // --- --- //

    convex_hull_data.open( argv[1] );
    points_data.open( argv[2] );

    if( !convex_hull_data || !points_data ) 
    { 
        cout << "Error: file could not be opened" << endl;
        exit(1);
    }
    
    // --- reading convex_hull data file --- //
    convex_hull_data >> convex_hull_size;
    for( int i = 0 ; i < convex_hull_size*2 ; i++ )
    {
        int value;
        convex_hull_data >> value;
        aux.push_back( value );
    }

    for( int i=0; i<aux.size(); i=i+2 ) {
        convex_hull.push_back( make_pair( aux[i],aux[i+1] ) );
    }
    // --- --- //

    aux.clear();

    // --- reading points data file --- //
    points_data >> points_size;
    for( int i = 0 ; i < points_size*2 ; i++ )
    {
        int value;
        points_data >> value;
        aux.push_back( value );
    }
    for( int i=0; i<aux.size(); i=i+2 ) {
        pair<int,int> elem = make_pair( aux[i],aux[i+1] );
        if( find(convex_hull.begin(), convex_hull.end(), elem ) == convex_hull.end()) // if there is not in convex_hull
        {
            points.insert( make_pair( aux[i],aux[i+1] ) );
        }
        points_index.push_back( make_pair( aux[i],aux[i+1] ) );
    }
    // --- --- //    


    start = chrono::high_resolution_clock::now();

    // --- Calling function that finds hamiltonian cycle --- //
    insert_points( convex_hull, points );
    // --- --- //

    stop = chrono::high_resolution_clock::now();
    double duration =chrono::duration<double, ratio<1, 1000>>(stop-start).count();

    

    // --- Creating output file with generated edges --- //
    string fileName("saida3.txt");
    ofstream file_out;

    file_out.open(fileName);
    if( !file_out )
    {
      cout << "Error: file could not be opened" << endl;
      exit(1);
    }

    vector< pair<int, int>> auxiliar;

    convex_hull[convex_hull.size()-1] = convex_hull[0]; 
    for (int i = 0; i < convex_hull.size() ; i++ ) {
        file_out << convex_hull[i].first << " " << convex_hull[i].second << "\n";
    }
    
    file_out.close();
    // --- --- //
    

    // --- Distance --- //
    double hamiltonian_distance = 0;

    for (int i = 0; i < convex_hull.size() - 1 ; i++ )
    {
        hamiltonian_distance += distance_between_points( make_pair( convex_hull[i].first, convex_hull[i].second ),
                                                 make_pair( convex_hull[i+1].first, convex_hull[i+1].second ) );
    }
    // --- --- //

    // --- Output program --- //
    cout << "\nTotal Hamiltonian Distance: " << hamiltonian_distance << "\n";
    cout << "Insertions Duration: " << duration << "ms \n";
    cout<<endl;
    // --- --- //

    return 0;
}