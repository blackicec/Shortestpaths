#include <iostream>
#include <vector>
#include <map>
#include <stack>
#include <fstream>

using std::cout;
using std::cin;
using std::endl; 
using std::ifstream;
using std::map;
using std::vector;
using std::stack;
using std::string;
using std::priority_queue<int, std::vector<int>, std::greater<int> >;

map< string, Node > graph; // the graph containing all the edges
std::priority_queue<int, std::vector<int>, std::greater<int> > heap;

int main( int argc, char** argv) {

	if( argc < 2 ) {
		cout << "Usage: \"shortest_path.exe [filename.ext]\"\n";
		return 1;
	}	

	ifstream file;
	file.open( argv[1] );

	// if file will not open, then alert user and kill program
	if( !file.is_open() ) {
		cout << "An error ocurred while trying to open the file\n";
		return -1;
	}
	
	string i, j;
	while( file >> i >> j ) { 

			map< string, Node >::iterator it = graph.find( i );
			map< string, Node >::iterator it2 = graph.find( j );

			// if inital vertex is not in the map then put it in and give it
			// a node with its one connectivity node, j
			if( it == graph.end() ) {
				Node n;
				n.value = i;
				n.index = -1;
				n.added = false;

				graph[i] = n;

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
					terminal.value = j;
					terminal.index = -1;
					terminal.added = false;

					graph[j] = terminal;
				}
	}

	file.close();
}

