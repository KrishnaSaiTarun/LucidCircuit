/* 		                                TASKS IMPLEMENTED

	1. File Parser logic in seperate class - []
	2. Geaph inhertis node and provides functionalities like bulid grpah and destroy graph
	3. Node class has it ports defined 
		- If there is no input to a port, then the input comes from itself
		- It has the number of outgoing edges, the list of them, NodeId, type 
	4. Part 1 is implemented using straight swap of type 1 nodes with 2 type 0 nodes
	5. Part 2 has the following algorithm
		-First find the critical path of the circuit
		-Traverse on that path to find for any reduction possible as our aim is to reduce the height (or depth) of the graph
		-Once a set of 3 rreplacable type 0 nodes are found, replace it and update the depths of the graph
		-Use a greedy approach to always replace a set from the critical path in order to priorotize height reduction.
		 
*/

/*                                              EXTRA TASKS I WANT TO IMPLEMENT

	1. Use a factory design principle for node creation. 
	2. Update the depths in a much efficient way (if the algorithm is correct)
	3. Add extra error handling with the input file.
	4. Use logger to implement easy debugging.
*/               



#include <iostream>
#include <fstream>
#include <bits/stdc++.h>

using namespace std;


class Node {


	private:
	
		// If no input then the input comes from itself
		void InitializeNode(Node* node){

			node->a = node;
			node->b = node;
			if(node->nodeType > 0)
			node->c = node;
			if(node->nodeType > 1)
			node->d = node;
		}
		
		
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
		
		Node(int ID, Node* in1, Node* in2) : NodeId(ID), nodeType(0), a(in1), b(in1), zCount(0) {}
		
		Node(int ID, Node* in1, Node* in2, Node* in3) : NodeId(ID), nodeType(1), a(in1), b(in2), c(in3), zCount(0) {}
		  
		Node(int ID, Node* in1, Node* in2, Node* in3, Node* in4) : NodeId(ID), nodeType(2), a(in1), b(in2), c(in3), d(in4), zCount(0) {}
		
		Node* CreateNodeType0(int nodeId){

			Node* newNode = new Node(nodeId, NULL, NULL);
			InitializeNode(newNode);
			return newNode;
		}

		Node* CreateNodeType1(int nodeId){

			Node* newNode = new Node(nodeId, NULL, NULL, NULL);	
			InitializeNode(newNode);
			return newNode;
		}

		Node* CreateNodeType2(int nodeId){

			Node* newNode = new Node(nodeId, NULL, NULL, NULL, NULL);	
			InitializeNode(newNode);
			return newNode;
		} 
		
		// Check If it is a valid Node set for replacement by a type 2
		bool isValidNode(Node* node){

			if(node->nodeType != 0) return false;
	
			Node* aIn = node->a;
			Node* bIn = node->b;
	
			if(aIn == node || bIn == node) return false;
	
			if(aIn->zCount != 1 || bIn->zCount != 1) return false;
	
	
			return true;
		}
		
		//Check if it is an input node
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
		
		//Check if it is an ouput node
		bool isOutput(Node* node){

			if(node->zCount != 0) return false;
	
			return true;
		}
	
		
};



class Graph: public Node
{	

	private:


		priority_queue<pair<int, int>> outputs;
		map<int, int> depth;
		int numNodes;
		vector<Node*> nodeList;
		unordered_set<int> type0notVisited;
	
	
		// Create a link in the graph and update the outgoing edges
		void CreateLink(int sourceId, int sinkId, char port){
	
			Node* sinkNode = nodeList[sinkId];
			Node* sourceNode = nodeList[sourceId];
	
			if(port == 'a')
				sinkNode->a = sourceNode;
			else if(port == 'b')
				sinkNode->b = sourceNode;
			else if(port == 'c'){
		
				if(sinkNode->nodeType < 1){
			
					ClearAllMemory();
					cout << "Node is of wrong type" << endl;
					exit(1);
				}
		
				sinkNode->c = nodeList[sourceId];
			}
	
			sourceNode->zCount += 1;
			tuple<Node* , int, char> data(sinkNode, sinkId, port);
			sourceNode->outgoingEdges.push_back(data);
	
		}
		
		//Delete a link in the graph and update the outgoing edges 
		void DeleteLink(Node* source, Node* sink){

			for(auto it = source->outgoingEdges.begin(); it!=source->outgoingEdges.end(); it++){
	
				Node* nextNode = get<0>(*it);
		
				if(nextNode == sink){
					source->outgoingEdges.erase(it);
					source->zCount -= 1;
					break;
				}
			}
		}

