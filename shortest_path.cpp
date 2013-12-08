#include <iostream>
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

} Node;

typedef struct PathNode {
	char id;
	int shortest_path;

	char parent_id;	// previous node in shortest path that leads to this node

	inline bool operator< (const PathNode n) const {
		return ((shortest_path < n.shortest_path) ? true : false);
	}

	inline bool operator> (const PathNode n) const {
		return ((shortest_path > n.shortest_path) ? true : false);
	}
	
	inline bool operator== (const PathNode n) const {
		return ((shortest_path == n.shortest_path) ? true : false);
	}
} PathNode;

// FUNCTIONS
void process(char*);	// records edges, weights and nodes to form a graph

// the PathNode array will include the shortest paths when this function completes
void Dijkstra(char, vector< PathNode >& );
void printGraph();
void printShortestPaths( char, vector< PathNode >& );
// END FUNCTION DECLARATIONS

// GLOBAL VARIABLES
bool is_directed;			// set if the edges in the graph are directed
int weight_sum;				// the sum of all weights in the original graph
map< char, Node > graph; 	// the graph containing all the edges
vector< Edge > edges; 		// This will keep a list of edges and there weights
vector< PathNode > final_paths;
priority_queue< PathNode, std::vector<PathNode>, std::greater<PathNode> > heap;
// END GLOBAL DEFINITIONS

int main( int argc, char** argv) {

	char dij_source,	// the source node for running Dijkstra's Algorithm
		short_source;	// the start node for running Shortest Reliable Path

	dij_source = short_source = ' ';

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
	printGraph();

	// vector will hold every node's shortest path once Dijkstra has been ran
	vector< PathNode > shortest_paths;
	Dijkstra( dij_source, shortest_paths);

	// sent shortest path data to a file called output.txt
	printShortestPaths( dij_source, shortest_paths);
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

	cout << "My edges:\n";

	for(int i = 0; i < edges.size(); ++i)
		cout << edges[i].start_node << " -> " << edges[i].end_node 
			<< " with weight = " << edges[i].weight << endl;
}

void Dijkstra(char source, vector<PathNode>& final_paths) {

	// Initialize all the node's shortest paths to the sum of the weights
	// since this is the maximum possible path from any node to another
	// without forming a cycle
	map< char, Node >::iterator it = graph.begin();	

	for( ; it != graph.end(); ++it) {
		// take every key in the map and initalize the vector holding the final
		// paths.
		PathNode node;
		node.id = (char)it->first;
		node.parent_id = '~';	// default parent node character
		node.shortest_path = weight_sum;

		final_paths.push_back( node );
	} 
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

void printShortestPaths( char source, vector< PathNode >& s_paths) {

	cout << "Dijkstra\nSource: " << source << endl;

	for(int i = 0; i < s_paths.size(); ++i)
		cout << "NODE " << s_paths[i].id << ':' << s_paths[i].shortest_path
			<< '\n';

	cout << "End Dijkstra" << endl;
}

