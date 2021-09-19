#include <stdio.h>
#include <stdlib.h>

#define OK 1
#define ERROR 0

#define DISPLAY 1

typedef struct node{
    struct node *parent;
    struct node *leftChild;
    struct node *rightChild;
    int key;
    int BF;
}Node;

typedef struct tree{
    Node *root;
}Tree;

Node *initNode(int k, Node *p)
{
    Node *n = (Node *) malloc (sizeof(Node));
    if (n != NULL){
        n->key = k;
        if (p != NULL) n->parent = p;
        else n->parent = n;
        n->leftChild = NULL;
        n->rightChild = NULL;
        n->BF = 0;
    }
    return n;
}//initnode()

Tree *initTree(int k)
{
    Tree *t = (Tree *) malloc (sizeof(Tree));
    if (t != NULL){
        t->root = initNode(k, NULL);
    }
    return t;
}//initTree()

Node *find (int k, Node *root)
{
	// termination conditions - either true, search is ended
	if ((root == NULL) || (root->key == k))	return root;
	if (k > root->key)
		return find(k,root->rightChild);
	else
		return find(k,root->leftChild);	
}//find()

int maxint(int a, int b)
{
    return (a >= b) ? a : b;
}//maxint()

int maxTreeHeight(Node *root)
{
    int nodeHeight = 0;
    if (root != NULL){
        nodeHeight = 1 + maxint(maxTreeHeight(root->leftChild), maxTreeHeight(root->rightChild));
    }
    return nodeHeight;
}//maxTreeHeight

int balanceFactor(Node *root)
{
    return maxTreeHeight(root->leftChild) - maxTreeHeight(root->rightChild);
}//balanceFactor()

int updateBF(Node *root)
{
    if (root != NULL){
        root->BF = balanceFactor(root);
        updateBF(root->leftChild);
        updateBF(root->rightChild);
    }
    return OK;
}//updateBF()

int rotateLeft(Node *n, Tree *t){
    if (DISPLAY) printf("Rotate left about %d\n", n->key);

    Node *Rchild = n->rightChild;
    Node *p = n->parent;
    //node is root of tree
    if(n == p){
        //set the Rchild to be the parent
        t->root = Rchild;
        Rchild->parent = Rchild;
        //set the nodes right child to the Rchild leftchild
        n->rightChild = Rchild->leftChild;
        if(Rchild->leftChild){
            Rchild->leftChild->parent = n;
        }
        //move node to left of Rchild 
        Rchild->leftChild = n;
        n->parent = Rchild;
        return OK;
    }
    //node is not the root of tree
    else{
        //node is right child of parent
        if(p->key < n->key){
            p->rightChild = Rchild;
            Rchild->parent = p;
        }
        //node is left child of parent
        else{
            p->leftChild = Rchild;
            Rchild->parent = p;
        }
        //node rightChild is swiched to the Rchild left child
        n->rightChild = Rchild->leftChild;
        //if the Rchild has a left child update its parent to be n
        if (Rchild->leftChild){
            Rchild->leftChild->parent = n;
        }
        //Rchild is now where n was in the tree, connect n to Rchild as its left child
        Rchild->leftChild = n;
        n->parent = Rchild;
        return OK;
    }
}//rotateLeft()

int rotateRight(Node *n, Tree *t){
    if (DISPLAY) printf("Rotate right about %d\n", n->key);

    Node *Lchild = n->leftChild;
    Node *p = n->parent;
    //node is root of tree
    if(n == p){
        //set Lchild to be the root of the tree and update it's parent
        t->root = Lchild;
        Lchild->parent = Lchild;
        //set the nodes left child to the Lchild rightchild
        n->leftChild = Lchild->rightChild;
        if(Lchild->rightChild){
            Lchild->rightChild->parent = n;
        }
        //move node to right of Lchild 
        Lchild->rightChild = n;
        n->parent = Lchild;
        return OK;
    }
    //node is not root of tree
    else{
        //node is right child of parent
        if(p->key < n->key){
            p->rightChild = Lchild;
            Lchild->parent = p;
        }
        //node is left child of parent
        else{
            p->leftChild = Lchild;
            Lchild->parent = p;
        }
        //set n leftChild to be is left child's right child
        n->leftChild = Lchild->rightChild;
        //update Lchilds rightchild parent if it exists
        if(Lchild->rightChild){
            Lchild->rightChild->parent = n;
        }
        //Lchild is now where n was in the tree, change Lchilds rightchild to be n
        Lchild->rightChild = n;
        n->parent = Lchild;
        return OK;
    }
}//rotateRight()

