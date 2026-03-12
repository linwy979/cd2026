#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 作業要求的 Linked List 結構
struct my_node {
    char data;
    int count;
    struct my_node *next;
};

typedef struct my_node Node;

int main() {
    // 讀取自己這個檔案
    FILE *fptr = fopen(__FILE__, "r");
    if (fptr == NULL) {
        printf("Error: Cannot open file\n");
        return -1;
    }

    Node *head = NULL;
    Node *tail = NULL;
    
    // 加分題優化：建立一個 256 大小的指標陣列來記錄看過的字元
    // 這樣找字元的時間複雜度就是 O(1)
    Node *fast_look[256] = {NULL};

    int ch_val;
    while ((ch_val = fgetc(fptr)) != EOF) {
        unsigned char idx = (unsigned char)ch_val;

        // 如果這個字元已經在我的 List 裡了
        if (fast_look[idx] != NULL) {
            fast_look[idx]->count++;
        } 
        else {
            // 沒看過的字元，新開一個空間給它
            Node *newNode = (Node *)malloc(sizeof(Node));
            newNode->data = (char)ch_val;
            newNode->count = 1;
            newNode->next = NULL;

            // 把新節點接在 List 的最後面
            if (head == NULL) {
                head = newNode;
                tail = newNode;
            } else {
                tail->next = newNode;
                tail = newNode;
            }
            // 存入加速表，下次遇到同字元就不必重新跑迴圈找
            fast_look[idx] = newNode;
        }
    }
    fclose(fptr);

    // 開始印出結果 (依照出現順序)
    Node *ptr = head;
    while (ptr != NULL) {
        // 處理空白和換行的特殊印出方式
        if (ptr->data == '\n') printf("\\n : %d\n", ptr->count);
        else if (ptr->data == ' ') printf("' ' : %d\n", ptr->count);
        else if (ptr->data == '\t') printf("\\t : %d\n", ptr->count);
        else printf("%c : %d\n", ptr->data, ptr->count);
        
        ptr = ptr->next;
    }

    // 養成好習慣：程式結束前釋放記憶體
    Node *curr = head;
    while (curr != NULL) {
        Node *temp_ptr = curr;
        curr = curr->next;
        free(temp_ptr);
    }

    return 0;
}
