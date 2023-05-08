#include <iostream>
using namespace std;
// Enum to set counterCWise() function, which determines, on which side a point lies, to inclue or exclude the point lying on the edge
enum class colinearResolve { dontInclude, includeIfCloser, justInclude };

class Point{
	private:
		int x;
		int y;
	public:
		Point();
		Point(int, int);
		Point(const Point&);
		Point(int);
		~Point();
		void setX(int);
		void setY(int);
		void set(int, int);
		void set(const Point&);
		int getX() const;
		int getY() const;
		Point& operator=(const Point&);

		long long int vecToPointLen();

		friend Point operator+(const Point&, const Point&);
		friend Point operator-(const Point&, const Point&);
		friend Point operator*(const Point&, const Point&);
		friend bool operator==(const Point&, const Point&);
		friend bool operator!=(const Point&, const Point&);
};

Point operator+(const Point&, const Point&);
Point operator-(const Point&, const Point&);
Point operator*(const Point&, const Point&);
bool operator==(const Point&, const Point&);
bool operator!=(const Point&, const Point&);

Point::Point(){
	x = 0;
	y = 0;
}

Point::Point(int xIn, int yIn){
	x = xIn;
	y = yIn;
}

Point::Point(const Point& coordsIn){
	x = coordsIn.getX();
	y = coordsIn.getY();
}

Point::Point(int num){
	x = num;
	y = num;
}

Point::~Point(){};

void Point::setX(int xIn){
	x = xIn;
}

void Point::setY(int yIn){
	y = yIn;
}

void Point::set(int xIn, int yIn){
	x = xIn;
	y = yIn;
}

void Point::set(const Point& coordsIn){
	x = coordsIn.getX();
	y = coordsIn.getY();
}

int Point::getX() const { return x; }

int Point::getY() const { return y; }

Point& Point::operator=(const Point& coordsIn){
	x = coordsIn.getX();
	y = coordsIn.getY();
	return *this;
}

long long int Point::vecToPointLen(){
	return (long long int)x * (long long int)x + (long long int)y * (long long int)y;
}

Point operator+(const Point& a, const Point& b){
	return Point(a.x + b.x, a.y + b.y);
}

Point operator-(const Point& a, const Point& b){
	return Point(a.x - b.x, a.y - b.y);
}

Point operator*(const Point& a, const Point& b){
	return Point(a.x * b.x, a.y * b.y);
}

bool operator==(const Point& a, const Point& b){
	return (a.x == b.x) && (a.y == b.y);
}

bool operator!=(const Point& a, const Point& b){
	return !((a.x == b.x) && (a.y == b.y));
}

template<class T>
class List{
	public:
		struct Node{
			T value;
			Node* prev = NULL;
			Node* next = NULL;

		};
	private:
		Node* top;
		Node* bottom;
		int size;
	public:
		List(){ top = NULL; bottom = NULL; size = 0; }
		~List();
		void push(const T el);
		T pop();
		T topVal();
		T nextToTop();
		int& getSize() { return size; }
		Node* getStackBottom(){ return bottom; }
		Node* getNextNode(Node* prevNode){
			if(prevNode)
				return prevNode->next;
			return NULL;
		}
		Node* removeNode(Node* nd){
			Node* nextNode = nd->next;
			if(nd->prev){
				nd->prev->next = nd->next;
			}else{
				bottom = nd->next;
			}
			if(nd->next){
				nd->next->prev = nd->prev;
			}else{
				top = nd->prev;
			}
			delete nd;
			size--;
			return nextNode;
		}
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
	tmp->next = NULL;
	if(top)
		top->next = tmp;
	else
		bottom = tmp;
	top = tmp;
	size++;
}

template<class T>
T List<T>::pop(){
	T val;
	Node* tmp = top;
	if(tmp){
		val = top->value;
		top = top->prev;
		top->next = NULL;
		delete tmp;
		size--;
	}
	return val;
}

template<class T>
T List<T>::nextToTop(){
	if(top){
		return top->prev ? top->prev->value : T(0);
	}
	return T(0);
}

template<class T>
T List<T>::topVal(){
	if(top){
		return top->value;
	}
	return T(0);
}


// The function accepts three points and enum to resolve the situation of point found of the edge of a line.
int counterCWise(Point O, Point a, Point b, colinearResolve colResolve){
	Point vecA = a - O, vecB = b - a;
	long long int aXbY = (long long int)vecA.getX() * (long long int)vecB.getY(); 
	long long int aYbX = (long long int)vecA.getY() * (long long int)vecB.getX(); 
	// if the vectors are colinear (their vector product == 0)
	if(aXbY == aYbX){
		switch (colResolve){
			case colinearResolve::includeIfCloser:
				// that point lays closer, which length (or its square) of the vector built from 0 point to that exact point is smaller
				return vecA.vecToPointLen() < vecB.vecToPointLen();
			case colinearResolve::justInclude:
				// if it should be just included irrespective of its distance from origin
				return 1;
			case colinearResolve::dontInclude:
				// if it should not be included in any case
				return 0;
		}
	}
	// The direction is counted based on the vector multiplication. If it is > 0 => the turn is a counter-clockwise turn.
	// Vector multiplication (for ex., for vectors a and b): a.x * b.y - b.x * a.y. Comparing it to zero and carrying over the ">" sign we receive:
	return aXbY > aYbX;
}

