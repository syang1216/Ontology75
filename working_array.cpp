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
	TrieNode* child[ALPHABET]; 
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
		bool flag = 0; // for finding match
	//	while(true){
		if(str[i]==' '){
		//	pair<int, TrieNode*> pr = questions->child[52];
			if(!(questions->child[52])){
				struct TrieNode* next = (struct TrieNode*)malloc(sizeof(struct TrieNode));
				memset(next, 0, sizeof(struct TrieNode));

				questions->child[52] = next;
				next->occur++;
				questions = next;
			}else{
				questions->child[52]->occur++;
				questions = questions->child[52];
			}
		}else if(str[i] == '?'){
			if(!(questions->child[53])){
				struct TrieNode* next = (struct TrieNode*)malloc(sizeof(struct TrieNode));
				memset(next, 0, sizeof(struct TrieNode));

				questions->child[53] = next;
				next->occur++;
				questions = next;
			}else{
				questions->child[53]->occur++;
				questions = questions->child[53];
			}
		}else if(isupper(str[i])){
			if(!(questions->child[str[i]-39])){
				struct TrieNode* next = (struct TrieNode*)malloc(sizeof(struct TrieNode));
				memset(next, 0, sizeof(struct TrieNode));

				questions->child[str[i]-39] = next;
				next->occur++;
				questions = next;
			}else{
				questions->child[str[i]-39]->occur++;
				questions = questions->child[str[i]-39];
			}
		}else{
			if(!(questions->child[str[i]-'a'])){
				struct TrieNode* next = (struct TrieNode*)malloc(sizeof(struct TrieNode));
				memset(next, 0, sizeof(struct TrieNode));

				questions->child[str[i]-'a'] = next;
				next->occur++;
				questions = next;
			}else{
				questions->child[str[i]-'a']->occur++;
				questions = questions->child[str[i]-'a'];
			}
		}

	}
}

/* returns occurances */
int searchTrie(string str, TrieNode* questions){
	int occur = 0;
	for(int i=0;i<str.length();i++){
		char c = str[i];
	//	bool found = false;
		if(!questions){
			return 0;
		}

		TrieNode* pr;
		if(c==' '){
			pr = questions->child[52];
		}else if(c=='?'){
			pr = questions->child[53];
		}else if(isupper(c)){
			pr = questions->child[c-39];
		}else{
			pr = questions->child[c-'a'];
		}

		questions = pr;
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
//	ifstream in("test.txt");

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
		cin >> str;
		parse.pop();
	}

	/* Input parsed into tree structure ^^^ */

	cin >> n;
	while(n > 0){
		cin >> str;
		str = str.substr(0,str.length()-1); // cut out the colon
		TreeNode* thisNode = map[str];

		getline(cin, str);
		str = str.substr(1,str.length()); // \r character
	//	buildTrie(str, thisNode->questions);

		// go all the way up to parent the build the tree
		while(thisNode){
			if(!thisNode->questions){
				thisNode->questions = (struct TrieNode*)malloc(sizeof(TrieNode));
				memset(thisNode->questions, 0, sizeof(struct TrieNode));
			}
			buildTrie(str, thisNode->questions);
			thisNode = thisNode->parent;
		}
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
		count+= searchTrie(str.substr(1, str.length()-2), thisNode->questions); // cut out newline char
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