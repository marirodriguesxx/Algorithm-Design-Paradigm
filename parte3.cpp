#include<iostream>
#include<fstream>
#include<set>
#include<vector>
#include<cmath>
#include<cfloat>
using namespace std;

double distance_between_points( pair<int, int> p1, pair<int, int> p2 )
{
   double delta_x = p2.first - p1.first;
   double delta_y = p2.second - p1.second;
   double distance = sqrt( (delta_x*delta_x) + (delta_y*delta_y) );
   return distance; 
}

void insert_points( set< pair<int, int> > & convex_hull, set< pair<int, int> > & points )
{
    set< pair<int, int> >::iterator it;
    set< pair<int, int> >::iterator it2;
    double lower_distance = DBL_MAX;
    pair<int, int> lower_point;

    for( it = convex_hull.begin() ; it != convex_hull.end() ; it++ ){
        lower_distance = DBL_MAX;
        if( points.size() != 0 )
        {
            for( it2 = points.begin() ; it2 != points.end() ; it2++ )
            {
                
                if( it++ == convex_hull.end() )
                {
                    it--;
                    set< pair<int, int> >::iterator it_begin = convex_hull.begin();
                    double distance_points_convex_hull = distance_between_points( make_pair(it->first,it->second), make_pair(it_begin->first,it_begin->second) );
                    double distance_E_N_1 = distance_between_points( make_pair(it->first,it->second), make_pair(it2->first,it2->second) );
                    double distance_E_N_2 = distance_between_points( make_pair(it2->first,it2->second), make_pair(it_begin->first,it_begin->second) );

                    double distance = ( distance_E_N_1 + distance_E_N_2 ) - distance_points_convex_hull;
                    if( distance < lower_distance )
                    {
                        lower_distance = distance;
                        lower_point.first = it2->first;
                        lower_point.second = it2->second;
                    }
                    cout << "cheguei no último ponto ele é: " << it->first << " " << it->second << endl;
                }
                else
                {
                    it--;
                    it++;
                    int p1 = it->first;
                    int p2 = it->second;
                    it--;
                    double distance_points_convex_hull = distance_between_points( make_pair(it->first,it->second), make_pair(p1,p2) );
                    double distance_E_N_1 = distance_between_points( make_pair(it->first,it->second), make_pair(it2->first,it2->second) );
                    double distance_E_N_2 = distance_between_points( make_pair(it2->first,it2->second), make_pair(p1,p2) );

                    double distance = ( distance_E_N_1 + distance_E_N_2 ) - distance_points_convex_hull;
                    if( distance < lower_distance )
                    {
                        lower_distance = distance;
                        lower_point.first = it2->first;
                        lower_point.second = it2->second;
                    }
                }
            }
            points.erase( lower_point );
            convex_hull.insert( lower_point );
            cout << "Estou inserindo: " << lower_point.first << " " << lower_point.second << endl;
        }
        else
        {
            return;
        }
    }
}

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
    cout << "ANTES" << endl;
    for (it = convex_hull.begin() ; it != convex_hull.end() ; it++ ) {
            cout<<"("<<it->first<<","<<it->second<<") \n";
    }
    /*cout << "-----" << endl;
    for (it = points.begin() ; it != points.end() ; it++ ) {
            cout<<"("<<it->first<<","<<it->second<<") \n";
    }*/
    // --- --- //

    insert_points( convex_hull, points );

    // --- DEBUG --- //
    cout << "Depois" << endl;
    for (it = convex_hull.begin() ; it != convex_hull.end() ; it++ ) {
            cout<<"("<<it->first<<","<<it->second<<") \n";
    }
    /*cout << "-----" << endl;
    for (it = points.begin() ; it != points.end() ; it++ ) {
            cout<<"("<<it->first<<","<<it->second<<") \n";
    }*/
    // --- --- //

    return 0;
}