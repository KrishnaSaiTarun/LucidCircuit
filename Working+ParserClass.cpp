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
		Node* d = NULL;
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
		  
		Node(int ID, Node* in1, Node* in2, Node* in3, Node* in4) : NodeId(ID), nodeType(2), a(in1), b(in2), c(in3), d(in4), zCount(0) {
		
			//cout << "Node ID: " << NodeId << " created" << endl;
		} 
	
		
};



class Graph: public Node
{

	public: 
		vector<Node*> nodeList;
		priority_queue<pair<int, int>> outputs;
		map<int, int> depth;
		
		unordered_set<int> type0notVisited;
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

/*void updateZ(Graph* g, int NodeId){

	Node* currentNode = g->nodeList[NodeId];
	
	//cout << "Current node is" << currentNode->NodeId << endl; 
	currentNode->zCount += 1;
	//cout << "Node " << NodeId << " z count is " << currentNode->zCount << endl;
}*/

void CreateLink(Graph* g, int sourceId, int sinkId, char port){
	
	Node* sinkNode = g->nodeList[sinkId];
	Node* sourceNode = g->nodeList[sourceId];
	
	if(port == 'a')
		sinkNode->a = sourceNode;
	else if(port == 'b')
		sinkNode->b = sourceNode;
	else if(port == 'c'){
		
		if(sinkNode->nodeType < 1){
			
			ClearAllMemory(g);
			cout << "Node is of wrong type" << endl;
			exit(1);
		}
		
		sinkNode->c = g->nodeList[sourceId];
	}
	
	sourceNode->zCount += 1;
	tuple<Node* , int, char> data(sinkNode, sinkId, port);
	sourceNode->outgoingEdges.push_back(data);
	
	
}

void DeleteLink(Graph* g, Node* source, Node* sink){

	for(auto it = source->outgoingEdges.begin(); it!=source->outgoingEdges.end(); it++){
	
		Node* nextNode = get<0>(*it);
		
		if(nextNode == sink){
			source->outgoingEdges.erase(it);
			source->zCount -= 1;
			break;
		}
	}
}



/*int getNodeType(string line){

	int nodeType = 0;
	for(int i = line.find_last_of(" ", line.length()-1) + 1; i < line.length(); i++){
	
			nodeType = nodeType*10 + (line[i] - '0');
	}
	
	return nodeType;	
		
}*/

/*int getNodeId(string line){

	int nodeId = 0;
	for(int i = line.find_first_of("N", 0) + 1; line[i] != ' '; i++){
	
			nodeId = nodeId*10 + (line[i] - '0');
	}
	return nodeId;
}*/

/*int getSourceNodeId(string line){

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
	
	//updateZ(g, sourceId);
	
	int sinkId = getSinkNodeId(line);
	char sinkPort = getSinkPort(line);
	
	//cout << " sink is " << sinkId << " and port is " << sinkPort << endl;
	
	CreateLink(g, sourceId, sinkId, sinkPort);
}*/

void InitializeNode(Node* node){

	node->a = node;
	node->b = node;
	if(node->nodeType > 0)
	node->c = node;
	if(node->nodeType > 1)
	node->d = node;
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

void CreateNodeType2(Graph* g, int nodeId){

	Node* newNode = new Node(nodeId, NULL, NULL, NULL, NULL);	
	InitializeNode(newNode);
	g->nodeList.push_back(newNode);
}



void CopyOutgoingEdges(Graph* g, Node* oldNode, Node* newNode){

	newNode->outgoingEdges = oldNode->outgoingEdges;
	newNode->zCount = oldNode->zCount;
	

}

void ChangeSourceForNext(Graph* g, Node* source, Node* sink, char port){

	if(port == 'a')
		sink->a = source;
	else if(port == 'b')
		sink->b = source;
	else if(port == 'c'){		
		sink->c = source;
	}

}

void ReplaceType1(Graph* g, Node* node){

	//First see its input a 
	Node* a = node->a;
	Node* b = node->b;
	Node* c = node->c;
	
	int newNodeId1 = g->nodeList.size();
	CreateNodeType0(g, newNodeId1);
	g->type0notVisited.insert(newNodeId1);
	Node* newNode1 = g->nodeList[newNodeId1];
	
	int newNodeId2 = g->nodeList.size();
	CreateNodeType0(g, newNodeId2);
	g->type0notVisited.insert(newNodeId2);
	Node* newNode2 = g->nodeList[newNodeId2];	
	
	if(a!=node){
	
		DeleteLink(g, a, node);
		CreateLink(g, a->NodeId, newNodeId1, 'a');	
	}
	
	if(b!=node){
		
		DeleteLink(g, b, node);
		CreateLink(g, b->NodeId, newNodeId1, 'b');
	}
	
	if(c!=node){
		
		DeleteLink(g, c, node);
		CreateLink(g, c->NodeId, newNodeId2, 'b');
	}
	
	CreateLink(g, newNodeId1, newNodeId2, 'a');

	
	CopyOutgoingEdges(g, node, newNode2);
		
	for(auto it = node->outgoingEdges.begin(); it!=node->outgoingEdges.end(); it++){
	
		Node* nextNode = get<0>(*it);
		int nextId = get<1>(*it);
		char nextPort = get<2>(*it);
		
		cout << nextId << "  "<< nextPort << endl;
		//CreateLink(g, newNodeId2, nextId, nextPort);
		//DeleteLink(g, node, nextNode);
		
		//Delete
		
		ChangeSourceForNext(g, newNode2, nextNode, nextPort);
	}
	
	
	g->nodeList[node->NodeId] = NULL;
	delete node;

}

void printGraph(Graph* logicGates){

	cout << endl << endl << "The graph is" << endl;
	
	for(int i = 0; i<logicGates->nodeList.size(); i++){
	
		if(logicGates->nodeList[i]!=NULL){
			
			Node* cur_node = logicGates->nodeList[i];
			
			cout << "Node ID is " << cur_node->NodeId << endl;
			//cout << "Type of Node " << i << " is "<< logicGates.nodeList[i]->nodeType << endl;
			cout << "Its port A is coming from NDOE " << cur_node->a->NodeId << endl;
			cout << "Its port B is coming from NDOE " << cur_node->b->NodeId << endl;
			if(cur_node->c != NULL)
			cout << "Its port C is coming from NDOE " << cur_node->c->NodeId << endl;
			if(cur_node->d != NULL)
			cout << "Its port D is coming from NDOE " << cur_node->d->NodeId << endl;
			cout << "Ouput going to " << cur_node->zCount << " nodes:" << endl;
			
			
			for(auto it = cur_node->outgoingEdges.begin(); it<cur_node->outgoingEdges.end(); it++){
				
				Node* next = get<0>(*it);
				cout << "To Node " << get<1>(*it) << " To port "<< get<2>(*it) << endl;
			}
		
		}
		cout << endl;
	}
	
	cout << endl << endl << endl;
}
bool isInput(Node* node){
	
	if(node->nodeType == 0){
	
		if(node->a != node || node->b != node) 
			return false;
	}
	
	if(node->nodeType == 2){
	
		if(node->a != node || node->b != node || node->c != node || node->d != node) 
			return false;
	}	
	return true; 
}

bool isOutput(Node* node){


	if(node->zCount != 0) return false;
	
	
	/// Check for node type as well later.
	
	
	return true;
}



void findDepthHelper(Graph* g, Node* node, int cur_depth){
	
	if(node->zCount == 0) return;
	
	stack<Node*> outgoing;
	for(auto it = node->outgoingEdges.begin(); it< node->outgoingEdges.end(); it++){
				
		Node* next = get<0>(*it);
		
		if(g->depth.find(next->NodeId) == g->depth.end())
			g->depth[next->NodeId] = cur_depth + 1;
		else 
			g->depth[next->NodeId] = max(g->depth[next->NodeId], cur_depth+1);
			
		outgoing.push(next);
		//cout << "To Node " << get<1>(*it) << " OR "<< next->NodeId << " To port "<< get<2>(*it) << endl;
	}
	
	while(!outgoing.empty()){
		Node* next = outgoing.top();
		
		findDepthHelper(g, next, cur_depth+1);
		
		outgoing.pop();
	}
	
	return;
}

void   findDepth(Graph* g){

	
	//Make a list of all the leaf nodes available first
	stack<Node*> inNodes;
	
	for(int i=0; i < g->nodeList.size(); i++){
	
		Node* curNode = g->nodeList[i];
		
		if(curNode != NULL){
		
			if(isInput(curNode)){
			
				inNodes.push(curNode);
			}
				
		}
	}
	
	
	while(!inNodes.empty()){
	
		Node* in = inNodes.top();
		int cur_depth = 0;
		g->depth[in->NodeId] = cur_depth;
		

		
		//cout << "The node is " << in->NodeId << "    Depth is" << cur_depth;
		findDepthHelper(g, in, cur_depth);
		

		inNodes.pop();
	}
	
	
	return;

}

void getOutputs(Graph* g){

	for(int i=0; i < g->nodeList.size(); i++){
	
		Node* curNode = g->nodeList[i];
		
		if(curNode != NULL){
		
			if(isOutput(curNode)){
			
				int NodeId = curNode->NodeId;
				g->outputs.push({g->depth[NodeId], NodeId});
			}
				
		}
	}
	
	return;
	
}

bool isValidNode(Node* node){

	if(node->nodeType != 0) return false;
	
	Node* aIn = node->a;
	Node* bIn = node->b;
	
	if(aIn == node || bIn == node) return false;
	
	if(aIn->zCount != 1 || bIn->zCount != 1) return false;
	
	//g->type0notVisited.erase(node->NodeId); 
	
	return true;
}

void replaceSet(Graph* g, Node* node){

	
	Node* Ina = node->a;
	Node* Inb = node->b;
	
	
	int newNodeId = g->nodeList.size();
	
	CreateNodeType2(g, newNodeId);
	
	//g->type0notVisited.insert(newNodeId1);
	Node* newNode = g->nodeList[newNodeId];
	
	if(Ina->a != Ina)
	newNode->a = Ina->a;		
	if(Ina->b != Ina)
	newNode->b = Ina->b;
	if(Inb->a != Inb)
	newNode->c = Inb->a;
	if(Inb->b != Inb)
	newNode->d = Inb->b;
	
	CopyOutgoingEdges(g, node, newNode);
	
	if(Ina->a != Ina){
	
		DeleteLink(g, Ina->a, Ina);
		CreateLink(g, Ina->a->NodeId, newNodeId, 'a');	
	}
	
	if(Ina->b != Ina){
		
		DeleteLink(g, Ina->b, Ina);
		CreateLink(g, Ina->b->NodeId, newNodeId, 'b');
	}
	
	if(Inb->a != Inb){
		
		DeleteLink(g, Inb->a, Inb);
		CreateLink(g, Inb->a->NodeId, newNodeId, 'c');
	}
	
	if(Inb->b != Inb){
		
		DeleteLink(g, Inb->b, Inb);
		CreateLink(g, Inb->b->NodeId, newNodeId, 'd');
	}
	
	//CopyOutgoingEdges(g, node, newNode2);
		
	for(auto it = node->outgoingEdges.begin(); it!=node->outgoingEdges.end(); it++){
	
		Node* nextNode = get<0>(*it);
		int nextId = get<1>(*it);
		char nextPort = get<2>(*it);
		
		cout << nextId << "  "<< nextPort << endl;
		//CreateLink(g, newNodeId2, nextId, nextPort);
		//DeleteLink(g, node, nextNode);
		
		//Delete
		
		ChangeSourceForNext(g, newNode, nextNode, nextPort);
	}
	

	
	g->depth[newNodeId] = max(g->depth[Ina->NodeId], g->depth[Ina->NodeId]);
	
	g->nodeList[node->NodeId] = NULL;
	g->nodeList[Ina->NodeId] = NULL;
	g->nodeList[Inb->NodeId] = NULL;
	
	g->depth.erase(node->NodeId);
	g->depth.erase(Ina->NodeId);
	g->depth.erase(Inb->NodeId);
	
	
	//g->type0notVisited.erase(node->NodeId);
	g->type0notVisited.erase(Ina->NodeId);
	g->type0notVisited.erase(Inb->NodeId);
	
	
	
	delete node;
	delete Ina;
	delete Inb;


}

int Traverse(Graph* g, int id){

	Node* node = g->nodeList[id];

	if(g->type0notVisited.find(id) != g->type0notVisited.end()){
	
		g->type0notVisited.erase(id); 
		
		if(isValidNode(node)){
	
			replaceSet(g, node);
			return id;
	
		}
	}
	
	
	priority_queue<pair<int, int>> orderedDfs;
	
	if(node->nodeType == 2){
	
		if(node->a != node)
			orderedDfs.push({g->depth[node->a->NodeId], node->a->NodeId});
		if(node->b != node)
			orderedDfs.push({g->depth[node->b->NodeId], node->b->NodeId});
		if(node->c != node)
			orderedDfs.push({g->depth[node->c->NodeId], node->c->NodeId});			
		if(node->d != node)
			orderedDfs.push({g->depth[node->d->NodeId], node->d->NodeId});	
	}
	
	else if(node->nodeType == 0){
	
		if(node->a != node)
			orderedDfs.push({g->depth[node->a->NodeId], node->a->NodeId});
		if(node->b != node)
			orderedDfs.push({g->depth[node->b->NodeId], node->b->NodeId});		
	}
	
	
	if(orderedDfs.size() == 0) return -1;
	
	while(orderedDfs.size() != 0){
	
		pair<int, int> p = orderedDfs.top();
		int nextId = p.second;
		
		int status = Traverse(g, nextId);
		if(status >= 0) return status;
		
		orderedDfs.pop();
	
	}
	
	return -1;	
	
}

void flushOutputs(Graph* g){

	g->outputs = priority_queue<pair<int, int>>();
}

void flushDepth(Graph* g){


	g->depth.clear();
}

void TraverseBack(Graph* g){

	//if(g->type0notVisited.size() == 0) return;
	
	while(!g->outputs.empty()){
	
		pair<int, int> p = g->outputs.top();
		cout << "Top of Heap is  " << p.second << endl;
	
		int outId = p.second;
	 
	 
	 
	 // If returned id -> replacement has been done 
	 // If returned -1 -> all ends reached -> remove this 
	 //  
		int removedId = Traverse(g, outId);
	
		cout << "The removed Id is   " << removedId << endl;
	
		if(removedId == -1) {
		
			g->outputs.pop();
		}
	
		else if(removedId >= 0){
			flushOutputs(g);
			flushDepth(g);
			break;	
		} 
	}
	
	
	
}
void printDepth(Graph* logicGates){

	cout<<endl << endl<< endl << "Let us print the depth" << endl;

	for(auto it = logicGates->depth.begin(); it!= logicGates->depth.end(); it++){

		cout << "Node Id " << it->first << "   Depth " << it->second << endl;
	}
	
}

void printNotVisited(Graph* logicGates){

	for(auto it = logicGates->type0notVisited.begin(); it!= logicGates->type0notVisited.end(); it++){
	
		cout << "Node Id " << *it << endl;
	}
}

class FileParser{


	private:
	
		int getSourceNodeId(string line){

			int sourceId = 0;
			
		//Just in case we have a type of double digital that might apprear in the future		
			for(int i = line.find_first_of("N", 0) + 1; line[i] != '.'; i++){

				sourceId = sourceId*10 + (line[i] - '0');
			}
			
			return sourceId;
	
		}

		int getSinkNodeId(string line){

			int sinkId = 0;
			
			//Just in case we have a type of double digital that might apprear in the future
			for(int i = line.find_last_of("N", line.length()-1) + 1; line[i] != '.'; i++){

				sinkId = sinkId*10 + (line[i] - '0');
			}
			
			return sinkId;
	
		}

		char getSinkPort(string line){

			return line[line.length()-1];
	
		}



		int getNodeType(string line){

			int nodeType = 0;
			
			//Just in case we have a type of double digital that might apprear in the future
				
			for(int i = line.find_last_of(" ", line.length()-1) + 1; i < line.length(); i++){
	
				nodeType = nodeType*10 + (line[i] - '0');
			}
	
			return nodeType;	
		
		}
		
		
		int getNodeId(string line){

			int nodeId = 0;
			
			//In case we have node IDs greater than 10
			for(int i = line.find_first_of("N", 0) + 1; line[i] != ' '; i++){
	
				nodeId = nodeId*10 + (line[i] - '0');
			}
			
			return nodeId;
		}
	
		
	public:
		int nodesCount = 0;
	
		int getNumNodes(ifstream& input){
			
			
			string lineInFile;
			getline(input, lineInFile);
			
			//In case our Node count is more than 10
			for(int i = lineInFile.find_last_of(" ", lineInFile.length()-1) + 1; i < lineInFile.length(); i++){
		
				nodesCount = nodesCount*10 + (lineInFile[i] - '0');
			}
			
			return nodesCount;
			
		}
		
		vector<pair<int, int>> getIdType(ifstream& input){
		
			vector<pair<int, int>> NodeId_type;
			string lineInFile;
			
			for(int i = 0; i<nodesCount; i++){
		
				getline(input, lineInFile);
				
				int nodeType = getNodeType(lineInFile);
				int nodeId = getNodeId(lineInFile);
				
				NodeId_type.push_back({nodeId, nodeType});
			
			}
			
			return NodeId_type;		
		
		}
		
		vector<tuple<int, int, char>> getEdges(ifstream& input){
			
			string line;
			vector<tuple<int, int, char>> edges;
			
			while(getline(input, line)){

				int sourceId = getSourceNodeId(line);
				//cout << "Parsed and got Source ID " << sourceId <<  endl;
	
				//updateZ(g, sourceId);
	
				int sinkId = getSinkNodeId(line);
				char sinkPort = getSinkPort(line);
				tuple<int, int, char> edge(sourceId, sinkId, sinkPort);
				edges.push_back(edge);
			}
			
			return edges;
	
	//cout << " sink is " << sinkId << " and port is " << sinkPort << endl;
			
			
		}

};

int main(int argc, const char* argv[]){

	
	cout << "Program Started.........." << endl;
	
	ifstream input_file(argv[1]);
	
	FileParser fp;
	
	//string lineInFile;
	//getline(input_file, lineInFile);
	
	//It is the first line which gives the number of nodes
	//int nodesCount = 0;
	//for(int i = lineInFile.find_last_of(" ", lineInFile.length()-1) + 1; i < lineInFile.length(); i++){
	//	nodesCount = nodesCount*10 + (lineInFile[i] - '0');
	//}
	string lineInFile;
	int nodesCount = fp.getNumNodes(input_file);
	
	//cout << nodesCount << endl;
	//int nodesCount = lineInFile[lineInFile.length()-1] - '0';
	
	vector<pair<int,int>> NodeId_type = fp.getIdType(input_file);
	
	
	
	
	
	
	//Create a graph
	Graph logicGates (nodesCount);
	
	//cout << "Graph Object Created......." << endl;
	
	/*for(int i = 0; i<nodesCount; i++){
		
		getline(input_file, lineInFile);
		int nodeType = getNodeType(lineInFile);
		
		//cout << "Type of Node " << i << " is "<< nodeType << endl;
		
		
		int nodeId = getNodeId(lineInFile);
		//cout << "Node Id retrieved is "<< nodeId << endl;
		
		if(nodeType == 0){
		
			CreateNodeType0(&logicGates, nodeId);
			logicGates.type0notVisited.insert(nodeId);
			}
			
		else if(nodeType == 1){
		
			CreateNodeType1(&logicGates, nodeId);
			}

	}*/
	
	
	for(auto it = NodeId_type.begin(); it != NodeId_type.end(); it++){
		
		int nodeId = it->first;
		int nodeType = it->second;

		if(nodeType == 0){
		
			CreateNodeType0(&logicGates, nodeId);
			logicGates.type0notVisited.insert(nodeId);
			}
			
		else if(nodeType == 1){
		
			CreateNodeType1(&logicGates, nodeId);
			}		
		
		
		
	}
	
	//cout << "The number of nodes in the Global List is " << logicGates.nodeList.size() << endl;
	
	vector<tuple<int, int, char>> edges = fp.getEdges(input_file);
	
	
	for(auto it = edges.begin(); it != edges.end(); it++){
	
		int sourceId = get<0>(*it);
		int sinkId = get<1>(*it);
		int sinkPort = get<2>(*it);
		
		CreateLink(&logicGates, sourceId, sinkId, sinkPort);
	
	}
	
	//while(getline(input_file, lineInFile)){
	
		//cout << lineInFile << endl;
		//ParseLineToEdge(&logicGates, lineInFile);	
		
	
		
	//}
	
	printGraph(&logicGates);
	
	cout << endl << endl << endl << endl << "Let us begin Part 1" << endl << endl;
	
	//Find a type 1 and replace with 2 type 0s
	//cout << logicGates.nodeList.size() << endl;
	
	for(int i = 0; i < nodesCount; i++){
	
		//cout << logicGates.nodeList.size() << endl;
		Node* curNode = logicGates.nodeList[i];
		
		if(curNode->nodeType == 0) continue;
		
		cout << "Found a Type 1" << endl; 
		
		ReplaceType1(&logicGates, curNode);
		
		
	
	}
	
	cout << endl << endl << endl;
	
	printGraph(&logicGates);
	
	
	
	// ---------------------------- PART 2 ------------------- PART 2 --------------------- PART 2 -----------------------
	
	cout << endl << endl << endl << endl << "PART 2" << endl;
	

	while(logicGates.type0notVisited.size()){
	
		findDepth(&logicGates);
		
		printDepth(&logicGates);
	
		getOutputs(&logicGates);

	
		printNotVisited(&logicGates);	
	
		TraverseBack(&logicGates);
		
		
	
		cout << endl << endl << endl;
		printGraph(&logicGates);
	
		printNotVisited(&logicGates);

		//printDepth(&logicGates);
		
		
	}
	
	
	
	cout << "Deleting everything now" << endl;
	
	for(int i=0; i<logicGates.nodeList.size(); i++){
		//cout << "Type of Node " << i << " is "<< logicGates.nodeList[i]->nodeType << endl;
		if(logicGates.nodeList[i]!=NULL)	
			delete logicGates.nodeList[i];
	}
	

	
}
