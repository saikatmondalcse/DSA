#include<bits/stdc++.h>
using namespace std;
#define all(x) (x).begin(), (x).end()
#define pb push_back
#define ff first
#define ss second
#define sz(x) ((int)(x).size())
typedef long long ll;

#ifndef ONLINE_JUDGE
#define deb(x) cout<<#x<<" "<<x<<"\n"
#define deb2(x,y) cout<<"[ "<<#x<<"="<<x<<","<<#y<<"="<<y<<" ]\n"
#define deba(x,n) cout<<#x<<": [ ";for(int i=0;i<n;i++){cout<<x[i]<<" ";}cout<<"]\n"; 
#define deba2(x,n,m) cout<<#x<<":\n";for(int i=0;i<n;i++){for(int j=0;j<m;j++)cout<<x[i][j]<<" ";cout<<"\n";}; 
#define debe(x) cout<<#x<<":[ ";for(auto i:x){cout<<i<<" ";}cout<<"]\n"; 
#else
#define deb(x) 
#define deb2(x,y) 
#define deba(x,n) 
#define deba2(x,n,m) 
#define debe(x) 
#endif

/*
    case 1 : root is unbalanced and left of root and left of left of root has more height -> right rotate root(R)

    case 2 : root is unbalanced and left of root has more height and right of left root has more height -> left rotate on left of root(L) and then case 1(R)
    
    case 3 : root is unbalanced and right of root has more height and right of right of root has more height -> left rotate on root(L)

    case 4 : root is unbalanced and right of root has more height and left of right of root has more height -> right rotate on right of root(R) and case 3(L)
 

*/


struct AVLNode{
    int key;
    AVLNode *left,*right;
    int h;
    AVLNode(int x)
    {
        left=right=NULL;
        h=1;
        key=x;
    }
};

int height(AVLNode *root)
{
    if(root==NULL)
        return 0;
    else
        return root->h;
}

/*
           y            x
        x     c  ->  a     y    
      a   b             b     c

*/
AVLNode *right_rotation(AVLNode *root)
{
    AVLNode *new_root=root->left;
    root->left=new_root->right;
    new_root->right=root;
    root->h=1+max(height(root->left),height(root->right));
    new_root->h=1+max(height(new_root->left),height(new_root->right));
    return new_root;
}

/*
         y                  x
      a     x    ->     y       c      
          b   c       a    b

*/
AVLNode *left_rotation(AVLNode *root)
{
    AVLNode *new_root=root->right;
    root->right=new_root->left;
    new_root->left=root;
    root->h=1+max(height(root->left),height(root->right));
    new_root->h=1+max(height(new_root->left),height(new_root->right));

    return new_root;
}

void inorder(AVLNode *root)
{
    if(root==NULL)
        return ;
    inorder(root->left);
    cout<<root->key<<" ";
    inorder(root->right);
}


AVLNode *balance(AVLNode *root)
{
    int bal=height(root->left)-height(root->right);
    if(bal>1)
    {
        if(height(root->left->left)>=height(root->left->right))
        {
            return right_rotation(root);
        }
        else
        {
            root->left=left_rotation(root->left);
            return right_rotation(root);
        }
    }
    else if(bal<-1)
    {
        if(height(root->right->right)>=height(root->right->left))
        {
            return left_rotation(root);
        }
        else
        {
            root->right=right_rotation(root->right);
            return left_rotation(root);
        }
    }   
    else
        return root;
}


AVLNode *insert_key(AVLNode *root,int value) // only unique
{   
    if(root==NULL)
    {
        return new AVLNode(value);
    }
    if(value<root->key)
    {
        root->left=insert_key(root->left,value);
    }
    else if(value>root->key)
    {
        root->right=insert_key(root->right,value);
    }
    root->h=1+max(height(root->left),height(root->right));

    int bal=height(root->left)-height(root->right);

    return balance(root);
}


AVLNode *erase_key(AVLNode *root,int x)
{
    if(root==NULL)
        return NULL;
    if(root->key>x)
    {
        root->left=erase_key(root->left,x);
    }
    else if(root->key<x)
    {
        root->right=erase_key(root->right,x);
    }
    else
    {
        if(root->left==NULL)
        {
            AVLNode *new_root=root->right;
            delete(root);
            root=new_root;
        }
        else if(root->right==NULL)
        {
            AVLNode *new_root=root->left;
            delete(root);
            root=new_root;
        }
        else
        {
            AVLNode *succ=root->right;
            while(succ->left!=NULL)
                succ=succ->left;
            root->key=succ->key;
            root->right=erase_key(root->right,root->key);
        }
    }
    if(root==NULL)
        return root;
    root->h=1+max(height(root->left),height(root->right));

    return balance(root);
}

bool checkbalance(AVLNode *root)
{
    if(root==NULL)
        return true;
    int l=height(root->left),r=height(root->right);
    if(abs(l-r)<=1)
    {
        return checkbalance(root->left)&checkbalance(root->right);
    }
    else
        return false;
}

AVLNode *search_key(AVLNode *root,int x) // returns NULL if key is not present
{
    while(root!=NULL)
    {
        if(root->key==x)
            return root;
        else if(root->key>x)
        {
            root=root->left;
        }
        else
            root=root->right;
    }
    return root;
}

AVLNode *lowr_bound(AVLNode *root,int x)
{
    AVLNode *lb=NULL;
    while(root!=NULL)
    {
        if(root->key>=x)
        {
            lb=root;
            root=root->left;
        }
        else
            root=root->right;
    }
    return lb;
}



int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

    AVLNode *root=NULL;

    vector<int>v(2000); // upto 2e4 fine
    iota(v.begin(),v.end(),1);
    shuffle(all(v),rng);
    // debe(v);
    for(auto x:v)
    {
        root=insert_key(root,x);
        if(!checkbalance(root))
        {
            deb(x);
        }
        // deb(root->key);
    }
    shuffle(all(v),rng);

    for(auto x:v)
    {
        if(search_key(root,x)==NULL)
        {
            deb(x);
        }
    }

    // root=erase_key(root,40);
    // root=erase_key(root,41);

    // cout<<(lowr_bound(root,40)->key);

    shuffle(all(v),rng);
    for(auto x:v)
    {
        root=erase_key(root,x);
        if(!checkbalance(root))
        {
            deb(x);
        }
    }

    return 0;
}
