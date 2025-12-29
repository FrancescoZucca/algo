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

void QL_recurse_in_order(datetime_t s, datetime_t f, struct node* node, double* min, double* max){

    int l = date_compare(s, node->val.date, 1);
    int r = date_compare(f, node->val.date, 1);

    // se non si è ancora arrivati all'inizio dell'intervallo non è necessario iterare a sx.
    if (l <= 0)
        if (node->left) QL_recurse_in_order(s, f, node->left, min, max);

    if (l <= 0 && r >= 0){
        if (node->val.price < *min) *min = node->val.price;
        if (node->val.price > *max) *max = node->val.price;
    }

    // se si è già superata la fine dell'intervallo, non è necessario continuare.
    if (r >= 0)
        if (node->right) QL_recurse_in_order(s, f, node->right, min, max);
}

void QL_interval_price(datetime_t date_start, datetime_t date_finish, quote_list_t list, double* min, double* max){
    QL_recurse_in_order(date_start, date_finish, list->root, min, max);
}

void QL_recurse_dfs(struct node* node, double* min, double* max){
    if (!node) return;

    if (node->val.price < *min) *min = node->val.price;
    if (node->val.price > *max) *max = node->val.price;

    QL_recurse_dfs(node->left, min, max);
    QL_recurse_dfs(node->right, min, max);
}

void QL_minmax_price(quote_list_t list, double* min, double* max){
    QL_recurse_dfs(list->root, min, max);
}

void QL_recurse_distance(struct node* node, int cur, int* max, int* min){
    if (node->left)
        QL_recurse_distance(node->left, cur+1, max, min);

    if (node->right)
        QL_recurse_distance(node->right, cur+1, max, min);

    if (!node->left && !node->right){
        if (*max < cur) *max = cur;
        if (*min > cur) *min = cur;
    }
}

void QL_minmax_distance(quote_list_t list, int* max, int* min){
    QL_recurse_distance(list->root, 0, max, min);
}

struct node* QL_rotL(struct node* node){
    struct node* tmp = node->right;
    node->right = tmp->left;
    tmp->left = node;
    return tmp;
}

struct node* QL_rotR(struct node* node){
    struct node* tmp = node->left;
    node->left = tmp->right;
    tmp->right = node;
    return tmp;
}

int QL_calculate_N(struct node* node){
    if (!node) return 0;
    return 1 + QL_calculate_N(node->left) + QL_calculate_N(node->right);
}

struct node* QL_partR(struct node* node, int r){
    int t = QL_calculate_N(node->left);
    if (t > r) {
        node->left = QL_partR(node->left, r);
        node = QL_rotR(node);
    }
    if (t < r) {
        node->right = QL_partR(node->right, r-t-1);
        node = QL_rotL(node);
    }

    return node;
}

struct node* QL_balance_R(struct node* node){
    if (!node) return node;
    int r = (QL_calculate_N(node)+1)/2 - 1;
    node = QL_partR(node, r);
    node->left = QL_balance_R(node->left);
    node->right = QL_balance_R(node->right);
    return node;
}

void QL_balance(quote_list_t list, double S){
    int min, max;
    QL_minmax_distance(list, &max, &min);
    if ((double)max/min > S)
        list->root = QL_balance_R(list->root);
}