#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 1. 定義 TokenType (完全依照範例)
typedef enum {
    TYPE_TOKEN,
    MAIN_TOKEN,
    IF_TOKEN,
    ELSE_TOKEN,
    WHILE_TOKEN,
    ID_TOKEN,
    LITERAL_TOKEN,
    ASSIGN_TOKEN,
    EQUAL_TOKEN,
    GREATEREQUAL_TOKEN,
    LESSEQUAL_TOKEN,
    GREATER_TOKEN,
    LESS_TOKEN,
    PLUS_TOKEN,
    MINUS_TOKEN,
    LEFTPAREN_TOKEN,
    RIGHTPAREN_TOKEN,
    LEFTBRACE_TOKEN,
    RIGHTBRACE_TOKEN,
    SEMICOLON_TOKEN
} TokenType;

// 2. 結構定義 (基於 hw0 的命名習慣)
struct my_node {
    char value[256];
    TokenType type;
    struct my_node *next;
};
typedef struct my_node node;

// 3. 關鍵字類型判斷 (完全依照範例)
TokenType keywordtype(const char *word) {
    if (strcmp(word, "int") == 0) return TYPE_TOKEN;
    if (strcmp(word, "main") == 0) return MAIN_TOKEN;
    if (strcmp(word, "if") == 0) return IF_TOKEN;
    if (strcmp(word, "else") == 0) return ELSE_TOKEN;
    if (strcmp(word, "while") == 0) return WHILE_TOKEN;
    return ID_TOKEN;
}

// 4. TokenType 轉字串 (完全依照範例，包含 REFT 拼法)
const char *tokenname(TokenType t) {
    switch (t) {
        case TYPE_TOKEN:         return "TYPE_TOKEN";
        case MAIN_TOKEN:         return "MAIN_TOKEN";
        case IF_TOKEN:           return "IF_TOKEN";
        case ELSE_TOKEN:         return "ELSE_TOKEN";
        case WHILE_TOKEN:        return "WHILE_TOKEN";
        case ID_TOKEN:           return "ID_TOKEN";
        case LITERAL_TOKEN:      return "LITERAL_TOKEN";
        case ASSIGN_TOKEN:       return "ASSIGN_TOKEN";
        case EQUAL_TOKEN:        return "EQUAL_TOKEN";
        case GREATEREQUAL_TOKEN: return "GREATEREQUAL_TOKEN";
        case LESSEQUAL_TOKEN:    return "LESSEQUAL_TOKEN";
        case GREATER_TOKEN:      return "GREATER_TOKEN";
        case LESS_TOKEN:         return "LESS_TOKEN";
        case PLUS_TOKEN:         return "PLUS_TOKEN";
        case MINUS_TOKEN:        return "MINUS_TOKEN";
        case LEFTPAREN_TOKEN:    return "LEFTPAREN_TOKEN";
        case RIGHTPAREN_TOKEN:   return "REFTPAREN_TOKEN"; 
        case LEFTBRACE_TOKEN:    return "LEFTBRACE_TOKEN";
        case RIGHTBRACE_TOKEN:   return "REFTBRACE_TOKEN"; 
        case SEMICOLON_TOKEN:    return "SEMICOLON_TOKEN";
        default:                 return "UNKNOWN_TOKEN";
    }
}

int main() {
    FILE *file = fopen("input.c", "r");
    if (file == NULL) {
        perror("File opening failed");
        return 1;
    }

    node *head = NULL;
    node *tail = NULL; // 保留 hw0 的尾端指標優化
    int ch;

    while ((ch = fgetc(file)) != EOF) {
        // 忽略 whitespace
        if (ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r') continue;

        char buf[256] = {0};
        TokenType current_type;
        int found = 0;

        // ID/Keyword 辨識
        if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_') {
            int len = 0;
            buf[len++] = (char)ch;
            while ((ch = fgetc(file)) != EOF && 
                   ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || ch == '_')) {
                if (len < 255) buf[len++] = (char)ch;
            }
            if (ch != EOF) ungetc(ch, file);
            buf[len] = '\0';
            current_type = keywordtype(buf);
            found = 1;
        }
        // 整數 Literal 辨識
        else if (ch >= '0' && ch <= '9') {
            int len = 0;
            buf[len++] = (char)ch;
            while ((ch = fgetc(file)) != EOF && (ch >= '0' && ch <= '9')) {
                if (len < 255) buf[len++] = (char)ch;
            }
            if (ch != EOF) ungetc(ch, file);
            buf[len] = '\0';
            current_type = LITERAL_TOKEN;
            found = 1;
        }
        // 符號辨識 (完全依照範例 switch 邏輯)
        else {
            buf[0] = (char)ch;
            buf[1] = '\0';
            switch (ch) {
                case '=': {
                    int next = fgetc(file);
                    if (next == '=') { strcpy(buf, "=="); current_type = EQUAL_TOKEN; }
                    else { if (next != EOF) ungetc(next, file); current_type = ASSIGN_TOKEN; }
                    found = 1; break;
                }
                case '>': {
                    int next = fgetc(file);
                    if (next == '=') { strcpy(buf, ">="); current_type = GREATEREQUAL_TOKEN; }
                    else { if (next != EOF) ungetc(next, file); current_type = GREATER_TOKEN; }
                    found = 1; break;
                }
                case '<': {
                    int next = fgetc(file);
                    if (next == '=') { strcpy(buf, "<="); current_type = LESSEQUAL_TOKEN; }
                    else { if (next != EOF) ungetc(next, file); current_type = LESS_TOKEN; }
                    found = 1; break;
                }
                case '+': current_type = PLUS_TOKEN; found = 1; break;
                case '-': current_type = MINUS_TOKEN; found = 1; break;
                case '(': current_type = LEFTPAREN_TOKEN; found = 1; break;
                case ')': current_type = RIGHTPAREN_TOKEN; found = 1; break;
                case '{': current_type = LEFTBRACE_TOKEN; found = 1; break;
                case '}': current_type = RIGHTBRACE_TOKEN; found = 1; break;
                case ';': current_type = SEMICOLON_TOKEN; found = 1; break;
            }
        }

        // 插入節點 (使用 hw0 的 tail 優化方式，不使用 findninsert)
        if (found) {
            node *newNode = (node *)malloc(sizeof(node));
            strncpy(newNode->value, buf, 255);
            newNode->value[255] = '\0';
            newNode->type = current_type;
            newNode->next = NULL;

            if (head == NULL) {
                head = newNode;
                tail = newNode;
            } else {
                tail->next = newNode;
                tail = newNode;
            }
        }
    }
    fclose(file);

    // 印出所有 Token (依照格式印出)
    node *ptr = head;
    while (ptr != NULL) {
        printf("%s: %s\n", ptr->value, tokenname(ptr->type));
        ptr = ptr->next;
    }

    // 釋放記憶體 (hw0 習慣)
    node *current = head;
    while (current != NULL) {
        node *temp = current;
        current = current->next;
        free(temp);
    }

    return 0;
}
