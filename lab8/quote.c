#include "quote.h"
#include <stddef.h>

struct node {
    struct node *left, *right;
    quote_t val;
};

struct quote_list{
    struct node *root;
};

struct node* new_node(quote_t val){
    struct node* ret = malloc(sizeof(struct node));
    ret->left = ret->right = NULL;
    ret->val = val;
    return ret;
}

void QL_insert(quote_list_t list, quote_t quote){
    if(!list->root) {
        list->root = new_node(quote);
        return;
    }

    struct node* cur = list->root;
    int flag = 1;
    while(flag){
        int cmp = compare_dates(cur->val.date, quote.date);
        if (cmp > 0){
            if (cur -> right)
                cur = cur -> right;
            else {
                cur->right = new_node(quote);
                flag = 0;
            }
        } else {
            if (cur -> left)
                cur = cur->left;
            else {
                cur->left = new_node(quote);
                flag = 0;
            }
        }
    }
}

quote_t* QL_search(quote_list_t list, datetime_t date){
    struct node* cur = list->root;
    while(cur){
        //TODO: add this stuff
    }
    return NULL;
}