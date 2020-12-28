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
void postOrder(Node* node, stack<int> &s);
int booleanResult(int one, int two, int op);
map<char, Node*> mapping;
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
    return true;
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
	//cout<<prefix_string<<endl;
    int i=0;
    root=makeNode(prefix_string, i);
    table=mapping;
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
        temp->right->parent=temp;
        temp->left->parent=temp;

    }
    else if(temp->token=='~')
    {
        temp->left=makeNode(expr, ++num);
        temp->left->parent=temp;
    }
    else 
    {
        mapping[temp->token]=temp;
    }
    
    return temp;
}

void BTree::setVar(char var, bool val)
{
    table[var]->result=val;
}

bool BTree::evaluate(Node* p)
{
    stack<int> results;
    postOrder(p, results);
    p->result=results.top();

    return root->result;
}


void postOrder(Node* node, stack<int> &s)
{
    if(node==NULL)
        return;
    postOrder(node->left, s);
    postOrder(node->right, s);
    if(node->token=='&' ||node->token=='|')
    {
        int op, result;
        int first=s.top();
        s.pop();
        int second=s.top();
        s.pop();
        if(node->token=='&')
            op=AND;
        else
            op=OR;
        result=booleanResult(first, second, op);
        s.push(result);
    }
    else if(node->token=='~')
    {
        int temp=s.top();
        s.pop();
        if(temp==1)
            temp=0;
        else
            temp=1;

        s.push(temp);
    }
    else
    {
        s.push(node->result);
    }
    
}

int booleanResult(int one, int two, int op)
{
    if(one==1 && two==1 && op==AND)
        return 1;
    if((one==1 || two==1) && op==OR)
        return 1;

    return 0;
}
int main()
{
	BTree etree = BTree("~a&((~b)|(~c)&d)");
etree.setVar('a', true);
etree.setVar('b', false);
etree.setVar('c', false);
etree.setVar('d', true);
etree.evaluate(etree.root);
etree.show();
cout << "The evaluated result = " << etree.root->result << endl;
}