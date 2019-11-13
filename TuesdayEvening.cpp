#include <iostream>
#include <fstream>
#include <bits/stdc++.h>

using namespace std;

/* 

	1. Parser logic in seperate class - []
	2. One virtual class
*/
class Node {

	public:
	
		Node* a = NULL;
		Node* b = NULL;
		Node* c = NULL;
		int zCount = 0;
		int nodeType = -1;
		int NodeId = -1;
		vector<tuple<Node*, int, char>> outgoingEdges;
		
		Node() {}
		
		Node(int ID, Node* in1, Node* in2) : NodeId(ID), nodeType(0), a(in1), b(in1), zCount(0) {
		
			//cout << "Node ID: " << NodeId << " created" << endl;
		}
		
		Node(int ID, Node* in1, Node* in2, Node* in3) : NodeId(ID), nodeType(1), a(in1), b(in2), c(in3), zCount(0) {
		
			//cout << "Node ID: " << NodeId << " created" << endl;
		}  
	
		
};



class Graph: public Node
{

	public: 
		vector<Node*> nodeList;
	
		int numNodes;
		
		Graph(int num) : numNodes(num) {
		
		//cout << "This graph has " << numNodes << " nodes" << endl;
		
		}
	


};

void ClearAllMemory(Graph* g){

	for(int i=0; i<g->nodeList.size(); i++){
		//cout << "Type of Node " << i << " is "<< logicGates.nodeList[i]->nodeType << endl;	
		if(g->nodeList[i] != NULL)
			delete g->nodeList[i];
	}
	
}

void updateZ(Graph* g, int NodeId){

	Node* currentNode = g->nodeList[NodeId];
	
	//cout << "Current node is" << currentNode->NodeId << endl; 
	currentNode->zCount += 1;
	//cout << "Node " << NodeId << " z count is " << currentNode->zCount << endl;
}

void updateNode(Graph* g, int sourceId, int sinkId, char port){
	
	Node* sinkNode = g->nodeList[sinkId];
	Node* sourceNode = g->nodeList[sourceId];
	
	if(port == 'a')
		sinkNode->a = g->nodeList[sourceId];
	else if(port == 'b')
		sinkNode->b = g->nodeList[sourceId];
	else {
		
		if(sinkNode->nodeType < 1){
			
			ClearAllMemory(g);
			cout << "Node is of wrong type" << endl;
			exit(1);
		}
		
		sinkNode->c = g->nodeList[sourceId];
	}
	
	sourceNode->outgoingEdges.push_back({sinkNode, sinkId, port});
	
	
}



int getNodeType(string line){

	int nodeType = 0;
	for(int i = line.find_last_of(" ", line.length()-1) + 1; i < line.length(); i++){
	
			nodeType = nodeType*10 + (line[i] - '0');
	}
	
	return nodeType;	
		
}

int getNodeId(string line){

	int nodeId = 0;
	for(int i = line.find_first_of("N", 0) + 1; line[i] != ' '; i++){
	
			nodeId = nodeId*10 + (line[i] - '0');
	}
	return nodeId;
}

int getSourceNodeId(string line){

	int sourceId = 0;
	for(int i = line.find_first_of("N", 0) + 1; line[i] != '.'; i++){

		sourceId = sourceId*10 + (line[i] - '0');
	}
	return sourceId;
	
}

int getSinkNodeId(string line){

	int sinkId = 0;
	for(int i = line.find_last_of("N", line.length()-1) + 1; line[i] != '.'; i++){

		sinkId = sinkId*10 + (line[i] - '0');
	}
	return sinkId;
	
}

char getSinkPort(string line){

	return line[line.length()-1];
	
}

void ParseLineToEdge(Graph* g, string line){

	int sourceId = getSourceNodeId(line);
	//cout << "Parsed and got Source ID " << sourceId <<  endl;
	
	updateZ(g, sourceId);
	
	int sinkId = getSinkNodeId(line);
	char sinkPort = getSinkPort(line);
	
	//cout << " sink is " << sinkId << " and port is " << sinkPort << endl;
	
	updateNode(g, sourceId, sinkId, sinkPort);
}

