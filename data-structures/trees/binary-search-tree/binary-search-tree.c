#include "binary-search-tree.h"

Node *newNode(int data) {
    Node *newNode = (Node *)malloc(sizeof(Node));

    newNode -> data = data;
    newNode -> left = NULL;
    newNode -> right = NULL;

    return newNode;
}

Node *insert(Node *root, int data) {
    if(!root) {
        root = newNode(data);
    } else if (data > root->data) {
        root -> right = insert(root -> right, data);
    } else if(data < root-> data) {
        root -> left = insert(root -> left, data);
    }

    return root;
}

Node *getMax(Node *root) {
    if(root -> left) {
        return getMax(root->right);
    }
    return root;
}

Node *remove(Node *root, int data) {
    if(!root) {
        return root;
    } else if(data > root -> data) {
        root -> right = remove(root -> right, data);
    } else if(data == root -> data) {
        if(!root -> left && !root -> right) {
            free(root);
            return NULL;
        } else if(!root -> left) {
            Node *node = root;
            root = root -> right;
            free(node);
            return node;
        } else if(!root -> right) {
            Node *node = root;
            root = root -> left;
            free(node);
            return root;
        } else {
            Node *node = getMax(root -> left);
            
            root -> data = node->data;
            root -> left = remove(root->left, node->data);
        }
    }

    return root;
}

int height(Node *root) {
    if(!root) {
        return 0;
    } else {
        int right = height(root->right);
        int left = height(root->left);

        if(right > left) {
            return right + 1;
        } else {
            return left + 1;
        }
    }
}