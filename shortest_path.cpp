#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <fstream>

using std::cout;
using std::cin;
using std::endl; 
using std::ifstream;
using std::map;
using std::vector;
using std::string;
using std::priority_queue;

typedef struct Edge {
	char start_node,
		end_node;
	int weight;
} Edge;

typedef struct Node {

} Node;

// FUNCTIONS
void process(char*);
// END FUNCTION DECLARATIONS

// GLOBAL VARIABLES
bool is_directed;			// set if the edges in the graph are directed
map< char, Node > graph; 	// the graph containing all the edges
vector< Edge > edges; 		// This will keep a list of edges and there weights
std::priority_queue< int, std::vector<int>, std::greater<int> > heap;
priority_queue<int, std::vector<int>, std::greater<int> > queue;
// END GLOBAL DEFINITIONS

int main( int argc, char** argv) {

	string dij_source,	// the source node for running Dijkstra's Algorithm
		short_source;	// the start node for running Shortest Reliable Path

	dij_source = short_source = "";

	// attempt to open the file from user input
	ifstream file;
	char filename[80];

	cout << "-> Enter a filename that contains the graph.\n-> ";
	cin >> filename; 

	// check to make sure that the file is good
	file.open( filename );
	// if file will not open, then alert user and kill program
	if( !file.is_open() ) {
		cout << "An error ocurred while trying to read the file\n";
		return -1;
	}
	file.close();

	// begin processing the nodes and weights to form a graph

	cout << "-> The file has been processed.\n";
	process( filename );
}

void process(char* filename) {
	char i, j;	// vertex1, vertex2
	int w;		// weight of an edge
	string extra_text;

	ifstream file;
	file.open( filename );

	// clear the comments from the stream
	getline(file, extra_text);

	// if the file contains a '/n' between each line, clear it
	if( file.peek() == '\n')	file.ignore();

	// get the graph type from the stream
	getline(file, extra_text);

	while( file >> i >> j >> w) { 

		map< char, Node >::iterator it = graph.find( i );
		map< char, Node >::iterator it2 = graph.find( j );

		// add the new edge to the graph
		Edge e;
		
		e.start_node = i;
		e.end_node = j;
		e.weight = w;

		edges.push_back( e );

		
	}

	file.close();

	cout << "My edges:\n";

	for(int i = 0; i < edges.size(); ++i)
		cout << edges[i].start_node << " -> " << edges[i].end_node << " with weight = " << edges[i].weight << endl;
}

