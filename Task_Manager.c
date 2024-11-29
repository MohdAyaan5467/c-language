#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TASKS 100
#define TASK_LENGTH 100
#define DATE_LENGTH 11
#define USERNAME_LENGTH 50
#define PASSWORD_LENGTH 50

typedef struct {
    char description[TASK_LENGTH];
    char dueDate[DATE_LENGTH];
    int priority;
    int isCompleted;
} Task;

typedef struct {
    char username[USERNAME_LENGTH];
    char password[PASSWORD_LENGTH];
} User;
//global variables
Task tasks[MAX_TASKS];
int taskCount = 0;

User currentUser;

void saveTasks() {
    FILE *file = fopen("tasks.dat", "wb");
    if (file == NULL) {
        printf("Error saving tasks!\n");
        return;
    }
    fwrite(&taskCount, sizeof(int), 1, file);
    fwrite(tasks, sizeof(Task), taskCount, file);
    fclose(file);
}

void loadTasks() {
    FILE *file = fopen("tasks.dat", "rb");
    if (file == NULL) {
        return;
    }
    fread(&taskCount, sizeof(int), 1, file);
    fread(tasks, sizeof(Task), taskCount, file);
    fclose(file);
}

void saveUser() {
    FILE *file = fopen("user.dat", "wb");
    if (file == NULL) {
        printf("Error saving user data!\n");
        return;
    }
    fwrite(&currentUser, sizeof(User), 1, file);
    fclose(file);
}

int loadUser() {
    FILE *file = fopen("user.dat", "rb");
    if (file == NULL) {
        return 0;  // No user data found, return 0
    }
    fread(&currentUser, sizeof(User), 1, file);
    fclose(file);
    return 1;
}

int login() {
    char username[USERNAME_LENGTH], password[PASSWORD_LENGTH];
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);
    
    if (strcmp(username, currentUser.username) == 0 && strcmp(password, currentUser.password) == 0) {
        printf("Login successful!\n");
        return 1;
    } else {
        printf("Invalid username or password.\n");
        return 0;
    }
}

void signup() {
    printf("Enter a username: ");
    scanf("%s", currentUser.username);
    printf("Enter a password: ");
    scanf("%s", currentUser.password);
    saveUser();
    printf("User created successfully!\n");
}

void addTask() {
    if (taskCount >= MAX_TASKS) {
        printf("Task list is full!\n");
        return;
    }
    printf("Enter task description: ");
    getchar(); // to consume the newline
    fgets(tasks[taskCount].description, TASK_LENGTH, stdin);
    tasks[taskCount].description[strcspn(tasks[taskCount].description, "\n")] = 0; // remove new line character

    printf("Enter due date (YYYY-MM-DD): ");
    fgets(tasks[taskCount].dueDate, DATE_LENGTH, stdin);
    tasks[taskCount].dueDate[strcspn(tasks[taskCount].dueDate, "\n")] = 0; 

    printf("Enter priority (1-5): ");
    scanf("%d", &tasks[taskCount].priority);

    tasks[taskCount].isCompleted = 0;
    taskCount++;
    saveTasks();
    printf("Task added successfully!\n");
}

void viewTasks() {
    if (taskCount == 0) {
        printf("No tasks available.\n");
        return;
    }
    printf("\n%-5s %-50s %-12s %-10s %-10s\n", "No.", "Description", "Due Date", "Priority", "Status");
    printf("------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < taskCount; i++) {
        printf("%-5d %-50s %-12s %-10d %-10s\n", i + 1, tasks[i].description, tasks[i].dueDate, tasks[i].priority, tasks[i].isCompleted ? "Completed" : "Pending");
    }
}

void deleteTask() {
    if (taskCount == 0) {
        printf("No tasks to delete.\n");
        return;
    }
    int taskNumber;
    printf("Enter task number to delete: ");
    scanf("%d", &taskNumber);
    if (taskNumber < 1 || taskNumber > taskCount) {
        printf("Invalid task number!\n");
        return;
    }
    for (int i = taskNumber - 1; i < taskCount - 1; i++) {
        tasks[i] = tasks[i + 1];
    }
    taskCount--;
    saveTasks();
    printf("Task deleted successfully!\n");
}

void markTaskCompleted() {
    if (taskCount == 0) {
        printf("No tasks to mark as completed.\n");
        return;
    }
    int taskNumber;
    printf("Enter task number to mark as completed: ");
    scanf("%d", &taskNumber);
    if (taskNumber < 1 || taskNumber > taskCount) {
        printf("Invalid task number!\n");
        return;
    }
    tasks[taskNumber - 1].isCompleted = 1;
    saveTasks();
    printf("Task marked as completed!\n");
}

