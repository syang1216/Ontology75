#include <map>
#include <fstream>
#include <iostream>
#include <stack>
#include <string>
#include <cstdlib>
#include <cstring>

//#define ALPHABET 255

using namespace std;



struct TrieNode{
	// TrieNode* child[ALPHABET]; initially was going to use this but this will need a lot of memory allocation
	pair<char, TrieNode*> pr;
	TrieNode* next;
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
		bool flag = 0; // for finding match
		while(true){
			pair<char, TrieNode*> pr = questions->pr;
			if(!questions->next){ // not initialized node
				struct TrieNode* next = (struct TrieNode*)malloc(sizeof(struct TrieNode));
				memset(next, 0, sizeof(struct TrieNode));

				pair<char, TrieNode*> cT = make_pair(str[i], next);
				questions->pr = cT;
				questions->occur++;

				questions->next = (struct TrieNode*)malloc(sizeof(struct TrieNode));
				memset(questions->next, 0, sizeof(struct TrieNode));
				questions = cT.second;
				break;
			}else if(str[i] == pr.first){
				//thisTrie = pr.second;
				questions->occur++;
				questions = pr.second;
				break;
			//	flag = true;
			}
			questions = questions->next;
		}
	}
}

/* returns occurances */
int searchTrie(string str, TrieNode* questions){
	int occur;
	for(int i=0;i<str.length();i++){
		char s = str[i];
		bool found = false;
		while(questions){
			pair<char, TrieNode*> cT = questions->pr;
			if(cT.first==s){
				occur = questions->occur;
				questions = cT.second;
				found = true;
				break;
			}
			questions = questions->next;
		}

		if(!found){
			return 0;
		}
	}

	return occur;
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
	//ifstream in("test3.txt");

	int n;
	//cin >> n;
	cin >> n;
	string str;
	string upperLevel;

	while(n>0){
		string prevStr = str;
		cin >> str;
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
					map[str] = root;
				}else{
					TreeNode* next = root;
					while(next->next){
						next = next->next;
					}
					next->next = (struct TreeNode*)(malloc(sizeof(struct TreeNode)));
					memset(next->next, 0, sizeof(struct TreeNode));
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
		cin >> str;
		parse.pop();
	}

	/* Input parsed into tree structure ^^^ */

	cin >> n;
	while(n > 0){
		cin >> str;
		str = str.substr(0,str.length()-1); // cut out the colon
		TreeNode* thisNode = map[str];
	/*	if(!thisNode->questions){
			thisNode->questions = (struct TrieNode*)malloc(sizeof(TrieNode));
			memset(thisNode->questions, 0, sizeof(struct TrieNode));
		}*/

		getline(cin, str);
		while(thisNode){
			if(!thisNode->questions){
				thisNode->questions = (struct TrieNode*)malloc(sizeof(TrieNode));
				memset(thisNode->questions, 0, sizeof(struct TrieNode));
			}
			buildTrie(str, thisNode->questions);
			thisNode = thisNode->parent;
		}
		//buildTrie(str.substr(1, str.length()), thisNode->questions);
		n--;
	//	cout << str << endl;
	}

	cin >> n;
	int cnt[n];
	int index = 0;
	while(index< n){
		cin >> str;
		TreeNode* thisNode = map[str];

		getline(cin, str);
		int count = 0;
		count+= searchTrie(str, thisNode->questions);
		//count += searchChildren(str.substr(1, str.length()), thisNode->children);
		cnt[index] = count;
		index++;
	}

	for(int i=0;i<n;i++){
		cout << cnt[i] << endl;
	}

	//destroyTree(root);
	return 0;
}