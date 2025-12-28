#include "quote.h"
#include <stddef.h>

struct node {
    struct node *left, *right;
    quote_t val;
};

struct quote_list{
    struct node *root;
};

void quote_parse(FILE* fp, quote_t* quote){
    parse_date(fp, &quote->date, 0);
    fscanf(fp, " %lf %d", &quote->price, &quote->weight);
}

struct node* new_node(quote_t val){
    struct node* ret = malloc(sizeof(struct node));
    ret->left = ret->right = NULL;
    ret->val = val;
    return ret;
}

quote_list_t QL_init(){
    quote_list_t ret = malloc(sizeof(struct quote_list));
    ret->root = NULL;
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
        int cmp = date_compare(cur->val.date, quote.date, 1);
        if (cmp > 0){
            if (cur -> right)
                cur = cur -> right;
            else {
                cur->right = new_node(quote);
                flag = 0;
            }
        } else if (cmp < 0) {
            if (cur -> left)
                cur = cur->left;
            else {
                cur->left = new_node(quote);
                flag = 0;
            }
        } else {
            int weight = cur->val.weight;
            cur->val.price = (cur->val.price * weight + quote.price * quote.weight)/(weight + quote.weight);
            cur->val.weight = weight + quote.weight;
            flag = 0;
        }
    }
}

void node_free(struct node* node){
    if (!node) return;

    node_free(node->left);
    node_free(node->right);
    free(node);
}

void QL_free(quote_list_t list){
    node_free(list->root);
    free(list);
}

quote_t* QL_search(datetime_t date, quote_list_t list){
    struct node* cur = list->root;
    while(cur){
        int cmp = date_compare(date, cur->val.date, 1);
        if (cmp < 0) cur = cur->left;
        else if (cmp > 0) cur = cur->right;
        else return &cur->val;
    }
    return NULL;
}