Node *findAncestor(Node *root, Node *inserted)
{
    Node *temp = root;
    Node *anc = NULL;
    while(temp != inserted){
        if (temp->BF > 1 || temp->BF < -1){
            anc = temp;
        }
        if(inserted->key > temp->key)
            temp = temp->rightChild;
        else
            temp = temp->leftChild;

        if (temp == NULL) break;
    }
    return anc;
}

int makeAVL(Node *root, Node *inserted, Tree *t)
{
    Node *anc = findAncestor(root, inserted);
    //traverse through tree looking for balance factors that are not 0
    //find closest one to the node inserted
    if (DISPLAY){
        if (anc == NULL){
            printf("None out of balalance connected to %d\n", inserted->key);
            //return error meaning that AVL did not have to change anything
            return ERROR;
        }
        printf("ancestor is: %d\n", anc->key);
    }
    else{
        if (anc == NULL) return ERROR;
    }
    
    //left heavy (L)
    if(anc->BF > 1){
        //inserted to right, do nothing case(2)
        if(inserted->key > anc->key) return OK;
        //inserted to left
        else{
            //inserted to right subtree of left rubtree case(5)
            if(inserted->key > anc->leftChild->key){
                rotateLeft(anc->leftChild, t);
                rotateRight(anc, t);
            }
            //inserted left case(3)
            else{
                rotateRight(anc, t);
            }
        }
        
    }
    //right heavy (R)
    else if(anc->BF < -1){
        //inserted to left, do nothing case(2)
        if(inserted->key < anc->key) return OK;
        //inserted to right
        else{
            //inserted to left subtree of right rubtree case(6)
            if(inserted->key < anc->rightChild->key){
                rotateRight(anc->rightChild, t);
                rotateLeft(anc, t);
            }
            //inserted left case(4)
            else{
                rotateLeft(anc, t);
            }
        }
    }
    return OK;
}

int printTree(Node *root)
{
    //DISPLAYS KEY AND BALANCE FACTORS

    // start of this tree
	printf("{");
	// values in the root node (assuming value is pointing to an integer)
	printf("(%ld, %d)", root->key, root->BF);
	
	// Now show left subtree or {} if there is no left subtree
	if (root->leftChild != NULL) printTree(root->leftChild);
	else printf("{}");
	// Marker between left and right subtrees		
	printf(",");
	// Now show right subtree or {} if there is no right subtree
	if (root->rightChild != NULL) printTree(root->rightChild);
	else printf("{}");
	// Close display of this tree with closing curly bracket	
	printf("}");
}//printtree()

int insert(int k, Node *root, Tree *t)
{
    int result = ERROR;
    printf("INSERTING: %d\n", k);
    if(root == NULL){
        Node *n = initNode(k, NULL);
        root = n;
        n->BF = 0;
        return OK;
    }
    if(root->key == k){
        return ERROR;
    }
    //inserted to left or parent
    else if(k < root->key){
        if (root->leftChild != NULL)
        {
            result = insert(k, root->leftChild, t);
        }
        else{
            Node *n = initNode(k, root);
            root->leftChild = n;
            //make sure the tree is AVL once the node is inserted
            if (DISPLAY){
                updateBF(t->root);
                printf("-----tree before-----\n");
                printTree(t->root);
                printf("\n");
                if(makeAVL(t->root, n, t)){
                    updateBF(t->root);
                    printf("\n-----tree after-----\n");
                    printTree(t->root);
                    printf("\n");
                }
                printf("\n");
            }
            else{
                updateBF(t->root);
                makeAVL(t->root, n, t);
                updateBF(t->root);
            }
            result = OK;
        }
    }
    //inserted to right of parent
    else{
        if(root->rightChild != NULL){
            result = insert(k, root->rightChild, t);
        }
        else{
            Node *n = initNode(k, root);
            root->rightChild = n;
            //make sure the tree is AVL once the node is inserted
            if (DISPLAY){
                updateBF(t->root);
                printf("-----tree before-----\n");
                printTree(t->root);
                printf("\n");
                if(makeAVL(t->root, n, t)){
                    updateBF(t->root);
                    printf("\n-----tree after-----\n");
                    printTree(t->root);
                    printf("\n");
                }
                printf("\n");
            }
            else{
                updateBF(t->root);
                makeAVL(t->root, n, t);
                updateBF(t->root);
            }
            result = OK;
        }
    }
    return result;
}//insert()

void main(void)
{
    Tree *t = initTree(50);
    Node *treeRoot = t->root;
    insert(20, t->root, t);
    insert(90, t->root, t); 
    insert(10, t->root, t); 
    insert(40, t->root, t); 
    insert(70, t->root, t); 
    insert(100, t->root, t); 
    insert(5, t->root, t); 
    insert(15, t->root, t); 
    insert(30, t->root, t); 
    insert(45, t->root, t);
    insert(42, t->root, t);  
    printf("-----final tree-----\n");
    printTree(t->root);
}