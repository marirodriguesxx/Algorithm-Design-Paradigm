#include<iostream>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include<algorithm>
#include<cmath>

using namespace std;

vector<int> hamiltonian;

void linear_equation( pair<int,int> & pa, pair<int,int> & pb, int & delta_x, int & delta_y, double & additive_constant )
{
    int x1, y1, x2, y2;
    x1 = pa.first; y1 = pa.second;
    x2 = pb.first; y2 = pb.second;

    delta_x = x2 - x1;
    delta_y = y2 - y1;

    additive_constant = -( x1*y2 - y1*x2 );
}

int is_on_left( pair<int, int> & p1, pair<int, int> & p2, pair<int, int> & p3 )
{
    int det = ( p1.first * p2.second ) + ( p3.first * p1.second ) + ( p2.first * p3.second) - ( p3.first * p2.second ) - ( p2.first * p1.second) - ( p1.first * p3.second);
    return det;
}

void fill_in_subsets(vector<pair<int, int>> &points, vector<pair<int, int>> &S1, vector<pair<int, int>> &S2, pair<int, int> &pa, pair<int, int> &pb){
    for( int i = 1 ; i < points.size() - 1 ; i++ )
    {
        int verify_subset = is_on_left( pa, pb, points[i] );
        if( verify_subset > 0 )
        {
            S1.push_back( points[i] );
        }
        else if( verify_subset < 0 )
        {
            S2.push_back( points[i] );
        }
    }
}

void fill_in_subset(vector<pair<int, int>> points, vector<pair<int, int>> &S1, pair<int, int> pa, pair<int, int> pb){
    for( int i = 0 ; i < points.size()  ; i++ )
    {
        int verify_subset = is_on_left( pa, pb, points[i] );
        if( verify_subset > 0 )
        {
            S1.push_back( points[i] );
        }
    }
}

void get_hamiltonian(vector<pair<int, int>> &points, pair<int, int> &pa, pair<int, int> &pb, vector<int> &e1, vector<int> &e2){
    for( int i = 1 ; i < points.size() - 1 ; i++ )
    {
        int verify_subset = is_on_left( pa, pb, points[i] );
        if( verify_subset > 0 )
        {
            e1.push_back( i+1 );
        }
        else if( verify_subset < 0 )
        {
            e2.push_back( i+1 );
        }
    }
    // cout<<"e1 size: "<<e1.size()<<endl;
    // cout<<"e2 size: "<<e2.size()<<endl;

    hamiltonian.push_back(1);
    for(int i=0;i<e1.size();i++){
        hamiltonian.push_back(e1[i]);
    }
    hamiltonian.push_back(points.size());
    for(int i=e2.size()-1;i>=0;i--){
        hamiltonian.push_back(e2[i]);
    }
    hamiltonian.push_back(1);
}

double distance_point_to_line(int dx, int dy, int additive_constant, pair<int,int>point)
{
    double numerator = abs( (dy*point.first) + (dx*point.second) + additive_constant);
    double denominator = sqrt( (dx*dx) + (dy*dy) );
    double distance = numerator/denominator;
    return distance;
}

