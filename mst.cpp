#include <iostream>
#include <cmath>
using namespace std;

// impossible weight
enum { IMP_WEIGHT = 1000001 };
// to store edges in the beginning arrays are used.
//
// a 2D-array meant to store initial edges set
// sorting is implemented by "heapsort", which sorts "rows" by third "column"
class EdgesSet{
	private:
		unsigned long size;
		unsigned long currentSize;
		long **edgesArr;
	public:
		EdgesSet(unsigned long);
		~EdgesSet();
		void addEdge(long, long, long);

	private:
		void swapEls(unsigned long, unsigned long);
		void sortTree(unsigned long);
	public:
		void sort();
		long getData(unsigned long, long) const;
		unsigned long getSize() const;
};

EdgesSet::EdgesSet(unsigned long sizeIn){
	size = sizeIn;
	currentSize = 0;
	edgesArr = new long*[sizeIn];
	for(unsigned long i = 0; i < sizeIn; i++){
		edgesArr[i] = NULL;
	}
}

EdgesSet::~EdgesSet(){
	for( unsigned long i = 0; i < size; i++){
		if(edgesArr[i]){
			delete [] edgesArr[i];
		}
	}
	delete [] edgesArr;
}

void EdgesSet::addEdge(long c1, long c2, long p){
	edgesArr[currentSize] = new long[3];
	edgesArr[currentSize][0] = c1;
	edgesArr[currentSize][1] = c2;
	edgesArr[currentSize][2] = p;
	currentSize++;
}

void EdgesSet::swapEls(unsigned long idx1, unsigned long idx2){
	long* tmp;
	tmp = edgesArr[idx1];
	edgesArr[idx1] = edgesArr[idx2];
	edgesArr[idx2] = tmp;
}

void EdgesSet::sortTree(unsigned long lastIdxIn){
	for(unsigned long i = lastIdxIn; i > 0; i--){
		if(edgesArr[i][2] < edgesArr[(i-1)/2][2]){
			swapEls(i, (i-1)/2);
		}
	}
}

void EdgesSet::sort(){
	for(unsigned long i = currentSize-1; i > 0; i--){
		sortTree(i);
		swapEls(0, i);
	}
}

long EdgesSet::getData(unsigned long i, long data) const{
	return edgesArr[i][data]; 	
}

unsigned long EdgesSet::getSize() const{
	return currentSize;
}

// A list which represents a stack, having access only to the "top", the end of the list and adding new elements to the end as well.
template<class T>
class List{
	public:
		struct Node{
			T value;
			Node* prev;
		};
	private:
		Node* top;
	public:
		List(){ top = NULL; }
		~List();
		void push(T el);
		Node* getTop(){ return top;}
};

template<class T>
List<T>::~List(){
	while(top){
		Node* tmp = top;
		top = top->prev;
		delete tmp;
	}
}

template<class T>
void List<T>::push(T el){
	Node* tmp = new Node;
	tmp->value = el;
	tmp->prev = top;
	top = tmp;
}


class Edge;

class Vertex{
	private:
		// number "name" of the vertex
		long name;
		// The overall number of edges incident on a particular vertex is unknown, so a list structure seems reasonable (compared to arrays);
		// The list contains only edges addresses, incident to this vertex, to avoid creating multiple instances of the same value;
		List<Edge*> edgesList;
		bool visited;
	public:
		Vertex(long nameIn) {
			name = nameIn;
			visited = false;
		}
		// An edge is added when it is already known (created somewhere else and has an address).
		void addEdge(Edge* edge){ edgesList.push(edge); }
		long getName(){ return name;}
		List<Edge*>& getEdgesList(){ return edgesList; }
		bool isVisited(){ return visited; }
		void setVisited(){ visited = true; }
		void setUnvisited(){ visited = false; }
};

// An Edge structure to store addresses of two vertices it connects and a weight of the edge;
class Edge{
	private:
		Vertex *vA;
		Vertex *vB;
		long weight;
	public:
		Edge(Vertex* vAIn, Vertex* vBIn, long weightIn){
			vA = vAIn;
			vB = vBIn;
			weight = weightIn;
		}
		long getWeight(){return weight;}
		Vertex* getVertA(){ return vA; }
		Vertex* getVertB(){ return vB; }
		long getVertAName(){ return vA->getName(); }
		long getVertBName(){ return vB->getName(); }
};

class MST{
	private:
		// A field to store overall size of the vertices array;
		int vSize;
		// A field to store current size (non-empty cells number) of the array to easily add new element to an empty place;
		int vCurrSize;
		int eSize;
		int eCurrSize;
		// array of pointers instead of objects to know for sure when vertex or edge does not exist, to avoid creating default constructors for the structures and creating empty objects just to initialize the array
		Vertex **verticesArr;
		Edge **edgesArr;
	public:
		MST(int, const EdgesSet&);
		~MST();
		void addEdge(long, long, long);
		Vertex* addVertex(long);
		Vertex* getVertAddress(long);
		void setAllUnvisited();
		long findBottleNeck(long, long);
		long findCoursesNum(long, long, unsigned int);
};

