#include <stdio.h>
#include <string.h>

typedef enum {
    TYPE_TOKEN, MAIN_TOKEN, IF_TOKEN, ELSE_TOKEN, WHILE_TOKEN,
    ID_TOKEN, LITERAL_TOKEN, ASSIGN_TOKEN, EQUAL_TOKEN,
    GREATEREQUAL_TOKEN, LESSEQUAL_TOKEN, GREATER_TOKEN, LESS_TOKEN,
    PLUS_TOKEN, MINUS_TOKEN, LEFTPAREN_TOKEN, RIGHTPAREN_TOKEN,
    LEFTBRACE_TOKEN, RIGHTBRACE_TOKEN, SEMICOLON_TOKEN, 
    EOF_TOKEN, UNKNOWN_TOKEN
} TokenType;

TokenType token;
char lexeme[256];
int error_flag = 0; 

TokenType keywordtype(const char *word) {
    if(strcmp(word, "int")   == 0) return TYPE_TOKEN;
    if(strcmp(word, "main")  == 0) return MAIN_TOKEN;
    if(strcmp(word, "if")    == 0) return IF_TOKEN;
    if(strcmp(word, "else")  == 0) return ELSE_TOKEN;
    if(strcmp(word, "while") == 0) return WHILE_TOKEN;
    return ID_TOKEN;
}

void get_next_token() {
    int ch;
    while ((ch = getchar()) != EOF) {
        
        if (ch == ' ' || ch == '\n' || ch == '\t') continue;
            
        if((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_'){
            int len = 0;
            lexeme[len++] = (char)ch;
            while ((ch = getchar()) != EOF &&
                   ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') ||
                    (ch >= '0' && ch <= '9') || ch == '_')){
                if(len < 255) lexeme[len++] = (char)ch;
            }
            lexeme[len] = '\0';
            if(ch != EOF) ungetc(ch, stdin);
            token = keywordtype(lexeme);
            return;
        }

        if (ch >= '0' && ch <= '9') {
            int len = 0;
            lexeme[len++] = (char)ch;
            while ((ch = getchar()) != EOF && ch >= '0' && ch <= '9'){
                if(len < 255) lexeme[len++] = (char)ch;
            }
            lexeme[len] = '\0';
            if(ch != EOF) ungetc(ch, stdin);
            token = LITERAL_TOKEN;
            return;
        }

        switch (ch) {
            case '=': {
                int next = getchar();
                if(next == '=') { strcpy(lexeme, "=="); token = EQUAL_TOKEN; }
                else { if(next != EOF) ungetc(next, stdin); strcpy(lexeme, "="); token = ASSIGN_TOKEN; }
                return;
            }
            case '>': {
                int next = getchar();
                if(next == '=') { strcpy(lexeme, ">="); token = GREATEREQUAL_TOKEN; }
                else { if (next != EOF) ungetc(next, stdin); strcpy(lexeme, ">"); token = GREATER_TOKEN; }
                return;
            }
            case '<': {
                int next = getchar();
                if (next == '=') { strcpy(lexeme, "<="); token = LESSEQUAL_TOKEN; }
                else { if (next != EOF) ungetc(next, stdin); strcpy(lexeme, "<"); token = LESS_TOKEN; }
                return;
            }
            case '+': strcpy(lexeme, "+"); token = PLUS_TOKEN; return;
            case '-': strcpy(lexeme, "-"); token = MINUS_TOKEN; return;
            case '(': strcpy(lexeme, "("); token = LEFTPAREN_TOKEN; return;
            case ')': strcpy(lexeme, ")"); token = RIGHTPAREN_TOKEN; return;
            case '{': strcpy(lexeme, "{"); token = LEFTBRACE_TOKEN; return;
            case '}': strcpy(lexeme, "}"); token = RIGHTBRACE_TOKEN; return;
            case ';': strcpy(lexeme, ";"); token = SEMICOLON_TOKEN; return;
            default:  lexeme[0] = (char)ch; lexeme[1] = '\0'; token = UNKNOWN_TOKEN; return;
        }
    }
    token = EOF_TOKEN;
    lexeme[0] = '\0';
}

void parse_error() {
    if (!error_flag) {
        printf("ParseError\n");
        error_flag = 1;
    }
}

// 宣告帶有 depth 參數的 Parser 函數
void parse_S(int depth);
void parse_S_prime(int depth);
void parse_E(int depth);

void print_indent(int depth) {
    for (int i = 0; i < depth; i++) {
        printf("  "); 
    }
}

// S -> E S'
void parse_S(int depth) {
    if (error_flag) return;
    print_indent(depth);
    printf("S -> E S'\n");
    
    if (token == LITERAL_TOKEN || token == LEFTPAREN_TOKEN) {
        // 子節點，深度 + 1
        parse_E(depth + 1);
        parse_S_prime(depth + 1);
    } else {
        parse_error();
    }
}

// S' -> + S | epsilon
void parse_S_prime(int depth) {
    if (error_flag) return;
    if (token == PLUS_TOKEN) {
        print_indent(depth);
        printf("S' -> + S\n");
        get_next_token(); 
        
        // 子節點，深度 + 1
        parse_S(depth + 1);
    } else if (token == RIGHTPAREN_TOKEN || token == EOF_TOKEN) {
        return; // epsilon 不印東西
    } else {
        parse_error();
    }
}

// E -> num | ( S )
void parse_E(int depth) {
    if (error_flag) return;
    if (token == LITERAL_TOKEN) {
        print_indent(depth);
        printf("%s\n", lexeme); // 印出數字本身
        get_next_token();
    } else if (token == LEFTPAREN_TOKEN) {
        print_indent(depth);
        printf("E -> ( S )\n");
        get_next_token();
        
        // 子節點，深度 + 1
        parse_S(depth + 1);
        
        if (token != RIGHTPAREN_TOKEN) {
            parse_error();
            return;
        }
        get_next_token();
    } else {
        parse_error();
    }
}

int main() {
    get_next_token();
    
    if (token != EOF_TOKEN) {
        // 從深度 0 開始解析
        parse_S(0);
        
        if (!error_flag && token != EOF_TOKEN) {
            parse_error();
        }
    }
    
    return 0;
}