void convex_polygon(int &dx, int &dy, double &additive_constant, vector<pair<int,int>> subset, vector<pair<int, int>> & convex_hull,  pair<int,int> pa, pair<int,int> pb ){
    if(subset.size() == 0)
    {
        cout<<"aqui\n";
        return;
    }
    else{
        // -- finding point most disntace of papb
        int distance_max = 0;
        pair<int,int> p_max;
        for(int i=0; i<subset.size(); i++){
            int distance_aux = distance_point_to_line(dx,dy,additive_constant,subset[i]);
            if(distance_aux > distance_max){
                distance_max = distance_aux;
                p_max = subset[i];
            }
        }
        convex_hull.push_back(p_max);
        cout<<"p_max : "<<"("<<p_max.first<<","<<p_max.second<<  ")\n";

        vector<pair<int, int>> S11;
        vector<pair<int, int>> S22;

        fill_in_subset(subset,S11, pa, p_max);
        fill_in_subset(subset,S22, p_max, pb);
        //lines for debug
            cout<<"S11: ";
            for(int i=0; i<S11.size(); i++)
                cout<<"("<<S11[i].first<<","<<S11[i].second<<  ") ";
            cout<<endl;
            cout<<"S22: ";
            for(int i=0; i<S22.size(); i++)
                cout<<"("<<S22[i].first<<","<<S22[i].second<<") ";
            cout<<endl;

        linear_equation( pa, p_max, dx, dy, additive_constant );
        convex_polygon(dx , dy , additive_constant , S11, convex_hull, pa, p_max);
        linear_equation( p_max, pb, dx, dy, additive_constant );
        convex_polygon(dx , dy , additive_constant , S22, convex_hull, p_max, pb);
        // cout << "point ("<<p_max.first<<","<<p_max.second<<") have max distance "<<distance_max <<endl;
    }
}

void quick( vector<pair<int, int>> & points, vector<pair<int, int>> & convex_hull )
{   
    if(points.size() >= 2){
    // -- Get first and last point -- //
    pair<int,int> pa = points[ 0 ];
    pair<int,int> pb = points[ points.size() - 1 ];    
    cout<<"pa: "<<"("<<pa.first<<","<<pa.second<<  ") "<<"- pb: "<<"("<<pb.first<<","<<pb.second<<  ")\n ";
    convex_hull.push_back(pa);  //these points are part of the convex hull !
    convex_hull.push_back(pb);

    // -- Finding Linear Equantion of the points PA and PB --
    int dx = 0;
    int dy = 0;
    double additive_constant = 0;

    linear_equation( pa, pb, dx, dy, additive_constant );

    vector<pair<int, int>> S1;
    vector<pair<int, int>> S2;
    vector<int> e1;
    vector<int> e2;

    // -- we look into all points, excep the first and the last point (pa and pb)     
    fill_in_subsets(points,S1, S2, pa, pb);
    get_hamiltonian(points,pa,pb,e1,e2);
    // cout<<"s1 size: "<<S1.size()<<endl;
    // cout<<"s2 size: "<<S2.size()<<endl;
        
    convex_polygon(dx , dy , additive_constant , S1, convex_hull, pa, pb);
    convex_polygon(dx , dy , additive_constant , S2, convex_hull, pa, pb);
    //lines for debug
            // cout<<"S1: ";
            // for(int i=0; i<S1.size(); i++)
            //     cout<<"("<<S1[i].first<<","<<S1[i].second<<  ") ";
            // cout<<endl;
            // cout<<"S2: ";
            // for(int i=0; i<S2.size(); i++)
            //     cout<<"("<<S2[i].first<<","<<S2[i].second<<") ";
            // cout<<endl;
    }

}

int main( int argc, char** argv )
{
    ifstream points_file;
    int num_points = 0;
    int value;
    vector<int> aux;
    vector<pair<int,int>> points; //saves all points
    vector<pair<int,int>> convex_hull; //will contain all the points of the convex  hull

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

    // lines for debug
            cout<<" points \n";
            for(int i=0; i<points.size(); i++){
                cout<<"("<<points[i].first<<","<<points[i].second<<")\n";

            }
    // cout<<"total points = "<<points.size()<<endl;

    quick( points , convex_hull );

    cout<<"points in convex hull = "<<convex_hull.size()<<endl;
    cout<<"Convex Hull :";
    for(int i=0; i<convex_hull.size(); i++){
        cout<<"("<<convex_hull[i].first<<","<<convex_hull[i].second<<") ";
    }
    cout<<endl;
    cout<<"Hamiltonian cicle :";
    for(int i=0; i<hamiltonian.size(); i++){
        cout<<hamiltonian[i]<<" ";
    }
    cout<<endl;

    return 0;
}