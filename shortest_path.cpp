#include <iostream>
#include <stdlib.h>		// for atoi(char*)
#include <vector>
#include <map>
#include <queue>
#include <fstream>

using std::cout;
using std::cin;
using std::endl; 
using std::ifstream;
using std::ofstream;
using std::map;
using std::vector;
using std::string;
using std::priority_queue;

typedef struct Edge {
	char start_node,
		end_node;
	int weight;
	bool visited;
} Edge;

typedef struct Node {
	char id;
	vector<char> adj_nodes;

	int shortest_path;

	char parent_id;	// previous node in shortest path that leads to this node

	inline bool operator< (const Node* n) const {
		return ((shortest_path < n->shortest_path) ? true : false);
	}

	inline bool operator> (const Node* n) const {
		return ((shortest_path > n->shortest_path) ? true : false);
	}
	
	inline bool operator== (const Node* n) const {
		return ((shortest_path == n->shortest_path) ? true : false);
	}

} Node;

// FUNCTIONS
void process(char*);	// records edges, weights and nodes to form a graph

// the PathNode array will include the shortest paths when this function completes
void Dijkstra( char );
void printGraph();
void printShortestPaths( char );
int getDistance(char, char);	// returns the weight distance between 2 nodes
// END FUNCTION DECLARATIONS

// GLOBAL VARIABLES
bool is_directed;			// set if the edges in the graph are directed
int weight_sum;				// the sum of all weights in the original graph
map< char, Node > graph; 	// the graph containing all the edges
vector< Edge > edges; 		// This will keep a list of edges and there weights
priority_queue< Node*, std::vector<Node*>, std::greater<Node*> > minheap;
// END GLOBAL DEFINITIONS

int main( int argc, char** argv) {

	// check for [graph_file, start_node_dijk, start_node_shrt, k]
	if( argc < 5) {
		cout << "Usage: shortest_path.exe [graph_file.txt] [source_node]" 
			<< " [start_node for k] [k]\n";
		return 1;
	}

	char dij_source,	// the source node for running Dijkstra's Algorithm
		short_source;	// the start node for running Shortest Reliable Path

	int k = atoi(argv[4]);

	dij_source = argv[2][0];
	short_source =argv[3][0];

	// attempt to open the file from user input
	ifstream file;

	// check to make sure that the file is good
	file.open( argv[1] );
	// if file will not open, then alert user and kill program
	if( !file.is_open() ) {
		cout << "An error ocurred while trying to read the file\n";
		return -1;
	}
	file.close();

	// begin processing the nodes and weights to form a graph
	process( argv[1] );

	// will print originally recorded adjacency list
	//printGraph();

	// perform Dijkstra's algorithm on the graph
	Dijkstra( dij_source );

	// sent shortest path data to a file called output.txt
	printShortestPaths( dij_source );

	cout << "Execution Complete." << endl;
	
	return 0;
}

void process(char* filename) {
	char i, j;	// vertex1, vertex2
	int w;		// weight of an edge
	string extra_text;

	// this will serve as our "infinity" for the initial
	// shortest path weight
	weight_sum = 0;

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
		e.visited = false;

		edges.push_back( e );

		weight_sum += w;	// increase the sum of the 

		// now build an adjancency list from the node parsed

		// if inital vertex is not in the map then put it in and give it
		// a node with its one connectivity node, j
		if( it == graph.end() ) {
			Node n;
			n.id = i;

			graph[i] = n;			// add the node with the id as the key

			graph[i].adj_nodes.push_back(j);
		}

		// else, the key is already in the mapping, so add another
		// connectivity node to its vector
		else 
			graph[i].adj_nodes.push_back(j);

		// check to see if j is a key in the map
		// if not, then add it
		if( it2 == graph.end() ) {
			Node terminal;
			terminal.id = j;

			graph[j] = terminal;
		}

		// if the file contains a '/n' between each line, clear it
		if( file.peek() == '\n')	file.ignore();
	}
	file.close();
}

void Dijkstra(char source) {

	// Initialize all the node's shortest paths to the sum of the weights
	// since this is the maximum possible path from any node to another
	// without forming a cycle in our graph
	map< char, Node >::iterator it = graph.begin();
	map< char, Node >::iterator it2 = graph.begin();

	for( ; it != graph.end(); ++it) {
		// take every key in the map and initalize the vector holding the final
		// paths.
		Node *node;
		node = &it->second;
		node->parent_id = ' ';	// default parent node character

		// if the current node being initialized is the source, then set
		// the shortest path to itself to 0 
		it->second.shortest_path = node->shortest_path = 
			((node->id == source) ? 0 : weight_sum);

		//final_paths.push_back( node );

		minheap.push( node );
	}

	Node *current;
	char adj_node;
	int distance,
		new_distance;

	while( !minheap.empty() ) {
		
		current = minheap.top();
		minheap.pop();

		// locate node so we can get its adjacency list
		it = graph.find( current->id );

		for(int i = 0; i < it->second.adj_nodes.size(); ++i) {

			adj_node = it->second.adj_nodes[i];
			
			// find the node so we can update it if a shorter path is present
			it2 = graph.find( adj_node );

			// relax each edge connected to the current node iff it is less
			// than its current shortest path
			distance = getDistance(current->id, adj_node );
			new_distance = current->shortest_path + distance;

			if(new_distance < it2->second.shortest_path) {
				it2->second.shortest_path = new_distance;

				// we've found a new shortest path!
				it2->second.parent_id = current->id;
			}
		}
	}
}

int getDistance(char start, char end) {

	for(int i = 0; i < edges.size(); ++i) {
		if((start == edges[i].start_node) && (end == edges[i].end_node))
			return edges[i].weight;
	}

	return 0;	// if something goes wrong, just return 0
}

void printGraph() {

	map< char, Node >::iterator it = graph.begin();	    

	int vecSize = 0;

	cout << "Original Graph\n";
	for( ; it != graph.end(); ++it) {
		cout << it->first << ": ";

		vecSize = it->second.adj_nodes.size();
			
		// print adjacent nodes
		for(int i = 0; i < vecSize; ++i) {
			cout << it->second.adj_nodes[i] << ' ';
		}
		cout << endl;
	}
}

void printShortestPaths( char source ) {

	map< char, Node >::iterator it = graph.begin();

	cout << "Dijkstra\nSource: " << source << endl;

	for( ; it != graph.end(); ++it)
		cout << "NODE " << it->second.id << " : " << it->second.shortest_path
			<< '\n';

	cout << "End Dijkstra" << endl;
}

