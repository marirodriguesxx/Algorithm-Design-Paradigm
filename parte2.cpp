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
#include <iterator>
#include<bits/stdc++.h>

using namespace std;


int get_side( pair<int, int> p1, pair<int, int>p2, pair<int, int> p3 ) {
    int det = ( p1.first * p2.second ) + ( p3.first * p1.second ) + ( p2.first * p3.second) 
                    - ( p3.first * p2.second ) - ( p2.first * p1.second) - ( p1.first * p3.second);
    if (det > 0) return 1; //left side
    if (det < 0) return -1; //right side
    return 0; //colinear
}

// return the distance between p3 and the line p1p2
double distance_point_to_line(pair<int,int> p1, pair<int,int> p2, pair<int,int> p3) { 
    int a = p2.second - p1.second;
    int b = p1.first - p2.first;
    int c = -(p1.first*p2.second -p1.second*p2.first );
    double distance = (abs(a*p3.first + b*p3.second + c )/sqrt(a*a + b*b));
    return distance;
}

void fill_s1_s2_subsets( set<pair<int,int>> points, set<pair<int,int>> &S1, 
                            set<pair<int,int>> &S2, pair<int, int> pa, pair<int, int> pb ){

    set<pair<int ,int>> :: iterator it;
    for( it = points.begin() ; it != points.end() ; it++ ) {
        int side = get_side( pa, pb, make_pair(it->first,it->second) );
        if( side == 1 ){
            S1.insert( make_pair(it->first,it->second) );
        }
        else if( side == -1 ){
            S2.insert( make_pair(it->first,it->second) );
        }
    }

}

void fill_in_subsets( set<pair<int,int>> points, set<pair<int,int>> &s1, 
                                    pair<int, int> pa, pair<int, int> pb, int flag) {
    // we just pick up the points that are above the line
    //the flag will tell us the righ correction
    // because if we are 
    set<pair<int ,int>> :: iterator it;
    // cout<<"line: "<<"("<<pa.first<<","<<pa.second<<") to "<<"("<<pb.first<<","<<pb.second<<")\n";
    for( it = points.begin() ; it != points.end() ; it++ ) {
        // cout<<" point: ("<<it->first<<","<<it->second<<") ";
        int side = get_side(pa,pb, make_pair(it->first,it->second));
        if (side*flag == 1) {
            // cout<<"is on left side\n";
            s1.insert(make_pair(it->first,it->second));
        }
        // else if (side*flag == -1) {
        //      cout<<"is on right side\n";
        // }
        // else{
        //     cout<<"is on the line \n";
        // }
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
        double distance_max = 0;
        pair<int,int> p_max;

        for( it = subset.begin() ; it != subset.end() ; it++ ){

            double distance_aux = distance_point_to_line(pa,pb,make_pair(it->first,it->second));

            // we just look for the poins with distance max and at the same side 
            if(distance_aux > distance_max ){
                distance_max = distance_aux;
                p_max = make_pair(it->first,it->second);
            }
        }

        // inserting max point in the convex hull
        convex_hull.insert(p_max);

        // now, we are going to fill the subsets
        set<pair<int,int>> s11;
        set<pair<int,int>> s22;

        fill_in_subsets(subset,s11, pa, p_max, side );
        fill_in_subsets(subset,s22, p_max, pb, side );

        // and after we call recursion to the new subsets finded
        convex_polygon( s11, convex_hull, pa, p_max, side );
        convex_polygon( s22, convex_hull, p_max, pb, side );
    }
    return 0;
}

