// 
// graphBlockDFS.cpp
// Program written by Brady Sheehan
// implementation of a graph and the Block Depth First Search Algorithm
// that finds biconnected components of a graph
//
#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

class Vertex{
public:
	char id, parent;
	int num, pred;
	vector<Vertex*> neighbors;

	Vertex(){ //shouldn't ever happen since graph is built by hand
		cout << "Error: instance of Vertex created without id" << endl;
	}
	Vertex(char i){
		id = i;
		num = 0;
		pred = 0;
		parent = '0';
	}
	bool operator==(Vertex v){
		return (v.id == id);
	}
	friend ostream& operator<<(ostream&, const Vertex&);
	friend class Graph;
};

ostream& operator<<(ostream& out, const Vertex& v) {
	out << v.id << " " << v.num << " " << v.pred << ": "; //first print the fields of the vertex
	for (int i = 0; i < v.neighbors.size(); i++){
		out << v.neighbors[i]->id << " "; //then prints neighbors of each vertex
	}
	out << " parent: " << v.parent << endl; //then prints that vertex's parent
	return out;
}
class Edge{
	int cost;
	Vertex* fromv;
	Vertex* tov;
public:
	Edge(){ //shouldn't ever happen since edges are only created whenever a to and from
			//vertex are defined
		cout << "Error: instance of Edge created without arguments" << endl;
	}
	Edge(Vertex &v1, Vertex &v2, int c = 0){
		cost = c;
		fromv = &v1;
		tov = &v2;
	}
	int getCost(){
		return cost;
	}
	Vertex* getFromVertext(){
		return fromv;
	}
	Vertex* getToVertex(){
		return tov;
	}
	bool operator==(Edge e){ 
		//returns false iff two edges do not each contain the same two vertices
		//this function compensates for edges that can attempt to be visited in reverse order
		return (((e.fromv == fromv) && (e.tov == tov)) || ((e.fromv == tov) && (e.tov == fromv)));
	}
	friend ostream& operator<<(ostream&, Edge&);
	friend class graph;
};

ostream& operator<<(ostream& out, Edge& e) {
	out << e.fromv->id << e.tov->id; //and edge is just v's id and u's id next to eachother
	return out;
}

class ConnectionsStack{
	vector<Edge> edges;
	vector<Edge> visited;
public:
	void push(Edge edge1){
		edges.push_back(edge1);
		visited.push_back(edge1);
	}
	Edge pop(){
		if(!edges.empty()){
			Edge temp = edges.back();
			edges.pop_back();
			return temp;
		}
		cout << "vector was empty " << endl;
	}
	int getSize(){
		return edges.size();
	}
	bool contains(Edge e){ //function to see if a vertex was visited or not
		for (int i = 0; i < visited.size(); i++){
			Edge temp = visited[i];
			if (temp == e)
				return true;
		}
		return false;
	}
	friend class Graph;
};