class Lanterns{
	private:
		Point* lArr;
		int size;

		// block copy and assignment
		Lanterns(Lanterns&);
		Lanterns& operator=(Lanterns&);

		void swapEls(int idx1, int idx2);
		void sortTree(int lastIdxIn);
	public:
		Lanterns(int);
		~Lanterns();
		int getSize() const;
		Point& operator[](int);

		void sort();
};

Lanterns::Lanterns(int sizeIn){
	size = sizeIn;
	lArr = new Point[sizeIn];
}	

Lanterns::~Lanterns(){
	delete [] lArr;
}

int Lanterns::getSize() const { return size; }

Point& Lanterns::operator[](int idx){
	return lArr[idx];
}

void Lanterns::swapEls(int idx1, int idx2){
	Point tmp;
	tmp = lArr[idx1];
	lArr[idx1] = lArr[idx2];
	lArr[idx2] = tmp;
}

void Lanterns::sortTree(int lastIdxIn){
	for(int i = lastIdxIn; i > 1; i--){
		// not i-1, because arr[1] is always the top of the tree	
		if(counterCWise(lArr[0], lArr[(i)/2], lArr[i], colinearResolve::includeIfCloser)){
			swapEls(i, (i)/2);
		}
	}
}

void Lanterns::sort(){
	int smallestX;
	int smallestIdx = 0;
	smallestX = lArr[0].getX();
	for(int i = 1; i < size - 1; i++){
		if( lArr[i].getX() < smallestX){
			smallestX = lArr[i].getX();
			smallestIdx = i;
		}
	}

	for(int i = 0; i < size - 1; i++){
		if( lArr[i].getX() == smallestX){
			if(lArr[i].getY() < lArr[smallestIdx].getY())
				smallestIdx = i;
		}
	}

	swapEls(0, smallestIdx);

	for(int i = size-1; i > 1; i--){
		sortTree(i);
		swapEls(1, i);
	}
}

// A fuction to create a list of points, belonging to convex hull, with an empty list received as an argument together with an array of points to analyze
void convexHull(List<Point> &lst, Lanterns &lns){
	lst.push(lns[0]);
	lst.push(lns[1]);
	for(int i = 2; i < lns.getSize(); i++){
		// while three points in a row don't make a left (counter-clockwise) turn, the second point should be removed and the three points should be checked again, with two previous points added to the list and the third, constant point, which is a candidate
		while(lst.getSize() > 1 && !counterCWise(lst.nextToTop(), lst.topVal(), lns[i%lns.getSize()], colinearResolve::dontInclude))
			lst.pop();
		// If the turn is finally left, the candidate can be added to the list. It may be removed on the following iterations if the next candidate generates a left turn with it.
		lst.push(lns[i]);
	}
}

// A function to find points inside a convex hull. Two arguments: a list of points, forming a convex hull, and an array of points to check.
int pointsInsideConvexHull(List<Point> &ch, Lanterns &points){
	// A list is created out of points array to easily remove points which are outside of the convex hull without having to shift the rest of the array.
	List<Point> pointsList;
	for(int i = 0; i < points.getSize(); i++){
		pointsList.push(points[i]);
	}

	// Get bottom (start) of the convex hull
	List<Point>::Node* stackBottom = ch.getStackBottom();
	// Two first points of the convex hull to build a vector
	Point firstHullPoint, secondHullPoint;
	// Go through all the points in convex hull
	while(stackBottom){
		firstHullPoint = stackBottom->value;
		// If any next points left in the stack
		if(stackBottom->next)
			secondHullPoint = stackBottom->next->value;
		else
			// Get the first point of the hull again to enclose it and check the last part of it
			secondHullPoint = ch.getStackBottom()->value;

		// We need to check all the small lanterns for each section of the convex hull 
		// Get the first small lantern
		List<Point>::Node* nextNode = pointsList.getStackBottom();
		// check if any of the points lie to the right of the current hull part while there are any lanterns left in the stack
		while(nextNode){
			// if it lies to the right
			if(!counterCWise(firstHullPoint, secondHullPoint, nextNode->value, colinearResolve::justInclude)){
				// remove the lantern and get next one to check
				nextNode = pointsList.removeNode(nextNode);
				continue;
			}
			// get next lantern to check
			nextNode = pointsList.getNextNode(nextNode);
		}
		// check another section of convex hull
		stackBottom = stackBottom->next;
	}
	// return a size of the stack of lanterns left after the check
	return pointsList.getSize();
}

int main(){

	int L, S, x, y, smallLanternsNumInside;
	cin >> L;
	Lanterns largeLanterns(L);
	for(int i = 0; i < L; i++){
		cin >> x >> y;
		largeLanterns[i] = Point(x, y);
	}
	largeLanterns.sort();

	// create an empty list for points belonging to convex hull
	List<Point> lpList;
	// fill it
	convexHull(lpList, largeLanterns);


	cin >> S;
	Lanterns smallLanterns(S);
	for(int i = 0; i < S; i++){
		cin >> x >> y;
		smallLanterns[i] = Point(x, y);
	}

	// get number of small numbers inside the convex hull
	smallLanternsNumInside = pointsInsideConvexHull(lpList, smallLanterns);
	cout << smallLanternsNumInside << endl;

	return 0;
}
