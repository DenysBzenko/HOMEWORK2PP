#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <stack>

#define MAX_TEXT_SIZE 1024

class TextEditor {
private:
    char text[MAX_TEXT_SIZE] = "";
    char clipboard[MAX_TEXT_SIZE] = "";
    std::stack<std::string> undoStack;
    std::stack<std::string> redoStack;

    void pushToUndo() {
        undoStack.push(text);
        while (!redoStack.empty()) {
            redoStack.pop();
        }
    }

public:
    void append_text() {
        char input[100];
        printf("Enter text to append: ");
        fgets(input, sizeof(input), stdin);

        size_t len = strlen(text);
        strncat_s(text, sizeof(text), input, sizeof(text) - len - 1);

        if (len + strlen(input) < sizeof(text) && text[len + strlen(input) - 1] == '\n') {
            text[len + strlen(input) - 1] = '\0';
        }
    }

    void start_new_line() {
        strncat_s(text, sizeof(text), "\n", sizeof(text) - strlen(text) - 1);
        printf("New line is started\n");
    }

    void save_to_file() {
        char filename[100];
        printf("Enter the file name for saving: ");
        fgets(filename, sizeof(filename), stdin);
        if (filename[strlen(filename) - 1] == '\n') {
            filename[strlen(filename) - 1] = '\0';
        }

        FILE* file;
        errno_t err = fopen_s(&file, filename, "w");
        if (err != 0) {
            perror("Error opening file");
            return;
        }

        fputs(text, file);
        fclose(file);
        printf("Text has been saved successfully\n");
    }

    void load_from_file() {
        char filename[100];
        printf("Enter the file name for loading: ");
        fgets(filename, sizeof(filename), stdin);
        if (filename[strlen(filename) - 1] == '\n') {
            filename[strlen(filename) - 1] = '\0';
        }

        FILE* file;
        errno_t err = fopen_s(&file, filename, "r");
        if (err != 0) {
            perror("Error opening file");
            return;
        }

        char ch;
        int index = 0;
        while ((ch = fgetc(file)) != EOF && index < MAX_TEXT_SIZE - 1) {
            text[index++] = ch;
        }
        text[index] = '\0';

        fclose(file);
        printf("Text has been loaded successfully\n");
    }

    void print_text() {
        printf("%s\n", text);
    }

    void insert_text_by_index() {
        int line, index;
        char input[100];

        printf("Choose line and index (starting from 0): ");
        scanf_s("%d %d", &line, &index);
        while (getchar() != '\n');

        printf("Enter text to insert: ");
        fgets(input, sizeof(input), stdin);

        char copy[MAX_TEXT_SIZE];
        strcpy_s(copy, sizeof(copy), text);

        char* lines[MAX_TEXT_SIZE];
        char* context = NULL;
        char* token = strtok_s(copy, "\n", &context);
        int i = 0;
        while (token) {
            lines[i++] = token;
            token = strtok_s(NULL, "\n", &context);
        }
    }

    void delete_text_by_index() {
        int line, index, num_symbols;
        printf("Choose line, index and number of symbols to delete: ");
        scanf_s("%d %d %d", &line, &index, &num_symbols);
        while (getchar() != '\n');

        char copy[MAX_TEXT_SIZE];
        strcpy_s(copy, sizeof(copy), text);

        char* lines[MAX_TEXT_SIZE];
        char* context = NULL;
        char* token = strtok_s(copy, "\n", &context);
        int i = 0;
        while (token) {
            lines[i++] = token;
            token = strtok_s(NULL, "\n", &context);
        }
    }



    void cut(int start, int length) {
        pushToUndo();

        if (start + length > strlen(text)) {
            length = strlen(text) - start;
        }

        strncpy_s(clipboard, sizeof(clipboard), text + start, length);
        clipboard[length] = '\0';

        strcpy_s(text + start, sizeof(text) - start, text + start + length);
    }

    void copy(int start, int length) {
        if (start + length > strlen(text)) {
            length = strlen(text) - start;
        }

        strncpy_s(clipboard, sizeof(clipboard), text + start, length);
        clipboard[length] = '\0';
    }

    void paste(int position) {
        pushToUndo();

        char temp[MAX_TEXT_SIZE];
        strcpy_s(temp, sizeof(temp), text + position);
        strcpy_s(text + position, sizeof(text) - position, clipboard);
        strcat_s(text, sizeof(text), temp);
    }

    void undo() {
        if (!undoStack.empty()) {
            redoStack.push(text);
            strcpy_s(text, sizeof(text), undoStack.top().c_str());
            undoStack.pop();
        }
        else {
            printf("Nothing to undo!\n");
        }
    }

    void redo() {
        if (!redoStack.empty()) {
            undoStack.push(text);
            strcpy_s(text, sizeof(text), redoStack.top().c_str());
            redoStack.pop();
        }
        else {
            printf("Nothing to redo!\n");
        }
    }

    void insertWithReplacement(int position, const char* newText) {
        pushToUndo();

        if (position > strlen(text)) {
            position = strlen(text);
        }

        strcpy_s(text + position, sizeof(text) - position, newText);
    }

    void menu() {
        while (1) {
            printf("Choose the command:\n");
            printf("1. Append text symbols to the end\n");
            printf("2. Start the new line\n");
            printf("3. Use files to loading/saving the information\n");
            printf("4. Print the current text to console\n");
            printf("5. Insert the text by line and symbol index\n");
            printf("6. Exit\n");
            printf("7. Clear terminal.\n");
            printf("8. Delete text\n");
            printf("9. Cut\n");
            printf("10. Copy\n");
            printf("11. Paste\n");
            printf("12. Undo\n");
            printf("13. Redo\n");
            printf("14. Insert with replacement\n");

            int choice;
            scanf_s("%d", &choice);
            while (getchar() != '\n');

            switch (choice) {
            case 1:
                append_text();
                break;
            case 2:
                start_new_line();
                break;
            case 3:
                printf("Choose:\n");
                printf("1. Save to file\n");
                printf("2. Load from file\n");
                int file_choice;
                scanf_s("%d", &file_choice);
                while (getchar() != '\n');
                if (file_choice == 1) {
                    save_to_file();
                }
                else if (file_choice == 2) {
                    load_from_file();
                }
                else {
                    printf("Invalid choice!\n");
                }
                break;
            case 4:
                print_text();
                break;
            case 5:
                insert_text_by_index();
                break;
            case 6:
                exit(0);
            case 7:
                system("cls");
                break;
            case 8:
                delete_text_by_index();
                break;
            case 9:
                int start, length;
                printf("Enter start position and length to cut: ");
                scanf_s("%d %d", &start, &length);
                cut(start, length);
                break;
            case 10:
                printf("Enter start position and length to copy: ");
                scanf_s("%d %d", &start, &length);
                copy(start, length);
                break;
            case 11:
                printf("Enter position to paste: ");
                scanf_s("%d", &start);
                paste(start);
                break;
            case 12:
                undo();
                break;
            case 13:
                redo();
                break;
            case 14:
                printf("Enter position to insert: ");
                scanf_s("%d", &start);
                char replacement[MAX_TEXT_SIZE];
                printf("Enter text to insert: ");
                scanf_s("%s", replacement);
                insertWithReplacement(start, replacement);
                break;
            default:
                printf("Invalid choice!\n");
            }
        }
    }
};

    int main() {
        TextEditor editor;
        editor.menu();
        return 0;
    }
   
