Shortestpaths
=============

﻿James Powell (jep10g)
README file

This program can be compile with the included makefile. After navigating to the
proper directory containing the makefile and cpp file, the user will simply type
"make". This will generate a shortest_path.exe program file. This program expects
four command line parameters which will be the file name of the graph text file,
the source node to start the algorithm off with, the starting node for computing
the Shortest Reliable Path and an integer k.

I assume that the text file is in correct format and does not contain any other
data except for a one line comment, a identified that tells the program what kind
of graph to expect, the edges and weight represented by a single character for each 
node and a number for the weight of that edge as shown in the example files. This
time, I've coded this program to detect if spaces are between the lines, in case 
the test file(s) have them. This program will work on files on either.

Example of a graph excepted:
============================
# The one line comment
UD

A B 2
A Z 10
Z D 5
============================

I compiled the code using g++ -version 4.7.3.

Example:

	Let "d.txt" be the name of the file containing a directed graph to be computed.
	Also let 'A' be the source noded needed for Dijkstra's algorithm, 'B' be the
	start node needed for computing the Shortest Reliable Path and k = 10.

=============================================
	Commandline:
		make
		./shortest_path.exe d.txt A B 10
=============================================

Example of error handling:
=============================================

If a node is unreachable from a selected source, this program will alert you
by saying 'unreachable' where a value should be for the shortest paths.

	Consider the following input. We can see the if we use 'A' as a source, then
	nothing else can be reached in the graph because 'A' is not connected to
	anything except for itself.

	#comment
	D
	A A 2
	B C 2
	C D 1

	The output will be:

	Dijkstra
	Source: A
	NODE A : 0
	NODE B : Unreachable
	NODE C : Unreachable
	NODE D : Unreachable
	End Dijkstra