class Graph{
public:
	vector<Vertex> V;
	ConnectionsStack edges; 
	int counter; //num and pred counter
	int indent; //indentation counter
public:
	Graph(){
		Vertex a('a'), b('b'), c('c'), d('d'), e('e'), f('f'),g('g'),h('h');

		V.push_back(a); //this is in zero
		V.push_back(b); //1
		V.push_back(c); //2
		V.push_back(d); //3
		V.push_back(e); //4
		V.push_back(f); //5
		V.push_back(g); //6
		V.push_back(h); //7

		V[0].neighbors.push_back(&V[2]);
		V[0].neighbors.push_back(&V[3]);
		
		V[1].neighbors.push_back(&V[3]);
		V[1].neighbors.push_back(&V[5]);
		
		V[2].neighbors.push_back(&V[0]);
		V[2].neighbors.push_back(&V[4]);
		
		V[3].neighbors.push_back(&V[1]);
		V[3].neighbors.push_back(&V[0]);
		V[3].neighbors.push_back(&V[4]);
		V[3].neighbors.push_back(&V[5]);

		V[4].neighbors.push_back(&V[2]);
		V[4].neighbors.push_back(&V[3]);

		V[5].neighbors.push_back(&V[1]);
		V[5].neighbors.push_back(&V[3]);
		V[5].neighbors.push_back(&V[6]);
		V[5].neighbors.push_back(&V[7]);

		V[6].neighbors.push_back(&V[5]);
		V[7].neighbors.push_back(&V[5]);
	}

int min(int a, int b){ //returns the minimum of two integers, meant for minimum checks in blockDFS()
	if(a>=b)
		return b;
	else
		return a;
}

char minId(const Vertex* a, const Vertex* b){ //used for first minimum check with pred
											  //only meant for returning correct id to make code more readable
	if(a->pred>=b->pred)
		return b->id;
	else
		return a->id;
}

char minId2(const Vertex* a, const Vertex* b){ //used for second  minimum check with pred2
											   //only meant for returning correct id to make code more readable
	if(a->pred>=b->num)
		return b->id;
	else
		return a->id;
}
	void blockSearch(){
		for (int i = 0; i < V.size(); i++){ //initializes vertices.num = 0 for all vertices in the graph
			V[i].num = 0;
		}
		counter = 1;
		indent = 0;
		for (int j = 0; j < V.size(); j++){ //call blockDFS on all v.num == 0 vertices
			if (V[j].num == 0){
				blockDFS(&V[j]); //pass the address of each vertex in the vector of vertices
			}
		}
	}
	void blockDFS(Vertex *v){ //finds biconnected components of a graph
		//indent is a global variable that is decremented and incremented to, well, indent the 
		//code segments as the functions execute
		indent +=1;
		cout << setw (indent+3);
		cout << "DFS(" << v->id << ")" << endl;
		v->pred = v->num = counter++; //every recursive call, this counter gets incremented to adjust num and pred of vertices
		cout << setw (indent+4);
		cout << "num(" << v->id << ") = " << "pred(" << v->id << ") = " << v->pred << endl;
		for (int i = 0; i < v->neighbors.size(); i++){
			Vertex* u = v->neighbors[i]; //this is how we extract the neibors of v (which are adresses to the original vertices)
			u->parent = v->id; //every time we create the edge vu, v becomes u's parent
			Edge vu(*v, *u);
			cout << setw (indent+12);
			cout << "trying edge(" << vu << ")" << endl;
			if(!edges.contains(vu)){
				edges.push(vu);
				edges.visited.push_back(vu); //since the stack does not contain this edge, it must be marked as visited
				cout << setw (indent+10);
				cout << "push(edge(" << vu << "))" << endl;
			}
			if (u->num == 0){ 
				blockDFS(u); //recursive call for 
				indent -=1;
				if (u->pred >= v->num){  //if this is true, we have found a biconnected component and it will be printed
					Edge e = edges.pop(); 
					cout << setw (indent+7);
					cout << "BLOCK: " ;
					while (!(e == vu)){
						cout << "edge(" << e << ") ";
						e = edges.pop();
					}
					cout << "edge(" << e << ")" << endl;
				}
				//find minimum of v.pred and u.pred
				cout << setw (indent+4);
				v->pred = min(v->pred, u->pred);
				cout << "pred(" << v->id << ") = " << v->pred << " (= pred(" << minId(v,u) << "))" << endl;
			}
			else if (u->id != v->parent){ //u is not the parent of v
				//find minimum of v.pred and u.num
				v->pred = min(v->pred, u->num);
				cout  << setw (indent + 6) << "pred2(" << v->id << ") = " << v->pred << " (= pred(" << minId2(v,u) << "))" << endl;
			}
		}
	}
	friend ostream& operator<<(ostream&, const Graph&);
};

ostream& operator<<(ostream& out, const Graph& g) {
	for (int i = 0; i < g.V.size(); i++){
		out << g.V[i]; //prints each vertex in the vector of vertices
	}
	return out;
}

int main(){
	Graph g;
	cout << g << "\n" << endl;
	g.blockSearch();
	cout << "\n" << g << endl;
	return 0;
}