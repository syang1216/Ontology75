#include <map>
#include <fstream>
#include <iostream>
#include <stack>
#include <string>
#include <cstdlib>
#include <cstring>

#define ALPHABET 54
//0-25 a-z, 26-51 A-Z, 52 space 52 qustion mark

using namespace std;

struct TrieNode{
	//TrieNode* child[ALPHABET]; 
	map<char, TrieNode*> *childmap;
	//pair<char, TrieNode*> pr;
	//TrieNode* next;
	int occur;
};

struct TreeNode{
	char name[10];
	TreeNode* next;
	TreeNode* children;
	TreeNode* parent;
	TrieNode* questions;
};



/* builds the try with the given string */
void buildTrie(string str, TrieNode* questions){
//	TrieNode* thisTrie = questions;

	for(int i=0;i<str.length();i++){
	//	map<char, TrieNode*>* cmap = questions->childmap;
		if(!(questions->childmap)){
			questions->childmap = new map<char, TrieNode*>;
		}
	//	map<char,TrieNode*> cmap = *(questions->childmap);
		if(!(*(questions->childmap))[str[i]]){
			struct TrieNode* next = (struct TrieNode*)malloc(sizeof(struct TrieNode));
			memset(next, 0, sizeof(struct TrieNode));
		//	map<char, TrieNode*>* newmap = new map<char, TrieNode*>;
		//	next->childmap = newmap;
			(*(questions->childmap))[str[i]] = next;
		}
		(*(questions->childmap))[str[i]]->occur++;
		questions = (*(questions->childmap))[str[i]];
	}
}

/* returns occurances */
int searchTrie(string str, TrieNode* questions){
	for(int i=0;i<str.length();i++){
		if(!questions){
			return 0;
		}

		questions = (*(questions->childmap))[str[i]];
	}

	if(!questions) return 0;
	return questions->occur;
}

int searchChildren(string str, TreeNode* children){
	int countLevel = 0;
	while(children){
		countLevel+= searchTrie(str, children->questions);
		countLevel+= searchChildren(str, children->children);
		children = children->next;
	}
	return countLevel;
}

/*void destroyTree(TreeNode* root){
	while(root){
		destroyTree(root->children);

	}
}*/

int main(){
	stack<string> parse; // for parsing initial string
	map<string, TreeNode*> map; // for mapping name to treeNode

	TreeNode* root=NULL;
	ifstream in("test.txt");

	int n;
	//cin >> n;
	in >> n;
	string str;
	string upperLevel;

	while(n>0){
		string prevStr = str;
		in >> str;
		if(str == "("){
			upperLevel = prevStr;
			parse.push(upperLevel);
		}else if(str == ")"){
			//upperLevel = (string) parse.top();
			parse.pop();
			upperLevel = (string)parse.top();
		}else{
			n--;
			if(parse.empty()){ // root level
			//	in >> str;
				if(!root){
					root = (struct TreeNode*)(malloc(sizeof(struct TreeNode)));
					memset(root, 0, sizeof(struct TreeNode));
					strcpy(root->name,str.c_str());
					map[str] = root;
				}else{
					TreeNode* next = root;
					while(next->next){
						next = next->next;
					}
					next->next = (struct TreeNode*)(malloc(sizeof(struct TreeNode)));
					memset(next->next, 0, sizeof(struct TreeNode));
					strcpy(root->name,str.c_str());
					map[str] = next->next;
				}
			}else{
				struct TreeNode* thisNode = (struct TreeNode*)(malloc(sizeof(struct TreeNode)));
				memset(thisNode, 0, sizeof(struct TreeNode));
				strcpy(thisNode->name, str.c_str());
				struct TreeNode* parentNode = map[upperLevel];
				//insertChild(parentNode, thisNode);

				if(!parentNode->children){
					parentNode->children = thisNode;
					thisNode->parent = parentNode;
				}else{
					thisNode->parent = parentNode;
					parentNode = parentNode->children;
					while(parentNode->next){
						parentNode = parentNode->next;
					}
					parentNode->next = thisNode;
				}
				map[str] = thisNode;
			}
		}
	}

	while(!parse.empty()){ // flush the rest of stdin, the other ")"
		in >> str;
		parse.pop();
	}

	/* Input parsed into tree structure ^^^ */

	in >> n;
	while(n > 0){
		in >> str;
		str = str.substr(0,str.length()-1); // cut out the colon
		TreeNode* thisNode = map[str];

		getline(in, str);
		str = str.substr(1,str.length()); // \r character
	//	buildTrie(str, thisNode->questions);

		// go all the way up to parent the build the tree
	//	while(thisNode){
			if(!thisNode->questions){
				thisNode->questions = (struct TrieNode*)malloc(sizeof(TrieNode));
				memset(thisNode->questions, 0, sizeof(struct TrieNode));
			//	map<char, TrieNode*>* newmap = new map<char, TrieNode*>;
			//	thisNode->questions->childmap = newmap;
			}
			buildTrie(str, thisNode->questions);
	//		thisNode = thisNode->parent;
	//	}
		n--;
	//	cout << str << endl;
	}

	in >> n;
	int cnt[n];
	int index = 0;
	while(index< n){
		in >> str;
		TreeNode* thisNode = map[str];

		getline(in, str);
		int count = 0;
		count+= searchTrie(str.substr(1, str.length()), thisNode->questions); // cut out newline char
		count += searchChildren(str.substr(1, str.length()), thisNode->children);
		cnt[index] = count;
		index++;
	}

	for(int i=0;i<n;i++){
		cout << cnt[i] << endl;
	}

	//destroyTree(root);
	return 0;
}