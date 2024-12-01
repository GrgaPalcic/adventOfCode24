#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct s_node{
    int data;
    struct s_node *next;
} t_node;

//function to sort the list
t_node* sort(t_node* head){
    t_node *current = head;
    t_node *index = NULL;
    int temp;
    
    if(head == NULL){
        return head;
    }else{
        while(current != NULL){
            index = current->next;
            
            while(index != NULL){
                if(current->data > index->data){
                    temp = current->data;
                    current->data = index->data;
                    index->data = temp;
                }
                index = index->next;
            }
            current = current->next;
        }
    }
    return head;

}

long int distance (t_node *h_left, t_node *h_right){

    t_node* t_left = h_left;
    t_node* t_right = h_right;
    long int diff = 0;
    
    while(t_left != NULL && t_right != NULL){
        diff += abs(t_left->data - t_right->data);
        //printf("Diff: %d\n", diff);
        t_left = t_left->next;
        t_right = t_right->next;
    }
    return diff;
}

long int similarity(t_node *h_left, t_node *h_right){

    t_node* t_left = h_left;
    t_node* t_right = h_right;
    long int score = 0;
    
    while(t_left != NULL){
        t_right = h_right;
        int count = 0;
        while(t_right != NULL){
            if(t_left->data == t_right->data){
                count++;
            }
            t_right = t_right->next;
        }
        score += t_left->data * count;
        t_left = t_left->next;
    }
    return score;
}

void main(){
    
    char* buf = malloc(100);
    t_node *t_left_list;
    t_node *h_left_list;
    t_node *t_right_list;
    t_node *h_right_list;

    FILE *f = fopen("data.in", "rw");

    if(f == NULL){
        printf("Error opening file\n");
        return;
    }

    int a, b;

    if(fscanf(f, "%d %d", &a, &b) == 2){
        //printf("Read from file: %d %d\n", a, b);
        t_left_list = malloc(sizeof(t_node));
        t_left_list->data = a;
        t_left_list->next = NULL;
        h_left_list = t_left_list;

        t_right_list = malloc(sizeof(t_node));
        t_right_list->data = b;
        t_right_list->next = NULL;
        h_right_list = t_right_list;
    }

    while(fscanf(f, "%d %d", &a, &b) == 2){
        //printf("Read from file: %d %d\n", a, b);
        t_left_list->next = malloc(sizeof(t_node));
        t_left_list = t_left_list->next;
        t_left_list->data = a;
        t_left_list->next = NULL;
        
        t_right_list->next = malloc(sizeof(t_node));
        t_right_list = t_right_list->next;
        t_right_list->data = b;
        t_right_list->next = NULL;
    }

    h_left_list = sort(h_left_list);
    h_right_list = sort(h_right_list);
    printf("Difference: %d\n", distance(h_left_list, h_right_list));
    printf("Similarity: %d\n", similarity(h_left_list, h_right_list));

    
    return;
}