		// Copy data from a node being deleted
		void CopyOutgoingEdges(Node* oldNode, Node* newNode){

			newNode->outgoingEdges = oldNode->outgoingEdges;
			newNode->zCount = oldNode->zCount;

		}
		

		// Change source for a sink node when node replacement happens
		void ChangeSourceForNext(Node* source, Node* sink, char port){

			if(port == 'a')
				sink->a = source;
			else if(port == 'b')
				sink->b = source;
			else if(port == 'c'){		
				sink->c = source;
			}

		}
		
		//Helper to find depth of the graph
		void findDepthHelper(Node* node, int cur_depth){
	
			if(node->zCount == 0) return;
	
			stack<Node*> outgoing;
			for(auto it = node->outgoingEdges.begin(); it< node->outgoingEdges.end(); it++){
				
				Node* next = get<0>(*it);
		
				if(depth.find(next->NodeId) == depth.end())
					depth[next->NodeId] = cur_depth + 1;
				else 
					depth[next->NodeId] = max(depth[next->NodeId], cur_depth+1);
			
				outgoing.push(next);
			}
	
			while(!outgoing.empty()){
				Node* next = outgoing.top();
		
				findDepthHelper(next, cur_depth+1);
		
				outgoing.pop();
			}
	
			return;
		}

		// This will call the helper from a root node.
		void  findDepth(){

	
			//Make a list of all the leaf nodes available first
			stack<Node*> inNodes;
	
			for(int i=0; i < nodeList.size(); i++){
	
				Node* curNode = nodeList[i];
		
				if(curNode != NULL){
		
					if(isInput(curNode)){
			
						inNodes.push(curNode);
					}
				}
			}
	
	
			while(!inNodes.empty()){
	
				Node* in = inNodes.top();
				int cur_depth = 0;
				depth[in->NodeId] = cur_depth;
				findDepthHelper(in, cur_depth);
				inNodes.pop();
			}
	
			return;

		}
		
		//This is to get the list of all the root nodes to start the traversal.
		//Higher depth is given higher priority
		void getOutputs(){

			for(int i=0; i < nodeList.size(); i++){
	
				Node* curNode = nodeList[i];
		
				if(curNode != NULL){
		
					if(isOutput(curNode)){
			
						int NodeId = curNode->NodeId;
						outputs.push({depth[NodeId], NodeId});
					}
				
				}
			}
	
			return;
	
		}
		
		//Traverse the path in backward direction
		//OrderedDFS basically searches in the in the longest path first
		int Traverse(int id){

			Node* node = nodeList[id];

			if(type0notVisited.find(id) != type0notVisited.end()){
	
				type0notVisited.erase(id); 
		
				if(isValidNode(node)){
	
					replaceSet(node);
					return id;
	
				}
			}
	
	
			priority_queue<pair<int, int>> orderedDfs;
	
			if(node->nodeType == 2){
	
				if(node->a != node)
					orderedDfs.push({depth[node->a->NodeId], node->a->NodeId});
				if(node->b != node)
					orderedDfs.push({depth[node->b->NodeId], node->b->NodeId});
				if(node->c != node)
					orderedDfs.push({depth[node->c->NodeId], node->c->NodeId});			
				if(node->d != node)
					orderedDfs.push({depth[node->d->NodeId], node->d->NodeId});	
			}
	
			else if(node->nodeType == 0){
	
				if(node->a != node)
					orderedDfs.push({depth[node->a->NodeId], node->a->NodeId});
				if(node->b != node)
					orderedDfs.push({depth[node->b->NodeId], node->b->NodeId});		
			}
	
	
			if(orderedDfs.size() == 0) return -1;
	
			while(orderedDfs.size() != 0){
	
				pair<int, int> p = orderedDfs.top();
				int nextId = p.second;
		
				int status = Traverse(nextId);
				if(status >= 0) return status;
		
				orderedDfs.pop();
	
			}
	
			return -1;	
	
		}


		// Calls traverse from the root node from which it has to traverse
		void TraverseBack(){

			while(!outputs.empty()){
	
				pair<int, int> p = outputs.top();
	
				int outId = p.second;
	
			 // If returned id -> replacement has been done 
			 // If returned -1 -> all ends reached -> remove this from list and go to the root with next highest depth
			  
				int removedId = Traverse(outId);
	
				if(removedId == -1) {
		
					outputs.pop();
				}
	
				else if(removedId >= 0){
				
					cout << "The Removed Node is: " << removedId << endl;
					flushOutputs();
					flushDepth();
					break;	
				} 
			}
	
	
	
		}
		
		
		// TO print nodes that havent been visited
		/*void printNotVisited(){

			for(auto it = type0notVisited.begin(); it!= type0notVisited.end(); it++){
	
				cout << "Node Id " << *it << endl;
			}
		}*/
		
