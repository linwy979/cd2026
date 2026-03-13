#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct word{
    char letter;
    int num;
    struct word* next;
};
void printReverse(struct word *node){
    if(node == NULL)
        return;

    printReverse(node->next);
    printf("%c,", node->letter);
}
int main(){
    FILE *fp;
    char ch;
    fp = fopen(__FILE__,"r");
    if (fp == NULL) {
        printf("file open error\n");
        return 1;
    }
    
    struct word *head = NULL;
    
    while((ch = fgetc(fp)) != EOF) {
        if(ch == ' ' || ch == '\n' || ch == '\t') 
            continue;
        struct word *curr = head;
        
        while(curr!=NULL){
            if (curr->letter==ch){//exist
                curr->num++;
                break;
            }
            curr = curr->next;
        }
        if(curr == NULL){//not exist
            struct word *newnode = (struct word*)malloc(sizeof(struct word));
            newnode->letter = ch;
            newnode->num = 1;
            newnode->next = head;
            head = newnode;
        }
    }
    struct word *curr = head;
    printReverse(head);
    return 0;
}
