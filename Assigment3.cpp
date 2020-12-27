#include <string>
#include <stack>
#include <queue>
#include <map>
#include <iostream>
#include <algorithm>

using namespace std;
enum TOKEN {RIGHT_PAREN, OR, AND, NOT, LEFT_PAREN};



class Node {
public:
	Node(char token) {
		this->token = token;
		left = right = parent = NULL;
		result = -1;
	}
	char token; // the one-char symbol representing either an operator or an operand
	Node* left; // pointer to the left child
	Node* right; // pointer to the right child
	Node* parent; // pointer to the parent node
	bool result; // the evaluated Boolean result for this node/subtree
};


Node* makeNode(string expr, int &num);
class BTree
{
public:
	Node* root; // pointer to the root node
	BTree(string expr); // constructing a tree from a specified expression (in the string format)
	bool evaluate(Node* p); // evaluate the node/subtree rooted by the specified node
	void setVar(char var, bool val); // instantiate a variable with the specified Boolean value
	void show(); // output the constructed tree
	map<char, Node*> table; // you can use the map class provided by the C++ STL to maintain a table for looking up the the instantiations of all variables.
};
bool higherPrecedence(char first, char second)//function used to determine operator precedence
{
    if(first=='~')
    {
        return true;
    }
    else if(first=='&')
    {
        if(second=='~')
            return false;
        else 
            return true;
    }
    else if(first=='|')
    {
        return false;
    }
}

void preOrder(Node* node, int level)
{
	if(node==NULL)
		return;
    int tab=0;
    while(tab!=level-1)
    {
        cout<<"\t";
        tab++;
    }
	cout<<node->token<<endl;
	preOrder(node->left, level+1);
	preOrder(node->right, level+1);
}
string toPrefix(string infix)
{
    stack<char> s;
    string prefixExpression="";
    reverse(infix.begin(), infix.end());
    for(int i=0;i<infix.length();i++)
    {
        if(infix[i]=='~' || infix[i]=='&' || infix[i]=='|')
        {
            if(s.empty())
            {
                s.push(infix[i]);
            }
            else
            {
                while(1)
                {
                    char temp=s.top();
                    if(temp==')')
                    {
                        s.push(infix[i]);
                        break;
                    }
                    bool result=higherPrecedence(infix[i], temp);
                    if(result==true)
                    {
                        s.push(infix[i]);
                        break;
                    }
                    else
                    {
                        prefixExpression+=s.top();
                        s.pop();
                        if(s.empty())
                        {
                            s.push(infix[i]);
                            break;
                        }
                    }
                }
            }
        }
        else if(infix[i]==')')
        {
            s.push(infix[i]);
        }
        else if(infix[i]=='(')
        {
            while(1)
            {
                if(s.top()==')')
                {
                    s.pop();
                    break;
                }
                prefixExpression+=s.top();
                s.pop();
            }
        }
        else
        {
            prefixExpression+=infix[i];
        }
    }
    while(s.empty()==false)
    {
        prefixExpression+=s.top();
        s.pop();
    }
    reverse(prefixExpression.begin(), prefixExpression.end());

    return prefixExpression;
}

BTree::BTree(string expr)
{
    root=NULL;
	string prefix_string=toPrefix(expr);
	cout<<prefix_string<<endl;
    int i=0;
    root=makeNode(prefix_string, i);
}

void BTree::show()
{
    int level=1;
	preOrder(root, level);
}

Node* makeNode(string expr, int &num)
{
    Node* temp=new Node(expr[num]);
    if(temp->token=='&' || temp->token=='|')
    {
        temp->left=makeNode(expr, ++num);
        temp->right=makeNode(expr, ++num);//make sure you set the parents 
    }
    else if(temp->token=='~')
    {
        temp->left=makeNode(expr, ++num);
    }
    return temp;
}

int main()
{
	BTree etree = BTree("~a&((~b)|(~c)&d)");
    etree.show();
}