		// Clean up functions for our graph 
		
		void flushOutputs(){

			outputs = priority_queue<pair<int, int>>();
		}

		void flushDepth(){


			depth.clear();
		}
		
		void ClearAllMemory(){

			for(int i=0; i< nodeList.size(); i++){
				if(nodeList[i] != NULL)
					delete nodeList[i];
			}
	
		}
		
		
		
		
	public:

		Graph(int num) : numNodes(num) {}
		
		// Builds the graph with nodes and edges		
		void buildGraph(vector<pair<int,int>> NodeId_type, vector<tuple<int, int, char>> edges){
		
			for(auto it = NodeId_type.begin(); it != NodeId_type.end(); it++){
		
				int nodeId = it->first;
				int nodeType = it->second;

				if(nodeType == 0){
		
					Node* newNode = CreateNodeType0(nodeId);
					nodeList.push_back(newNode);
					type0notVisited.insert(nodeId);
					}
			
				else if(nodeType == 1){
		
					Node* newNode = CreateNodeType1(nodeId);
					nodeList.push_back(newNode);
					}		
			
			}
			
			
			for(auto it = edges.begin(); it != edges.end(); it++){
	
				int sourceId = get<0>(*it);
				int sinkId = get<1>(*it);
				int sinkPort = get<2>(*it);
		
				CreateLink(sourceId, sinkId, sinkPort);
	
			}
		}
		

		// Replacement of all type 1 nodes with two type 0 nodes each.
		void doPart1(int nodesCount){
		
			for(int i = 0; i < nodesCount; i++){

				Node* curNode = nodeList[i];
				if(curNode->nodeType == 0) continue;

				ReplaceType1(curNode);
			}
			
		}
		
		//Replace a set of 3 type 0 nodes with one type 2 if it is a valid set
		void doPart2(){
			
			while(type0notVisited.size()){
	
				findDepth();	
				getOutputs();	
				TraverseBack();
			}
		}
		
		
		// Making this public as a user can directly replace a node of type 1 if he knows where it is. 		
		void ReplaceType1(Node* node){

			//First see its inputs 
			Node* a = node->a;
			Node* b = node->b;
			Node* c = node->c;
	
			int newNodeId1 = nodeList.size();	
			Node* newNode1 = CreateNodeType0(newNodeId1);
			type0notVisited.insert(newNodeId1);
			nodeList.push_back(newNode1);
	
	
			int newNodeId2 = nodeList.size();	
			Node* newNode2 = CreateNodeType0(newNodeId2);
			type0notVisited.insert(newNodeId2);
			nodeList.push_back(newNode2);

			if(a!=node){
	
				DeleteLink(a, node);
				CreateLink(a->NodeId, newNodeId1, 'a');	
			}
	
			if(b!=node){
		
				DeleteLink(b, node);
				CreateLink(b->NodeId, newNodeId1, 'b');
			}
	
			if(c!=node){
		
				DeleteLink(c, node);
				CreateLink(c->NodeId, newNodeId2, 'b');
			}
	
			CreateLink(newNodeId1, newNodeId2, 'a');
			CopyOutgoingEdges(node, newNode2);
		
			for(auto it = node->outgoingEdges.begin(); it!=node->outgoingEdges.end(); it++){
	
				Node* nextNode = get<0>(*it);
				int nextId = get<1>(*it);
				char nextPort = get<2>(*it);

				ChangeSourceForNext(newNode2, nextNode, nextPort);
			}
	
			nodeList[node->NodeId] = NULL;
			
			cout<< "The removed Node is: " << node->NodeId << endl;
			delete node;

		}
		
			
		// Making this public as a user can directly replace a set if he knows a node is valid to be replaced. 
		void replaceSet(Node* node){

	
			Node* Ina = node->a;
			Node* Inb = node->b;
		
			int newNodeId = nodeList.size();	
			Node* newNode = CreateNodeType2(newNodeId);
			nodeList.push_back(newNode);

			if(Ina->a != Ina)
			newNode->a = Ina->a;		
			if(Ina->b != Ina)
			newNode->b = Ina->b;
			if(Inb->a != Inb)
			newNode->c = Inb->a;
			if(Inb->b != Inb)
			newNode->d = Inb->b;
	
			CopyOutgoingEdges(node, newNode);
	
			if(Ina->a != Ina){
	
				DeleteLink(Ina->a, Ina);
				CreateLink(Ina->a->NodeId, newNodeId, 'a');	
			}
	
			if(Ina->b != Ina){
		
				DeleteLink(Ina->b, Ina);
				CreateLink(Ina->b->NodeId, newNodeId, 'b');
			}
	
			if(Inb->a != Inb){
		
				DeleteLink(Inb->a, Inb);
				CreateLink(Inb->a->NodeId, newNodeId, 'c');
			}
	
			if(Inb->b != Inb){
		
				DeleteLink(Inb->b, Inb);
				CreateLink(Inb->b->NodeId, newNodeId, 'd');
			}
	
		
			for(auto it = node->outgoingEdges.begin(); it!=node->outgoingEdges.end(); it++){
	
				Node* nextNode = get<0>(*it);
				int nextId = get<1>(*it);
				char nextPort = get<2>(*it);
		
				ChangeSourceForNext(newNode, nextNode, nextPort);
			}
	

	
			depth[newNodeId] = max(depth[Ina->NodeId], depth[Ina->NodeId]);
	
			nodeList[node->NodeId] = NULL;
			nodeList[Ina->NodeId] = NULL;
			nodeList[Inb->NodeId] = NULL;
	
			depth.erase(node->NodeId);
			depth.erase(Ina->NodeId);
			depth.erase(Inb->NodeId);

			type0notVisited.erase(Ina->NodeId);
			type0notVisited.erase(Inb->NodeId);
		
			delete node;
			delete Ina;
			delete Inb;


		}
		
