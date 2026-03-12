#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct my_node {
    char data;
    int count;
    struct my_node *next;
};

typedef struct my_node Node;

int main() {

    FILE *fptr = fopen(__FILE__, "r");
    if (fptr == NULL) {
        printf("Error: Cannot open file\n");
        return -1;
    }

    Node *head = NULL;
    Node *tail = NULL;
    
    Node *fast_look[256] = {NULL};

    int ch_val;
    while ((ch_val = fgetc(fptr)) != EOF) {
        unsigned char idx = (unsigned char)ch_val;

       
        if (fast_look[idx] != NULL) {
            fast_look[idx]->count++;
        } 
        else {
            
            Node *newNode = (Node *)malloc(sizeof(Node));
            newNode->data = (char)ch_val;
            newNode->count = 1;
            newNode->next = NULL;

           
            if (head == NULL) {
                head = newNode;
                tail = newNode;
            } else {
                tail->next = newNode;
                tail = newNode;
            }
           
            fast_look[idx] = newNode;
        }
    }
    fclose(fptr);
   
    Node *ptr = head;
    while (ptr != NULL) {
        if (ptr->data == '\n') printf("\\n : %d\n", ptr->count);
        else if (ptr->data == ' ') printf("' ' : %d\n", ptr->count);
        else if (ptr->data == '\t') printf("\\t : %d\n", ptr->count);
        else printf("%c : %d\n", ptr->data, ptr->count);
        
        ptr = ptr->next;
    }

   
    Node *curr = head;
    while (curr != NULL) {
        Node *temp_ptr = curr;
        curr = curr->next;
        free(temp_ptr);
    }

    return 0;
}
