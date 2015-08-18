//
// TSPImplementation.cpp
// Program written by Brady Sheehan
// implementation of a graph
// and a brute force solution to the traveling salesman problem
//
#include <iostream>
#include <vector>
#include <list>
#include <iomanip>
#include <cmath>
using namespace std;

class Vertex{
public:
	char id;
	int x, y;
	bool onTour;

	Vertex(){ //shouldn't ever happen since graph is built by hand
		cout << "Error: instance of Vertex created without id" << endl;
	}
	Vertex(char i, int x1, int y1){
		id = i;
		x = x1;
		y = y1;
		onTour = false;
	}
	bool operator==(Vertex &v){
		return (v.id == id);
	}
	friend ostream& operator<<(ostream&, const Vertex&);
	friend class Graph;
};

ostream& operator<<(ostream& out, const Vertex& v) {
	cout << v.id << " (" << v.x << "," << v.y << ")" << endl;
	return out;
}

class Graph{
public:
	vector<Vertex> V;
	list<Vertex*> tour;
	int numVertices;
	Graph(){
		Vertex a('a', 1, 1), b('b', 3, 1), c('c', 2, 2), d('d', 3, 2), e('e', 4, 2), f('f', 1, 3), g('g', 2, 3), h('h', 2, 4), i('i', 4, 4);
		V.push_back(a); //this is in zero
		V.push_back(b); //1
		V.push_back(c); //2
		V.push_back(d); //3
		V.push_back(e); //4
		V.push_back(f); //5
		V.push_back(g); //6
		V.push_back(h); //7
		V.push_back(i); //8
		numVertices = V.size();
		//nearestInsertionAlgorithm();
	}

	double pythagoreanThm(Vertex* a, Vertex* b){
		double x = a->x - b->x;
		double y = a->y - b->y;
		return pow((x*x + y*y), (1/2));
	}

	void nearestInsertionAlgorithm(){
		tour.push_back(&V[0]);		//initialize the tour
		tour.push_back(&V[0]);		// need to push the starting and stopping points onto tour
		(&V[0])->onTour = true;
		while (tour.size() <= numVertices){
			double currDist = 100000000;     //assign an abritrarily large number for comparison purposes
			Vertex *vi = 0;					 //how I will remember the vertex who is closest to the tour
			for (int i = 0; i < V.size(); i++){
				if (!(&V[i])->onTour){			    //if vertex is not on tour, check its distance compared to every vertex on the tour
					for (list<Vertex*>::iterator j = tour.begin(); j != tour.end(); j++){
						double tempDist = pythagoreanThm(&V[i], *j);
						if (tempDist < currDist){
							currDist = tempDist;	//update the current smallest distance
							vi = &V[i];				//update the closest vertex
						}
					}
				}
			}//end search for vi and vp
			(*vi).onTour = true;     //at this point we know that vi will be added to the tour, but we don't know where yet
			currDist = 100000000;   //assign an abritrarily large number for comparison purposes
			Vertex *vq = 0;
			Vertex *vr = 0;
			bool inserted = false;
			//we need to update the edges on the tour that minimize the distances with the newly included vertex
			for (list<Vertex*>::const_iterator i = tour.begin(), j = i; j != tour.end(); i++, j = i){
				//pick some vq and vr that are neighbors, thats i and i+1 
				j++;
				if (tour.size() == 2){ //there is only the begin/end vertices on the tour, so just insert vi between those two
					vq = *i;
					vr = *j;
					tour.insert(--tour.end(), vi);
					inserted = true;
					break;
				}
				if (j == tour.end())
					break;
				double tempDist = pythagoreanThm(*i, vi) + pythagoreanThm(vi, *j) - pythagoreanThm(*i, *j);
				if (tempDist < currDist){	//update the minimum distance to be included;
					currDist = tempDist;
					vq = *i;
					vr = *j;
				}
			}//end search for vq and vr;
			//now the order of vertices visited on the tour needs to be updated according to the new vp,vq,vi,and vr
			list<Vertex*>::const_iterator i;
			for (i = tour.begin();; i++){ //this finds where vr is in the list
				if (**i == *vr)
					break;
			}
			//now insert vi before wherever i points on the tour (the location of vr);
			if (!inserted){			//just basically want to insert vi between vq and vr;
				tour.insert(i, vi); //equivalent to tour = tour U {edge(vqvi), edge(vivr)} - {edge(vqvr)};
			}
			printTour();
		}
		printTour();
	}
	void printTour(){
		cout << "The optimal tour is: ";
		//tour.reverse();
		for (list<Vertex*>::const_iterator i = tour.begin(); i != tour.end(); i++){
			cout << (*i)->id << " ";
		}
		cout << endl;
	}
	friend ostream& operator<<(ostream&, const Graph&);
};

ostream& operator<<(ostream& out, const Graph& g) {
	cout << "Vertex and Position: " << endl;
	for (int i = 0; i < g.V.size(); i++){
		cout << g.V[i];
	}
	return out;
}

int main(){
	Graph g;
	cout << g << endl;
	g.nearestInsertionAlgorithm();
	return 0;
}