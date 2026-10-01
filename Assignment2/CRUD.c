#include<stdio.h>
#include<string.h>

//Structure to hold student information
struct student
{
    int id;
    char name[20];
    int age;
};

//Function declarations
void createUser();
void readUsers();
void updateUser();
void deleteUser();  
int idexists(int id);// Function to check if a user with the given ID already exists

// Main function
int main(){
    int choice;
    int run=1;
    while(run){ 
        printf("1. Create User\n");
        printf("2. Read Users\n"); 
        printf("3. Update User\n");
        printf("4. Delete User\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch(choice){
            case 1:
                createUser();
                break;
            case 2:
                readUsers();
                break;
            case 3:
                updateUser();
                break;
            case 4:
                deleteUser();
                break;
            case 5:
                printf("Exiting...\n");
                run=0;
                break;
            default:
                printf("Invalid choice!\n");
            }
        printf("----------------\n");
    }
    return 0;
}

// Function to check if a user with the given ID already exists
int idexists(int id) {
    struct student s1;
    FILE *file = fopen("users.txt", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return 0;
    }
    while (fscanf(file, "%d %s %d", &s1.id, s1.name, &s1.age) == 3) {
        if (s1.id == id) {
            fclose(file);
            return 1; 
        }
    }
    fclose(file);
    return 0;
}

// Function to create a new user and save it to the file
void createUser(){
    struct student s1;
    FILE *file;
    file=fopen("users.txt","a");
    if (file==NULL)
    {
        printf("Error opening file!\n");
        return;
    }
    printf("Enter ID: ");
    scanf("%d", &s1.id);
    if (idexists(s1.id)) {
        printf("ID already exists. User not created.\n");
        fclose(file);
        return;
    }
    printf("Enter Name: ");
    scanf("%s", s1.name);
    printf("Enter Age: ");
    scanf("%d", &s1.age);
    fprintf(file, "%d %s %d\n", s1.id, s1.name, s1.age);
    fclose(file);
    printf("User created successfully.\n");
}

// Function to read and display all users from the file
void readUsers(){
    struct student s1;
    FILE *file;
    file=fopen("users.txt","r");
    if (file==NULL)
    {
        printf("Error opening file!\n");
        return;
    }   
    while (fscanf(file,"%d %s %d",&s1.id,s1.name,&s1.age)==3){
        printf("ID: %d\n",s1.id);
        printf("Name: %s\n",s1.name);
        printf("Age: %d\n",s1.age);
    }
    fclose(file);
}

// Function to update an existing user's information in the file
void updateUser(){
    FILE *file;
    file=fopen("users.txt","r");
    if (file==NULL)
    {
        printf("Error opening file!\n");
        return;
    }
    FILE *temp_file;
    temp_file=fopen("temp.txt","w");
    if (temp_file==NULL)
    {
        printf("Error opening temporary file!\n");
        fclose(file);
        return;
    }
    int target_id;
    int new_age;
    char new_name[20];

    printf("Enter ID to update: ");
    scanf("%d", &target_id);

    printf("Enter new name: ");
    scanf("%s", new_name);

    printf("Enter new age: ");
    scanf("%d", &new_age);
    struct student s1;
    int found = 0;
    while (fscanf(file,"%d %s %d",&s1.id,s1.name,&s1.age)==3){
        if (s1.id==target_id){
            strcpy(s1.name, new_name);
            s1.age=new_age;
            found = 1;
        }
        fprintf(temp_file, "%d %s %d\n", s1.id, s1.name, s1.age);
    }
    if (!found) {
        printf("User not found.\n");
    }
    fclose(file);
    fclose(temp_file);
    remove("users.txt");
    rename("temp.txt", "users.txt");
}

// Function to delete a user from the file based on the given ID
void deleteUser(){
    FILE *file;
    file=fopen("users.txt","r");
    if (file==NULL)
    {
        printf("Error opening file!\n");
        return;
    }
    FILE *temp_file;
    temp_file=fopen("temp.txt","w");
    if (temp_file==NULL)
    {
        printf("Error opening temporary file!\n");
        fclose(file);
        return;
    }
    int target_id;

    printf("Enter ID to delete: ");
    scanf("%d", &target_id);
    struct student s1;
    int found = 0;
    while (fscanf(file,"%d %s %d",&s1.id,s1.name,&s1.age)==3){
        if (s1.id==target_id){
            found = 1;
        } else {
            fprintf(temp_file, "%d %s %d\n", s1.id, s1.name, s1.age);
        }
    }
    if (!found) {
        printf("User not found.\n");
    }  
    fclose(file);
    fclose(temp_file);
    remove("users.txt");
    rename("temp.txt", "users.txt");
}