		//Print the graph whenever required to see how it looks like
		void printGraph(){

			cout << endl << "The graph is" << endl<< endl;
	
			for(int i = 0; i < nodeList.size(); i++){
	
				if(nodeList[i]!=NULL){
			
					Node* cur_node = nodeList[i];
			
					cout << "Node ID is " << cur_node->NodeId << endl;
					cout << "Type of Node is "<< cur_node->nodeType << endl;
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
					
					cout << endl;
				}	
			}
	
			cout << endl << endl;
		}
		
		//Clean up, removing all the allocated memory!!
		void destroyGraph(){
		
			for(int i = 0; i < nodeList.size(); i++){
		
				if(nodeList[i] != NULL)	
					delete nodeList[i];
			}		
		}
		

};


// Making file parsing a separate class will give it the ability to be used for any input file with same format for different programs

class FileParser{


	private:
	
	
		int nodesCount = 0;
		
		
		// I have done basic error handling 
		// I would like to add more like 
		int getSourceNodeId(string line, int nodes){
			
			
			if(line[0] != 'N'){
				
				cout << "Wrong input format: " << line << " : Usage example: N[Id].[Port] -> N[Id].[port]" << endl;
				exit(1);
			}
			int sourceId = 0;
			
		//Just in case we have a type of double digital that might apprear in the future		
			for(int i = line.find_first_of("N", 0) + 1; line[i] != '.'; i++){

				sourceId = sourceId*10 + (line[i] - '0');
			}
			
			if(sourceId < 0 || sourceId >= nodes){
			
				cout << "Error: " << line << " : Source Node does not exist" << endl;
				exit(1);
			}			
			
			return sourceId;
	
		}

		int getSinkNodeId(string line, int nodes){
			
			
			
			int sinkId = 0;
			
			//Just in case we have a type of double digital that might apprear in the future
			for(int i = line.find_last_of("N", line.length()-1) + 1; line[i] != '.'; i++){

				sinkId = sinkId*10 + (line[i] - '0');
			}
			
			if(sinkId < 0 || sinkId >= nodes){
			
				cout << "Error: " << line << " : Sink Node does not exist" << endl;
				exit(1);
			}
			return sinkId;
	
		}

		char getSinkPort(string line){
			
			//Assuming for now there are only 3 types of nodes, can be changed depending on the futrue 
			if(line[line.length()-1] >= 'a' || line[line.length()-1] <= 'd')
				return line[line.length()-1];
				
			else{
				cout << "Error : " << line << " : The last character should be a valid port : Usage example: N[Id].[Port] -> N[Id].[port]" << endl;
				exit(1);
			}
	
		}



