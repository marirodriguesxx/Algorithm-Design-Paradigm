// this code was develop by Mariana Rdrigues (es98875) and Matheus Ximenes (es.....)
//its a implementation of the quick algorithmto get the convex hull from a set of points

#include<iostream>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include<algorithm>
#include<set>
#include<cmath>

using namespace std;

vector<int> hamiltonian;

int get_side( pair<int, int> p1, pair<int, int>p2, pair<int, int> p3 ) {
    int det = ( p1.first * p2.second ) + ( p3.first * p1.second ) + ( p2.first * p3.second) 
                    - ( p3.first * p2.second ) - ( p2.first * p1.second) - ( p1.first * p3.second);
    if (det > 0) return 1; //left side
    if (det < 0) return -1; //right side
    return 0; //colinear
}

int distance_point_to_line(pair<int,int> p1, pair<int,int> p2, pair<int,int> p3) { 
    int distance = abs ((p3.second - p1.second) * (p2.first - p1.first) 
                        - (p2.second - p1.second) * (p3.first - p1.first));
    return distance;
}

void fill_s1_s2_subsets( set<pair<int,int>> points, set<pair<int,int>> &S1, 
                            set<pair<int,int>> &S2, pair<int, int> pa, pair<int, int> pb, 
                                                        vector<int> &e1, vector<int> &e2 ){

    set<pair<int ,int>> :: iterator it;
    // the int i will help us to fill the hamiltonian cicle
    int i=2;
    for( it = points.begin() ; it != points.end() ; it++ ) {
        int side = get_side( pa, pb, make_pair(it->first,it->second) );
        if( side == 1 ){
            S1.insert( make_pair(it->first,it->second) );
            e1.push_back( i );
        }
        else if( side == -1 ){
            S2.insert( make_pair(it->first,it->second) );
            e2.push_back( i );
        }
        i++;
    }
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

void fill_in_subsets( set<pair<int,int>> points, set<pair<int,int>> &S1, 
                                    pair<int, int> pa, pair<int, int> pb) {
    // we just pick up the points that are above the line
    set<pair<int ,int>> :: iterator it;
    for( it = points.begin() ; it != points.end() ; it++ ) {
        int side = get_side( pa, pb, make_pair(it->first,it->second) );
        if( side == 1 )
            S1.insert( make_pair(it->first,it->second) );
    }
}

int convex_polygon( set<pair<int,int>>subset, set<pair<int,int>> &convex_hull,  
                                        pair<int,int> pa, pair<int,int> pb, int side ) {
    // if thres no elements in the set, we stop
    if(subset.size() == 0) {
        return -1;
    }
    else {

        set<pair<int ,int>> :: iterator it;
        int distance_max = 0;
        pair<int,int> p_max;

        for( it = subset.begin() ; it != subset.end() ; it++ ){

            int distance_aux = distance_point_to_line(pa,pb,make_pair(it->first,it->second));
            int get_side_aux = get_side(pa, pb, make_pair(it->first,it->second) );

            // we just look for the poins with distance max and at the same side 
            if(distance_aux > distance_max && get_side(pa, pb, make_pair(it->first,it->second) ) == side){
                distance_max = distance_aux;
                p_max = make_pair(it->first,it->second);
            }
        }

        it=subset.find(p_max);
        // we need to have sure that the point exists in the set to erase it 
        // after we inser the max point in the convex hull
        if (it != subset.end()) {
            subset.erase (it);
            convex_hull.insert(p_max);
        }
        // if the point dosent exists, we dont have more points to compare
        // so we need to stop the function
        else{
            return -1;
        }

        // now, we are going to fill the subsets
        set<pair<int,int>> s11;
        set<pair<int,int>> s22;

        fill_in_subsets(subset,s11, pa, p_max);
        fill_in_subsets(subset,s11, p_max, pb);

        // and after we call recursion to the new subsets finded
        convex_polygon( s11, convex_hull, pa, p_max, get_side(pa, p_max, pb) );
        convex_polygon( s22, convex_hull, p_max, pb, -get_side(p_max, pb, pa) );
    }
    return 0;
}

void quick( set<pair<int,int>> points, set<pair<int,int>> &convex_hull ) { 
    if(points.size() > 3) {
        // set first and last point (pa and pb)
        set<pair<int ,int>> :: iterator it;
        pair<int,int> pa = make_pair(points.begin()->first,points.begin()->second);
        pair<int,int> pb;
        for (it = points.begin() ; it != points.end() ; it++ ) {
            pb = make_pair(it->first,it->second);
        }
   
        //these points are part of the convex hull !
        convex_hull.insert(pa);  
        convex_hull.insert(pb);


        set<pair<int,int>> s1;
        set<pair<int,int>> s2;
        vector<int> e1;
        vector<int> e2;

        // we look into all points and define s1 and s2
        // we also fill auxiliar vetor e1 and e2, we gonna use them to fill hamiltonian cicle
        fill_s1_s2_subsets(points,s1, s2, pa, pb,e1,e2);
                
        // we call the recursive function to get the convex hull
        // we call first for the left side
        convex_polygon( s1, convex_hull, pa, pb, 1);
        // and before we call for the right side
        convex_polygon( s2, convex_hull, pa, pb, -1 );
    }
    else { 
        cout << "Convex hull not possible\n"; 
        return; 
    } 

}

int main( int argc, char** argv ) {
    ifstream data;
    int num_points = 0;
    int value;
    vector<int> aux;
    set<pair<int,int>> points; //saves all points
    set<pair<int,int>> convex_hull; //will contain all the points of the convex  hull
    set < pair < int , int > > :: iterator it;



    data.open( argv[1] );
    data >> num_points; //getting the number of points in file (first line of each file must contais this information)

    
    while( data >> value ) {
        aux.push_back( value );
    }

    for( int i=0; i<aux.size(); i=i+2 ) {
        points.insert( make_pair( aux[i],aux[i+1] ) );
    }    

    quick( points , convex_hull );

    cout<<"number of elements in convex hull: "<<convex_hull.size()<<endl;
    cout<<"Convex Hull :";
    for (it = convex_hull.begin() ; it != convex_hull.end() ; it++ ) {
            cout<<"("<<it->first<<","<<it->second<<") ";
        }
    cout<<endl;
    cout<<"Hamiltonian cicle :";
    for(int i=0; i<hamiltonian.size(); i++){
        cout<<hamiltonian[i]<<" ";
    }
    cout<<endl;

    return 0;
}