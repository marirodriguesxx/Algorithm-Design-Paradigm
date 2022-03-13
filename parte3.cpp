#include<iostream>
#include<fstream>
#include<set>
#include<vector>
using namespace std;

int main(int argc, char **argv )
{
    ifstream convex_hull_data;
    ifstream points_data;
    set< pair<int, int> >::iterator it;

    int convex_hull_size;
    set< pair<int, int> > convex_hull;
    vector<int> aux;

    int points_size;
    set< pair<int, int> > points;

    convex_hull_data.open( argv[1] );
    points_data.open( argv[2] );

    if( !convex_hull_data || !points_data ) 
    { 
        cout << "Error: file could not be opened" << endl;
        exit(1);
    }
    
    // --- reading convex_hull data file --- //
    convex_hull_data >> convex_hull_size;
    cout << convex_hull_size << endl;
    for( int i = 0 ; i < convex_hull_size*2 ; i++ )
    {
        int value;
        convex_hull_data >> value;
        aux.push_back( value );
    }

    for( int i=0; i<aux.size(); i=i+2 ) {
        convex_hull.insert( make_pair( aux[i],aux[i+1] ) );
        //points_index.push_back( make_pair( aux[i],aux[i+1] ) );
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
        it = convex_hull.find( make_pair( aux[i],aux[i+1] ) );
        if( it == convex_hull.end() ) // if there is not in convex_hull
        {
            points.insert( make_pair( aux[i],aux[i+1] ) );
        }
        //points_index.push_back( make_pair( aux[i],aux[i+1] ) );
    }
    // --- --- //

    // --- DEBUG --- //
    for (it = convex_hull.begin() ; it != convex_hull.end() ; it++ ) {
            cout<<"("<<it->first<<","<<it->second<<") \n";
    }
    cout << "-----" << endl;
    for (it = points.begin() ; it != points.end() ; it++ ) {
            cout<<"("<<it->first<<","<<it->second<<") \n";
    }
    // --- --- //

    return 0;
}