		int getNodeType(string line){

			int nodeType = 0;
			
			//Just in case we have a type of double digital that might apprear in the future
				
			for(int i = line.find_last_of(" ", line.length()-1) + 1; i < line.length(); i++){
	
				nodeType = nodeType*10 + (line[i] - '0');
			}
			
			//For now let us assume we have only 2 types in the input and let us handle the error
			if(nodeType != 0 && nodeType != 1){
				
				cout << "Error : " << line << " : Node types 0 and 1 only allowed" << endl;
				exit(1);
			}
			return nodeType;	
		
		}
		
		
		int getNodeId(string line){

			int nodeId = 0;
			
			if(line[0] != 'N'){
				
				cout << "Wrong input format: " << line << " : Usage example: N[Id].[Port] -> N[Id].[port]" << endl;
				exit(1);
			}	
					
			//In case we have node IDs greater than 10
			for(int i = line.find_first_of("N", 0) + 1; line[i] != ' '; i++){
	
				nodeId = nodeId*10 + (line[i] - '0');
			}
			
			return nodeId;
		}
		
		//Checking the first line
		bool isNotValidFirstLine(string line){
		
		if(line[0] != 'n' || line[0] != 'N') return false;
		if(line[line.length()-1] > '9' || line[line.length()-1] < '0') return false;
		
		return true;	
			
		}
	
		
	public:
		void openFile(ifstream& input_file, string filename){
		
			input_file.open(filename);
	
			if (input_file.fail()) {
		
    			// file could not be opened
    				cout << "Error: File could not be opened / File Does not Exist: " << filename << endl; 
    				exit(1); 
			}			
		
		}
		
		//returns the number of nodes in the graph
		int getNumNodes(ifstream& input){
			

			string lineInFile;
			getline(input, lineInFile);
			
			if(isNotValidFirstLine(lineInFile)){
				
				cout << "Error: " << lineInFile << " Usage: n = [integer] or N = [integer] or Nodes = [integer] or nodes = [integer]" << endl;
				exit(1);
			}
			
			//In case our Node count is more than 10
			for(int i = lineInFile.find_last_of(" ", lineInFile.length()-1) + 1; i < lineInFile.length(); i++){
		
				nodesCount = nodesCount*10 + (lineInFile[i] - '0');
			}
			
			return nodesCount;
			
		}
		
		// retunrs the pair of Node Ids and its Type
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
		
		// Return the tuple of <source ID, sinkID, Sink PORT>
		vector<tuple<int, int, char>> getEdges(ifstream& input){
			
			string line;
			vector<tuple<int, int, char>> edges;
			
			while(getline(input, line)){

				int sourceId = getSourceNodeId(line, nodesCount);
				int sinkId = getSinkNodeId(line, nodesCount);
				char sinkPort = getSinkPort(line);
				
				tuple<int, int, char> edge(sourceId, sinkId, sinkPort);
				edges.push_back(edge);
			}
			
			return edges;
					
		}

};

/* 							The main does these: 

		1. Use parser object to get nodes and edges
		2. Build the graph with a graph object
		3. Call part 1 to replace all the type 1 nodes by two type 0 nodes
		4. Call part 2 to replace the set of three node 0s by a node 2 priorotizing height reduction

*/
int main(int argc, const char* argv[]){

	cout << "Program Started.........." << endl;
	
	//------------------------------------- Start with File Parsing --------------------------------------------
	ifstream input_file;	
	FileParser fp;	
	fp.openFile(input_file, argv[1]);

	//----------------------------1. Get Node Count, 2. Get the Node ID and Node Type 3. Get the edges with port.
	int nodesCount = fp.getNumNodes(input_file);	
	vector<pair<int,int>> NodeId_type = fp.getIdType(input_file);
	vector<tuple<int, int, char>> edges = fp.getEdges(input_file);
	
	//-------------------------------------Create a graph object and BUILD THE GRAPH ------------------------------
	Graph logicGates (nodesCount);	
	logicGates.buildGraph(NodeId_type, edges);
	logicGates.printGraph();
		
	
	//--------------------------------PART 1 : Find a type 1 and replace with 2 type 0s -------------------------
	
	cout << endl << endl << "PART 1" << endl << endl;
	logicGates.doPart1(nodesCount);
	cout << endl << endl;	
	logicGates.printGraph();
	
	// ---------------------------- PART 2 ------------------- PART 2 --------------------- PART 2 -----------------------
	
	cout << endl << endl << "PART 2" << endl << endl;
	logicGates.doPart2();	
	logicGates.printGraph();

	// ----------------------------------- Clear memory and exit gracefully -------------------------------------
	logicGates.destroyGraph();
	
}