void InitializeNode(Node* node){

	node->a = node;
	node->b = node;
	if(node->nodeType == 1)
	node->c = node;
}

void ReplaceType1(Graph* g, Node* node){

	//First see its input a 
	Node* a = node->a;
	Node* b = node->b;
	Node* c = node->c;
	
	if(a!=node){
	
	
	}

}

//TODO move to one create(type)  
void CreateNodeType0(Graph* g, int nodeId){

	Node* newNode = new Node(nodeId, NULL, NULL);
	InitializeNode(newNode);
	g->nodeList.push_back(newNode);
}

void CreateNodeType1(Graph* g, int nodeId){

	Node* newNode = new Node(nodeId, NULL, NULL, NULL);	
	InitializeNode(newNode);
	g->nodeList.push_back(newNode);
}



int main(int argc, const char* argv[]){

	//Remember to change the input.txt to argv later.

	//Read the input from the input file
	ifstream input_file("input.txt");
	
	
	//Read the file line by line 
	string lineInFile;
	getline(input_file, lineInFile);
	
	//It is the first line which gives the number of nodes
	int nodesCount = 0;
	for(int i = lineInFile.find_last_of(" ", lineInFile.length()-1) + 1; i < lineInFile.length(); i++){
		nodesCount = nodesCount*10 + (lineInFile[i] - '0');
	}
	
	//int nodesCount = lineInFile[lineInFile.length()-1] - '0';
	
	//Create a graph
	Graph logicGates (nodesCount);
	
	//cout << "Graph Object Created......." << endl;
	
	for(int i = 0; i<nodesCount; i++){
		
		getline(input_file, lineInFile);
		int nodeType = getNodeType(lineInFile);
		
		//cout << "Type of Node " << i << " is "<< nodeType << endl;
		
		
		int nodeId = getNodeId(lineInFile);
		//cout << "Node Id retrieved is "<< nodeId << endl;
		
		if(nodeType == 0){
		
			CreateNodeType0(&logicGates, nodeId);
			}
			
		else if(nodeType == 1){
		
			CreateNodeType1(&logicGates, nodeId);
			}

	}
	
	cout << "The number of nodes in the Global List is " << logicGates.nodeList.size() << endl;
	
	while(getline(input_file, lineInFile)){
	
		//cout << lineInFile << endl;
		ParseLineToEdge(&logicGates, lineInFile);	
		
	
		
	}
	
	cout << "The graph is" << endl;
	
	for(int i = 0; i<logicGates.nodeList.size(); i++){
	
		if(logicGates.nodeList[i]!=NULL){
			
			Node* cur_node = logicGates.nodeList[i];
			
			cout << "Node ID is " << cur_node->NodeId << endl;
			//cout << "Type of Node " << i << " is "<< logicGates.nodeList[i]->nodeType << endl;
			cout << "A " << cur_node->a->NodeId << endl;
			cout << "B " << cur_node->b->NodeId << endl;
			if(cur_node->c != NULL)
			cout << "C " << cur_node->c->NodeId << endl;
			cout << "Ouput going to " << cur_node->zCount << " nodes" << endl;
			
			
			for(auto it = cur_node->outgoingEdges.begin(); it<cur_node->outgoingEdges.end(); it++){
				
				Node* next = get<0>(*it);
				cout << "To Node " << get<1>(*it) << " OR "<< next->NodeId << " To port "<< get<2>(*it) << endl;
			}
		
		}
	}
	
	cout << "Let us begin Part 1" << endl;
	
	//Find a type 1 and replace with 2 type 0s
	
	for(int i = 0; i < logicGates.nodeList.size(); i++){
	
		Node* curNode = logicGates.nodeList[i];
		
		if(curNode == NULL || curNode->nodeType == 0) continue;
		
		// Found a Type 1 
		
		ReplaceType1(&logicGates, curNode);
		
		
	
	}
	
	
	
	cout << "Deleting everything now" << endl;
	
	for(int i=0; i<logicGates.nodeList.size(); i++){
		//cout << "Type of Node " << i << " is "<< logicGates.nodeList[i]->nodeType << endl;	
		delete logicGates.nodeList[i];
	}
	

	
}