MST::MST(int vNum, const EdgesSet& es){
	vSize = vNum;
	vCurrSize = 0;
	eSize = vNum-1;
	eCurrSize = 0;
	verticesArr = new Vertex*[vNum];
	for(int i = 0; i < vNum; i++){
		verticesArr[i] = NULL;
	}
	edgesArr = new Edge*[eSize];
	for(int i = 0; i < eSize; i++){
		edgesArr[i] = NULL;
	}
	// fill mst with data
	for(unsigned long i = 0; i < es.getSize(); i++){
		long v1 = es.getData(i, 0);
		long v2 = es.getData(i, 1);
		long w = es.getData(i, 2);
		addEdge(v1, v2, w);
	}
}

MST::~MST(){
	for(int i = 0; i < vSize; i++){
		if(verticesArr[i])
			delete verticesArr[i];
	}
	delete [] verticesArr;
	for(int i = 0; i < eSize; i++){
		if(edgesArr[i])
			delete edgesArr[i];
	}
	delete [] edgesArr;
}

void MST::addEdge(long c1, long c2, long p){
	// edge can be created when vertices addresses are already known
	// vertices should be created only if they haven't been added yet
	Vertex* v1p = getVertAddress(c1); 
	Vertex* v2p = getVertAddress(c2);
	// If any of the vertices does not exist, NULL pointer will be returned;
	
	// If both vertices exist, that mean that the edge may create a cycle;
	if(v1p && v2p){
		// If bottleneck found, the path from c1 to c2 already exists and adding new edge will create a cycle
		// If start and end are the same point, IMP_WEIGHT will be returned and should be treated as a cycle as well
		long isCycle = findBottleNeck(c1, c2);
		setAllUnvisited();
		if(isCycle)
			return;
	}
	if(!v1p){
		v1p = addVertex(c1);
	}
	if(!v2p){
		v2p = addVertex(c2);
	}
	// Finally, when not existing vertices have been created, the edge to the array of edges may be added to MST
	edgesArr[eCurrSize] = new Edge(v1p, v2p, p);
	// The edge has been added, so now it should be reflected in each vertex it is incident on
	v1p->addEdge(edgesArr[eCurrSize]);
	v2p->addEdge(edgesArr[eCurrSize]);
	eCurrSize++;
}

Vertex* MST::addVertex(long vertName){
	verticesArr[vCurrSize] = new Vertex(vertName);
	vCurrSize++;
	return verticesArr[vCurrSize-1];
}

Vertex* MST::getVertAddress(long vertName){
	Vertex** vertArr = verticesArr;
	while(*vertArr){
		if((*vertArr)->getName() == vertName){
			return *vertArr;
		}
		vertArr++;
	}
	return NULL;
}

void MST::setAllUnvisited(){
	for(int i = 0; i < vCurrSize; i++){
		if(verticesArr[i])
			verticesArr[i]->setUnvisited();
	}
}

// A sort of recursive DFS
long MST::findBottleNeck(long start, long end){
	long res = 0;
	Vertex* startV = getVertAddress(start);	
	if(startV){
		startV->setVisited();
		// if "IMP_WEIGHT" is returned to the outside function, it should be interpreted as start and end are the same
		// else it will be returned to the previous findBottleNeck instance and mean that the point was found
		if(startV->getName() == end){
			return IMP_WEIGHT;
		}
		// go through each edge
		// Node(head) pointer defined for Edge* type
		List<Edge*>::Node *head = startV->getEdgesList().getTop();
		while(head){
			long weight = head->value->getWeight();
			Vertex* first = head->value->getVertA();
			Vertex* second = head->value->getVertB();
			if(!(first->isVisited())){
				res = findBottleNeck(first->getName(), end);
				if(res)
					return weight < res ? weight : res;
			}
			if(!(second->isVisited())){
				res = findBottleNeck(second->getName(), end);
				if(res)
					return weight < res ? weight : res;
			}
			head = head->prev;
		}
	}
	return res;
}

// A seperate function although findBottleNeck is enough to handle "setAllUnvisited" inside
long MST::findCoursesNum(long start, long end, unsigned int t){
		if(start == end)
			return 0;
		long bottleNeck = findBottleNeck(start, end) -1;
		setAllUnvisited();
		return (long)ceil((double)t/(bottleNeck));
}

int main(){
	// m - number of cities;
	// d - number of roads;
	int m;
	unsigned long d;
	// two cities' "names" and road weight
	long c1, c2, p;
	// t - number of passengers to transfer;
	unsigned int t;
	// s, e - start and end of pessengers transportation
	long s, e;

	cin >> m >> d;
	// create Edge set (array of edges) dx3, where d is number of edges, 3 stand for 2 vertices and one weight of the road between them
	EdgesSet es(d);
	// read in all edges
	for (unsigned long i = 0; i < d; i++){
		cin >> c1 >> c2 >> p;
		es.addEdge(c1, c2, p);
	}
	// sort descending
	es.sort();
	// create MST based on edges
	MST tree(m, es);
	// read in starting and ending point to transport the passengers
	cin >> s >> e;
	// OR since one of the the vertices may be 0 (?)
	while ( s || e ){
		cin >> t;
		// find path from s to t;
		// count number of trips;
		cout << tree.findCoursesNum(s, e, t) << endl;
		cin >> s >> e;
	}
	return 0;
}