void editTask() {
    if (taskCount == 0) {
        printf("No tasks to edit.\n");
        return;
    }
    int taskNumber;
    printf("Enter task number to edit: ");
    scanf("%d", &taskNumber);
    if (taskNumber < 1 || taskNumber > taskCount) {
        printf("Invalid task number!\n");
        return;
    }
    getchar(); // to consume the newline character
    printf("Enter new task description: ");
    fgets(tasks[taskNumber - 1].description, TASK_LENGTH, stdin);
    tasks[taskNumber - 1].description[strcspn(tasks[taskNumber - 1].description, "\n")] = 0; // remove newline character

    printf("Enter new due date (YYYY-MM-DD): ");
    fgets(tasks[taskNumber - 1].dueDate, DATE_LENGTH, stdin);
    tasks[taskNumber - 1].dueDate[strcspn(tasks[taskNumber - 1].dueDate, "\n")] = 0; // remove newline character

    printf("Enter new priority (1-5): ");
    scanf("%d", &tasks[taskNumber - 1].priority);

    saveTasks();
    printf("Task edited successfully!\n");
}

void searchTasks() {
    if (taskCount == 0) {
        printf("No tasks available.\n");
        return;
    }
    char searchTerm[TASK_LENGTH];
    printf("Enter search term: ");
    getchar(); // to consume the newline character
    fgets(searchTerm, TASK_LENGTH, stdin);
    searchTerm[strcspn(searchTerm, "\n")] = 0; // remove newline character

    printf("\n%-5s %-50s %-12s %-10s %-10s\n", "No.", "Description", "Due Date", "Priority", "Status");
    printf("-----------------------------------------------------------------------------------\n");
    for (int i = 0; i < taskCount; i++) {
        if (strstr(tasks[i].description, searchTerm) != NULL) {
            printf("%-5d %-50s %-12s %-10d %-10s\n", i + 1, tasks[i].description, tasks[i].dueDate, tasks[i].priority, tasks[i].isCompleted ? "Completed" : "Pending");
        }
    }
}

int compareByDueDate(const void *a, const void *b) {
    Task *taskA = (Task *)a;
    Task *taskB = (Task *)b;
    return strcmp(taskA->dueDate, taskB->dueDate);
}

int compareByPriority(const void *a, const void *b) {
    Task *taskA = (Task *)a;
    Task *taskB = (Task *)b;
    return taskA->priority - taskB->priority;
}

void sortTasks() {
    if (taskCount == 0) {
        printf("No tasks available.\n");
        return;
    }
    int choice;
    printf("Sort by:\n");
    printf("1. Due Date\n");
    printf("2. Priority\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    if (choice == 1) {
        qsort(tasks, taskCount, sizeof(Task), compareByDueDate);
        printf("Tasks sorted by due date.\n");
    } else if (choice == 2) {
        qsort(tasks, taskCount, sizeof(Task), compareByPriority);
        printf("Tasks sorted by priority.\n");
    } else {
        printf("Invalid choice!\n");
    }
    saveTasks();
}