void get_hamiltonian(vector<pair<int,int>> points, vector<int> &hamiltonian, set<pair<int,int>> convex_hull ) {

    vector<int>ec1;
    vector<int>ec2;

    set<pair<int ,int>> :: iterator it;
    set<pair<int ,int>> :: iterator end;
    vector<pair<int,int>>::iterator it2;

    pair<int,int> pa = make_pair(convex_hull.begin()->first,convex_hull.begin()->second);
    pair<int,int> pb;
    for (it = convex_hull.begin() ; it != convex_hull.end() ; it++ ) { 
        pb = make_pair(it->first,it->second);
        end = it;
    }

    it2 = std::find (points.begin(), points.end(), pa);
    int pa_index = std::distance(points.begin(),it2);
    
    it2 = std::find (points.begin(), points.end(), pb);
    int pb_index = std::distance(points.begin(),it2 );

    convex_hull.erase(convex_hull.begin());
    convex_hull.erase(end);

    for (it = convex_hull.begin() ; it != convex_hull.end() ; it++ ) {

        pair<int,int> point = make_pair(it->first,it->second);
        it2 = std::find (points.begin(), points.end(), point);
        int index = std::distance(points.begin(),it2);
        int side = get_side(pa,pb,point);

        if(side == 1){ ec1.push_back(index+2); }
        else if (side == -1 ){ ec2.push_back(index+2); }
    }
    
    hamiltonian.push_back(pa_index);
    for(int i=0; i<ec1.size(); i++){
        hamiltonian.push_back(ec1[i]);
    }
    hamiltonian.push_back(pb_index);
    for(int i=ec2.size()-1; i>=0; i--){
        hamiltonian.push_back(ec2[i]);
    }
    hamiltonian.push_back(pa_index);

}

void quick( set<pair<int,int>> points, set<pair<int,int>> &convex_hull ) { 
    if(points.size() > 3) {
        // set first and last point (pa and pb)
        set<pair<int ,int>> :: iterator it;
        pair<int,int> pa = make_pair(points.begin()->first,points.begin()->second);
        pair<int,int> pb;
        for (it = points.begin() ; it != points.end() ; it++ ) {
            // cout<<"("<<it->first<<","<<it->second<<")\n";
            pb = make_pair(it->first,it->second);
        }
   
        //these points are part of the convex hull !
        convex_hull.insert(pa);  
        convex_hull.insert(pb);


        set<pair<int,int>> s1;
        set<pair<int,int>> s2;

        // we look into all points and define s1 and s2
        // we also fill auxiliar vetor e1 and e2, we gonna use them to fill hamiltonian cicle
        fill_s1_s2_subsets(points,s1, s2, pa, pb );
                
        // we call the recursive function to get the convex hull
        // we call first for the left side
        convex_polygon( s1, convex_hull, pa, pb, 1 );
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
    set<pair<int,int>> points; //saves all points sorted
    vector<pair<int,int>> points_index; //saves all points to get they index later
    set<pair<int,int>> convex_hull; //will contain all the points of the convex  hull
    vector<int> hamiltonian;
    set < pair < int , int > > :: iterator it;

    data.open( argv[1] );
    data >> num_points; //getting the number of points in file (first line of each file must contais this information)
    
    while( data >> value ) {
        aux.push_back( value );
    }

    for( int i=0; i<aux.size(); i=i+2 ) {
        points.insert( make_pair( aux[i],aux[i+1] ) );
        points_index.push_back( make_pair( aux[i],aux[i+1] ) );
    }    

    quick( points, convex_hull );

    // creating input to third part
    string fileName("inputToThirdPart.txt");
    ofstream file_out;
    //file_out.open( fileName, std::ios_base::app );

    file_out.open(fileName);
    if( !file_out ) { 
      cout << "Error: file could not be opened" << endl;
      exit(1);
   }

    cout<<"Convex Hull with "<<convex_hull.size()<<" elements : \n";
    file_out<<convex_hull.size()<<"\n";
    for (it = convex_hull.begin() ; it != convex_hull.end() ; it++ ) {
            cout<<"("<<it->first<<","<<it->second<<") \n";
            file_out <<it->first<<" "<<it->second << "\n";
    }

    file_out.close();

    get_hamiltonian(points_index,hamiltonian,convex_hull);

    cout<<"\nHamiltonian cicle : \n";
    for(int i=0; i<hamiltonian.size(); i++){
        cout<<hamiltonian[i]<<" ";
    }
    cout<<endl;

    return 0;
}