#include<iostream>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;

void linear_equation( pair<int,int> & pa, pair<int,int> & pb, int & delta_x, int & delta_y, double & constant_additive )
{
    int x1, y1, x2, y2;
    x1 = pa.first; y1 = pa.second;
    x2 = pb.first; y2 = pb.second;

    delta_x = x2 - x1;
    delta_y = y2 - y1;

    constant_additive = -( x1*y2 - y1*x2 );
}

int verify_location( pair<int, int> & points, int delta_x, int delta_y, int constant_additive )
{
    return ( ( points.first * delta_x ) + ( points.second * delta_y ) + ( constant_additive ) );
}

void fillSubSet( vector<pair<int, int>> & points, vector<pair<int, int>> & S1, vector<pair<int, int>> & S2, int delta_x, int delta_y, int constant_additive )
{
    int verifyLocation = 0;

    for( int i = 1 ; i < points.size() - 1 ; i++ )
    {
        verifyLocation = verify_location( points[i], delta_x, delta_y, constant_additive );
        if( verifyLocation > 0 )
        {
            S1.push_back( points[i] );
        }
        if( verifyLocation < 0 )
        {
            S2.push_back( points[i] );
        }
    }

}

int main( int argc, char** argv )
{
    ifstream points_file;
    int num_points = 0;
    int value;
    vector<int> aux;
    vector<pair<int,int>> points; //saves all points

    points_file.open( argv[1] );
    points_file >> num_points; //getting the number of points in file (first line of each file must contais this information)

    
    while( points_file >> value )
    {
        aux.push_back( value );
    }

    for( int i=0; i<aux.size(); i=i+2 )
    {
        points.push_back( make_pair( aux[i],aux[i+1] ) );
    }

    // -- Sortin points by first coordinate -- //
    sort( points.begin(), points.end() );


    // -- Get firs and last point -- //
    pair<int,int> pa = points[ 0 ];
    pair<int,int> pb = points[ points.size() - 1 ];

    // Fazer equação de reta entre ponto inicial e final
    int delta_x = 0;
    int delta_y = 0;
    double constant_additive = 0;

    linear_equation( pa, pb, delta_x, delta_y, constant_additive );

    vector<pair<int, int>> S1;
    vector<pair<int, int>> S2;

    fillSubSet( points, S1, S2, delta_x, delta_y, constant_additive );

    

    //lines for debug
            // cout<<" points \n";
            // for(int i=0; i<points.size(); i++){
            //     cout<<points[i].first <<" "<<points[i].second<<"\n";

            // }
            //    //cout<< distance_between_two_points(make_pair(2,-6),make_pair(7,3))<<endl;


    return 0;
}