void displayHelp() {
    printf("\nHelp Menu:\n");
    printf("1. Add Task: Allows you to add a new task with a description, due date, and priority.\n");
    printf("2. View Tasks: Displays all tasks with their details.\n");
    printf("3. Delete Task: Deletes a task based on its number.\n");
    printf("4. Mark Task as Completed: Marks a task as completed based on its number.\n");
    printf("5. Edit Task: Allows you to edit the details of an existing task.\n");
    printf("6. Search Tasks: Allows you to search for tasks by description.\n");
    printf("7. Sort Tasks: Allows you to sort tasks by due date or priority.\n");
    printf("8. Help: Displays this help menu.\n");
    printf("9. Exit: Exits the application.\n");
}
void viewPendingTasks() {
    int found = 0;
    printf("\nPending Tasks:\n");
    printf("%-5s %-50s %-12s %-10s\n", "No.", "Description", "Due Date", "Priority");
    printf("-------------------------------------------------------------------------------\n");
    for (int i = 0; i < taskCount; i++) {
        if (!tasks[i].isCompleted) {
            found = 1;
            printf("%-5d %-50s %-12s %-10d\n", i + 1, tasks[i].description, tasks[i].dueDate, tasks[i].priority);
        }
    }
    if (!found) {
        printf("No pending tasks found.\n");
    }
}
void clearAllTasks() {
    char confirmation;
    printf("Are you sure you want to delete all tasks? (y/n): ");
    getchar(); // Consume newline
    scanf("%c", &confirmation);
    if (confirmation == 'y' || confirmation == 'Y') {
        taskCount = 0;
        saveTasks();
        printf("All tasks cleared successfully!\n");
    } else {
        printf("Operation canceled.\n");
    }
}
void taskStatistics() {
    int completed = 0, pending = 0;
    for (int i = 0; i < taskCount; i++) {
        if (tasks[i].isCompleted) {
            completed++;
        } else {
            pending++;
        }
    }
    printf("\nTask Statistics:\n");
    printf("Total Tasks: %d\n", taskCount);
    printf("Completed Tasks: %d\n", completed);
    printf("Pending Tasks: %d\n", pending);
}
void viewCompletedTasks() {
    int found = 0;
    printf("\nCompleted Tasks:\n");
    printf("%-5s %-50s %-12s %-10s\n", "No.", "Description", "Due Date", "Priority");
    printf("---------------------------------------------------------------------------\n");
    for (int i = 0; i < taskCount; i++) {
        if (tasks[i].isCompleted) {
            found = 1;
            printf("%-5d %-50s %-12s %-10d\n", i + 1, tasks[i].description, tasks[i].dueDate, tasks[i].priority);
        }
    }
    if (!found) {
        printf("No completed tasks found.\n");
    }
}

void postponeTask() {
    if (taskCount == 0) {
        printf("No tasks to postpone.\n");
        return;
    }
    int taskNumber, days;
    printf("Enter task number to postpone: ");
    scanf("%d", &taskNumber);
    if (taskNumber < 1 || taskNumber > taskCount) {
        printf("Invalid task number!\n");
        return;
    }
    printf("Enter the number of days to postpone: ");
    scanf("%d", &days);

    // input date manually
    int year, month, day;
    sscanf(tasks[taskNumber - 1].dueDate, "%d-%d-%d", &year, &month, &day);

    struct tm tm = {0};
    tm.tm_year = year - 1900;
    tm.tm_mon = month - 1;
    tm.tm_mday = day + days; // Add No. of days
    mktime(&tm); // Normalize date

    // Update the dueDate
    strftime(tasks[taskNumber - 1].dueDate, DATE_LENGTH, "%Y-%m-%d", &tm);

    saveTasks();
    printf("Task postponed successfully!\n");
}


int main() {
    int choice;
    int isLoggedIn = 0;
printf("Welcome to OrganizeIt\n");
    // Load the user if exists
    if (!loadUser()) {
        printf("No user data found. Please sign up first.\n");
        signup();
    }

    // Simple login 
    while (!isLoggedIn) {
        printf("1. Login\n2. Signup\nEnter your choice: ");
        scanf("%d", &choice);
        if (choice == 1) {
            isLoggedIn = login();
        } else if (choice == 2) {
            signup();
            isLoggedIn = 1;//loop terminates
        } else {
            printf("Invalid choice. Try again.\n");
        }
    }

    // Load tasks after login
    loadTasks();

    // Main task loop
    while (1) {
        printf("\nDaily Task Manager\n");
        printf("1. Add Task\n");
        printf("2. View Tasks\n");
        printf("3. Delete Task\n");
        printf("4. Mark Task as Completed\n");
        printf("5. Edit Task\n");
        printf("6. Search Tasks\n");
        printf("7. Sort Tasks\n");
        printf("8. Help\n");
        printf("9. View Completed Tasks\n");
        printf("10. View Pending Tasks\n");
        printf("11. Clear All Tasks\n");
        printf("12. Task Statistics\n");
        printf("13. Postpone Task\n");
        printf("14. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                addTask();
                break;
            case 2:
                viewTasks();
                break;
            case 3:
                deleteTask();
                break;
            case 4:
                markTaskCompleted();
                break;
            case 5:
                editTask();
                break;
            case 6:
                searchTasks();
                break;
            case 7:
                sortTasks();
                break;
            case 8:
                displayHelp();
                break;
            case 9:
                viewCompletedTasks();
                break;
            case 10:
                viewPendingTasks();
                break;
            case 11:
                clearAllTasks();
                break;
            case 12:
                taskStatistics();
                break;
            case 13:
                postponeTask();
                break;
            case 14:
                printf("Exiting the task manager...\n");
                return 0;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
    return 0;
}
