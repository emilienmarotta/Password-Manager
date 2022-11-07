#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>

#define NAME_LIST_SIZE 512
#define PASS_LIST_SIZE 512
#define PASS_SIZE 256
#define NAME_SIZE 256
#define UNLCK_PASS_SIZE 256

int nb_combinations = 0; // Number of combinations of name and password

typedef enum Action{ // Action to be performed
    _ADD, // Add new combination
    _DISPLAY, // Display password
} Action;

void fgets_without_newline_char(char *str, int str_size); // Removes trailing newline character of the fgets() function
void place_cursor_at_the_beginning(FILE *file);
char* trim(char *str); // Clean the white space around a string
void set_unlock_password(FILE *unlock_password_file, char *password, bool *is_set_unlock_password); // Requests to set the unlock password only once (only if is_set_unlock_password == false)
bool check_if_unlock_password_file_empty(FILE *unlock_password_file); // If it's empty, set_unlock_password() will be called
void input_unlock_password(char *password); // Requests to input the unlock password -> unlock password has been already set
bool compare_unlock_password(FILE *unlock_password_file, char *unlock_password, char *entered_unlock_password); 
Action select_action(); // Determines what action to be performed  
void add_new_combi_name_password(char *name, char *password, FILE *file); // Adds a new combination of name and password -> e.g. : github password1234
void action_add_combination(char *name, char *password, FILE *file); // Adds a new combination of name and password
void action_display_password(FILE *file); // Displays assiociated password 

int main(void){
    FILE *backup_file = fopen("backup.txt", "a+"); // Contains the whole combinations
    FILE *unlock_password_file = fopen("unlock_password.txt", "a+"); // Contains the unlock password
    char unlock_password[UNLCK_PASS_SIZE];
    char entered_unlock_password[UNLCK_PASS_SIZE]; // Password entered when input_unlock_password() is called 
    bool is_set_unlock_password = false;
    bool is_access_allowed = false; // True if entered_unlock_password == unlock_password
    Action action;
    char name[NAME_SIZE];
    char password[PASS_SIZE];

    if(backup_file, unlock_password_file == NULL){ // Check if files exist
        exit(1);
    }

    printf("\nPASSWORD MANAGER\n");
    
    is_set_unlock_password = check_if_unlock_password_file_empty(unlock_password_file);
    set_unlock_password(unlock_password_file, unlock_password, &is_set_unlock_password); // Only if is_set_unlock_password == false
    
    while(is_access_allowed == false){
        input_unlock_password(entered_unlock_password);
        is_access_allowed = compare_unlock_password(unlock_password_file, unlock_password, entered_unlock_password); // Verifies that the entered password matches the unlock password
    }

    while(true){
        action = select_action();
        if(action == _ADD)
            action_add_combination(name, password, backup_file); // Remove name, password
        else
            action_display_password(backup_file);
    }

    fclose(backup_file);
    fclose(unlock_password_file);

    return 0;
}

void fgets_without_newline_char(char *str, int str_size){ 
    if(fgets(str, str_size, stdin) != NULL){
        size_t length = strlen(str);
        if(length > 0 && str[length-1] == '\n'){
            str[--length] = '\0';
        }
    }
}

void place_cursor_at_the_beginning(FILE *file){
    fseek(file, 0, SEEK_SET); // Places the cursor at the beginning of the file
}

char* trim(char *str){ // From https://www.unix.com/programming/21264-how-trim-white-space-around-string-c-program.html
    char pointer[strlen(str)+1];
    int i, j = 0;
    for(i = 0; str[i] != '\0'; i++){
        if(str[i] != ' ' && str[i] != '\t'){
            pointer[j++] = str[i];
        }
    }
    pointer[j] = '\0';
    str = pointer;
    return str;
} 

void set_unlock_password(FILE *unlock_password_file, char *password, bool *is_set_unlock_password){
    if(*is_set_unlock_password == false){
        printf("\nSet the unlock password : ");
        fgets_without_newline_char(password, UNLCK_PASS_SIZE);
        strcpy(password, trim(password));
        fputs(password, unlock_password_file);
    }
    *is_set_unlock_password = true;
}

bool check_if_unlock_password_file_empty(FILE *unlock_password_file){
    size_t file_size = 0;
    place_cursor_at_the_beginning(unlock_password_file);
    while(fgetc(unlock_password_file) != EOF){
        file_size++;
    }
    if(file_size != 0){
        return true; // Unlock password has been set
    }
    else{ 
        return false; // Unlock password hasn't been set
    }
}

void input_unlock_password(char *password){
    printf("\nInput the unlock password : ");
    fgets_without_newline_char(password, UNLCK_PASS_SIZE);
    strcpy(password, trim(password));
}

bool compare_unlock_password(FILE *unlock_password_file, char *unlock_password, char *entered_unlock_password){
    place_cursor_at_the_beginning(unlock_password_file);
    fgets(unlock_password, UNLCK_PASS_SIZE, unlock_password_file); // Retrieves the unlock password in unlck_password.txt
    if(strcmp(unlock_password, entered_unlock_password) == 0){
        return 1;
    }
    else{
        return 0;
    }
}

Action select_action(){
    Action action;
    char user_input = 0;
    bool exit_loop = false;
    while(exit_loop == false){ // if action != ('1' or '2' or '3' or '4') : asks again to select the action to be performed 
        printf("\n\n> Add : [Input 1]\n> Display : [Input 2]\n>> ");
        action = getchar();
        while (getchar() != '\n'); // Flushes the input buffer
        switch(action){
            case '1' : action = _ADD; exit_loop = true; break;
            case '2' : action = _DISPLAY; exit_loop = true; break;
            default : exit_loop = false;
        }
    }
    return action;
}

void add_new_combi_name_password(char *name, char *password, FILE *file){
    fprintf(file, "%s %s\n", name, password); 
    nb_combinations++;
}

void action_add_combination(char *name, char *password, FILE *file){
    char name_[NAME_SIZE];
    char password_[PASS_SIZE];
    printf("\nName : ");
    fgets_without_newline_char(name_, NAME_SIZE);
    strcpy(name_, trim(name_));
    printf("Password : ");
    fgets_without_newline_char(password_, PASS_SIZE);
    strcpy(password_, trim(password_));
    add_new_combi_name_password(name_, password_, file);
}

void action_display_password(FILE *file){
    char name[NAME_SIZE];
    char password[PASS_SIZE];
    char word_in_the_file[NAME_SIZE]; 
    bool name_exists = false; // True if the name is in the file
    printf("\nName : "); 
    fgets_without_newline_char(name, NAME_SIZE);
    strcpy(name, trim(name));
    place_cursor_at_the_beginning(file);
    do{
        fscanf(file, "%s", word_in_the_file);
        if(strcasecmp(word_in_the_file, name) == 0){
            fscanf(file, "%s", password);
            name_exists = true;
        }
    } while(fgetc(file) != EOF);
    if(name_exists)
        printf("Password : %s\n", password);
    else{
        printf("Not found...\n");
    }
}