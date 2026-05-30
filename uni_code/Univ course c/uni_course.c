#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_COURSES 7
#define MAX_SELECTED_COURSES 100
#define MAX_LINE_LENGTH 1000
#define NO_COURSES 6
#define MAX_COURSE_NAME_LENGTH 50
#define MAX_CHOICES 3
#define MAX_STUDENTS 200


#define RESET "\x1B[0m"
#define BOLD "\x1B[1m"
#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define YELLOW "\x1B[33m"
#define BLUE "\x1B[34m"
#define MAGENTA "\x1B[35m"
#define CYAN "\x1B[36m"
#define WHITE "\x1B[37m"
#define BOLD_RED "\033[1;31m"

const char* resetColor = "\033[0m";
const char* redColor = "\033[1;31m";
const char* greenColor = "\033[1;32m";
const char* yellowColor = "\033[1;33m";
const char* blueColor = "\033[1;34m";
const char* magentaColor = "\033[1;35m";
const char* cyanColor = "\033[1;36m";

typedef struct {
    char username[20];
    char password[20];
    int sem;
    float gpa;
    struct {
        char name[MAX_COURSE_NAME_LENGTH];
        int choices[MAX_CHOICES];
    } courses[MAX_COURSES];
} Student;

struct Core {
    char sname[50];
    int seats[3];
};

typedef struct {
    char username[50];
    float cgpa;
} waitlist_stud;


typedef struct {
    char username[100];
    char password[100];
    int semester;
    float cgpa;
    char core_courses[5][100];
    int core_faculty[5][3];
    char elective_course[100];
    int elective_faculty[3];
} User;

typedef struct {
    char name[100];
    int facultyPreferences[3];
} Allot;

typedef struct {
    char name[100];
} Course;

typedef struct {
    char username[20];
    char course[MAX_COURSE_NAME_LENGTH];
    int section;
} Allocation;

typedef struct {
    char username[20];
    char courses[MAX_COURSES][MAX_COURSE_NAME_LENGTH];
    char sections[MAX_COURSES];
} AllottedStudent;

typedef struct {
    char name[MAX_COURSE_NAME_LENGTH];
    int seats[3];
} Course_seat;

typedef struct {
    char name[50];
    float gpa;
    int choice1;
    int choice2;
    int choice3;
    int points;
} Wait_points;


void design(int num) {
    printf("\n\n");
    for (int i = 0; i < num; i++) {
        printf("=-=");
    }
    printf("\n\n");
}

void register_user() {
    User newUser;
    char user[100];
    char pass[100];
    int sem;
    float cgpa;

    design(40);
    printf("\t\t\t\t\t\tREGISTER PAGE\n\n");
    printf("\nThe username should have at least %s5%s characters and at most %s10%s characters.\n ",greenColor,resetColor,greenColor,resetColor);
    printf("\n\nEnter the username:");
    scanf("%s", user);

    printf("\nPassword should contain at least %s8%s characters.\n\nEnter the password: ",greenColor,resetColor);
    scanf("%s", pass);

    if (strlen(user) >= 5 && strlen(user) <= 10 && strlen(pass) >= 8) {
        printf(BOLD GREEN "\nUsername and Password are Successfully Registered\n" RESET);

        while (1) {
            printf("\n%s**%sThis system is defined for students moving towards 2nd semester\n",blueColor,resetColor);
            printf("\nEnter your new Semester number: ");
            scanf("%d", &sem);
            if (sem == 2) {
                break;
            } else {
                printf( BOLD RED "\nInvalid semester choice\n" RESET);
            }
        }

        while (1) {
            if (sem == 2) {
                printf("\nEnter your %sGPA%s of Sem 1: ",greenColor,resetColor);
                scanf("%f", &cgpa);
                if (cgpa <= 10) {
                    break;
                } else {
                    printf("\nEnter a valid GPA (less than %s10%s).\n",redColor,resetColor);
                }
            }
        }
        strcpy(newUser.username, user);
        strcpy(newUser.password, pass);
        newUser.semester = sem;
        newUser.cgpa = cgpa;

        FILE *fp;
        fp = fopen("users.csv", "a");
        if (fp != NULL) {
            fprintf(fp, "%s,%s,%d,%.2f\n", user, pass, sem, cgpa);
            fclose(fp);
        } else {
            perror( BOLD RED "Error opening users.csv" RESET);
        }
    } else {
        printf(BOLD RED "\nEnter a valid username and password.\n" RESET);
    }
}

int count_file_students(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("\nNo students in file..\n");
        return 0; // Return -1 to indicate an error
    }

    int count = 0;
    char line[256];

    while (fgets(line, sizeof(line), file)) {
        count++;
    }

    fclose(file);

    return count;
}

void print_student_count(int choice) {
    const char *filenames[] = {
        "alloted_students.csv",
        "reallocated_students.csv",
        "design_thinking_waitlist.csv",
        "machine_learning_waitlist.csv",
        "data_science_waitlist.csv",
        "applied_mathematics_waitlist.csv",
        "robotics_waitlist.csv",
        "theoretical_physics_waitlist.csv",
        "data_architecture_waitlist.csv",
        "psychology_waitlist.csv",
        "film_appreciation_waitlist.csv"
    };

    if (choice >= 1 && choice <= 11) {
        int count = count_file_students(filenames[choice - 1]);
        printf("Number of students in %s: %d\n", filenames[choice - 1],count);
    } else {
        printf("Invalid choice.\n");
    }
}


void write_to_csv(FILE *csv_file, const char *course_name, int section) {
    if (section > 0 && section <= 3) {
        fprintf(csv_file, ",%s,%d", course_name, section);
    }
}

void read_core_courses(struct Core *courses, int *subcount) {
    FILE *core_file = fopen("course_seats.csv", "r");
    if (core_file == NULL) {
        perror( BOLD RED "Error opening course_seats.csv" RESET );
        exit(1);
    }

    char line[100];
    int i = 0;
    while (fgets(line, sizeof(line), core_file)) {
        char *token = strtok(line, ",");
        if (token != NULL) {
            strcpy(courses[i].sname, token);
            for (int j = 0; j < 3; j++) {
                token = strtok(NULL, ",");
                if (token != NULL) {
                    courses[i].seats[j] = atoi(token);
                }
            }
            i++;
        }
    }
    *subcount = i;

    fclose(core_file);
}

void write_core_courses(const struct Core *courses, int subcount) {
    FILE *core_file = fopen("course_seats.csv", "w");
    if (core_file == NULL) {
        perror( BOLD RED "Error opening course_seats.csv for writing" RESET);
        exit(1);
    }

    for (int i = 0; i < subcount; i++) {
        fprintf(core_file, "%s,%d,%d,%d\n", courses[i].sname, courses[i].seats[0], courses[i].seats[1], courses[i].seats[2]);
    }

    fclose(core_file);
}


void empty_waiting_list(const char *course_name) {
    char filename[100];
    snprintf(filename, sizeof(filename), "%s_waiting_list.csv", course_name);
    FILE *waiting_file = fopen(filename, "w");
    if (waiting_file == NULL) {
        perror( BOLD RED "Error opening waiting list file" RESET);
        exit(1);
    }
    fclose(waiting_file);
}
void create_waiting_list(const char *course_name, Student *students, int i, int k) {
    char filename[100];
    int count=0;
    snprintf(filename, sizeof(filename), "%s_waiting_list.csv", course_name);
    FILE *waiting_file = fopen(filename, "a");
    if (waiting_file == NULL) {
        perror( BOLD RED "Error opening waiting list file" RESET);
        exit(1);
    }
    fprintf(waiting_file, "%s,%.2f,%d,%d,%d,%d\n", students[i].username, students[i].gpa, students[i].courses[k].choices[0], students[i].courses[k].choices[1], students[i].courses[k].choices[2], 0); // Initialize points to 0
    fclose(waiting_file);
}


int is_already_allocated(Allocation *allocations, int alloc_count, const char *username, const char *course) {
    for (int i = 0; i < alloc_count; i++) {
        if (strcmp(allocations[i].username, username) == 0 && strcmp(allocations[i].course, course) == 0) {
            return 1;
        }
    }
    return 0;
}

void read_allocated_students(Allocation *allocations, int *alloc_count) {
    FILE *csv_file = fopen("alloted_students.csv", "r");

    char line[MAX_LINE_LENGTH];
    *alloc_count = 0;

    while (fgets(line, sizeof(line), csv_file) != NULL) {
        sscanf(line, "%19[^,],%49[^,],%d", allocations[*alloc_count].username, allocations[*alloc_count].course, &allocations[*alloc_count].section);
        (*alloc_count)++;
    }

    fclose(csv_file);
}

void reallocate_waitlisted_students(Student students[],int num_students, struct Core *courses, int subcount) {
    read_core_courses(courses, &subcount); // Read the updated seat availability

    // Remove students from waiting list files if allocated
    for (int i = 0; i < subcount; i++) {
        for (int c = 0; c < 3; c++) {
            if (courses[i].seats[c] > 0) {
                char filename[100];
                snprintf(filename, sizeof(filename), "%s_waiting_list.csv", courses[i].sname);
                FILE *waiting_file = fopen(filename, "r");
                if (waiting_file != NULL) {
                    char waiting_line[1024];
                    FILE *temp_waiting_file = fopen("temp_waiting_list.csv", "w");
                    if (temp_waiting_file == NULL) {
                        perror("Error opening temp_waiting_list.csv");
                        fclose(waiting_file);
                        continue;
                    }

                    while (fgets(waiting_line, sizeof(waiting_line), waiting_file) != NULL) {
                        char wait_username[20];
                        sscanf(waiting_line, "%19[^,]", wait_username);

                        int reallocated = 0;
                        for (int k = 0; k < num_students; k++) {
                            if (strcmp(wait_username, students[k].username) == 0) {
                                for (int l = 0; l < NO_COURSES; l++) {
                                    if (strcmp(students[k].courses[l].name, courses[i].sname) == 0) {
                                        if (courses[i].seats[c] > 0) {
                                            courses[i].seats[c]--;
                                            alloted_update(wait_username,c,courses[i]);
                                            reallocated = 1;
                                            break;
                                        }
                                    }
                                }
                            }
                            if (reallocated) break;
                        }
                        if (!reallocated) {
                            fprintf(temp_waiting_file, "%s", waiting_line);
                        }
                    }

                    fclose(waiting_file);
                    fclose(temp_waiting_file);
                    remove(filename);
                    rename("temp_waiting_list.csv", filename);
                }
            }
        }
    }

    write_core_courses(courses, subcount); // Write back the updated seat counts
}

void alloted_update(char w_username[], int c, char cname[]) {
    char line[1024];
    FILE *reallocated_csv_file = fopen("reallocated_students.csv", "a");
    if (reallocated_csv_file == NULL) {
        perror("Error opening reallocated_students.csv");
        exit(1);
    }

    struct restud {
        char username[20];
        char courses[MAX_COURSES][MAX_COURSE_NAME_LENGTH];
        char sections[10];
    } all_stud[MAX_STUDENTS];

    FILE *fp = fopen("alloted_students.csv", "r");
    if (fp == NULL) {
        perror("Error opening alloted_students.csv");
        exit(1);
    }

    FILE *a_temp = fopen("temp_alloted.csv", "w");
    if (a_temp == NULL) {
        perror("Error opening temp_alloted.csv");
        exit(1);
    }

    int count = 0;
    while (fgets(line, sizeof(line), fp) != NULL) {
        sscanf(line, "%19[^,],%49[^,],%c,%49[^,],%c,%49[^,],%c,%49[^,],%c,%49[^,],%c,%49[^,],%c",
               all_stud[count].username,
               all_stud[count].courses[0], &all_stud[count].sections[0],
               all_stud[count].courses[1], &all_stud[count].sections[1],
               all_stud[count].courses[2], &all_stud[count].sections[2],
               all_stud[count].courses[3], &all_stud[count].sections[3],
               all_stud[count].courses[4], &all_stud[count].sections[4],
               all_stud[count].courses[5], &all_stud[count].sections[5]);
        count++;
    }

    for (int i = 0; i < count; i++) {
        if (strcmp(all_stud[i].username, w_username) == 0) {
            for (int j = 0; j < MAX_COURSES; j++) {
                if (strcmp(all_stud[i].courses[j], cname) == 0) {
                    all_stud[i].sections[j] = c + 1 + '0'; // Convert section number to char
                    break;
                }
            }
            fprintf(reallocated_csv_file, "%s,%s,%d\n", w_username, cname, c+1);
        }
    }

    for (int i = 0; i < count; i++) {
        fprintf(a_temp, "%s,%s,%c,%s,%c,%s,%c,%s,%c,%s,%c,%s,%c\n",
                all_stud[i].username,
                all_stud[i].courses[0], all_stud[i].sections[0],
                all_stud[i].courses[1], all_stud[i].sections[1],
                all_stud[i].courses[2], all_stud[i].sections[2],
                all_stud[i].courses[3], all_stud[i].sections[3],
                all_stud[i].courses[4], all_stud[i].sections[4],
                all_stud[i].courses[5], all_stud[i].sections[5]);
    }

    fclose(fp);
    fclose(a_temp);
    fclose(reallocated_csv_file);

    // Remove the old file and rename the new file
    remove("alloted_students.csv");
    rename("temp_alloted.csv", "alloted_students.csv");
}

int allocate_courses(Student *students, int num_students, struct Core *courses, int subcount) {
    int all_count=0;


    FILE *csv_file = fopen("alloted_students.csv", "w");
    for (int i=0;i<7;i++) {
        empty_waiting_list(courses[i].sname);
        empty_waiting_list("Film Appreciation");
        empty_waiting_list("Psychology");
    }

    if (csv_file == NULL) {
        return 0;
    }

    for (int i = 0; i < num_students; i++) {
        fprintf(csv_file, "%s", students[i].username);

        for (int k = 0; k < NO_COURSES; k++) {
            int course_flag = 0;
            if (strlen(students[i].courses[k].name) > 0) {
                for (int j = 0; j < subcount; j++) {
                    if (strcmp(students[i].courses[k].name, courses[j].sname) == 0) {
                        for (int c = 0; c < 3; c++) {
                            int section = students[i].courses[k].choices[c];
                            if (section >= 1 && section <= 3) {
                                if (courses[j].seats[section - 1] > 0) {
                                    courses[j].seats[section - 1]--;
                                    write_to_csv(csv_file, students[i].courses[k].name, section);
                                    all_count++;
                                    course_flag = 1;
                                    break;
                                }
                            }
                        }
                        if (!course_flag) {
                            fprintf(csv_file, ",%s,W", students[i].courses[k].name);
                            create_waiting_list(students[i].courses[k].name, students, i, k);
                            all_count++;
                        }
                        break;
                    }
                }
            }
        }

        fprintf(csv_file, "\n");
    }

    fclose(csv_file);
    return all_count;
}

int has_selected_courses(const char *username) {
    FILE *fp = fopen("user_coursechoices.csv", "r");
    if (fp == NULL) {
        return 0; // If the file doesn't exist, assume the student hasn't selected courses
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), fp) != NULL) {
        char stored_user[100];
        sscanf(line, "%[^,]", stored_user);
        if (strcmp(stored_user, username) == 0) {
            fclose(fp);
            return 1; // Student has selected courses
        }
    }

    fclose(fp);
    return 0; // Student hasn't selected courses
}

void read_and_print_alloted_courses(const char *username) {
    FILE *fp = fopen("alloted_students.csv", "r");

    char line[MAX_LINE_LENGTH];
    int found = 0;

    printf("\nAllotted Courses for %s%s%s:\n",cyanColor, username,resetColor);

    while (fgets(line, sizeof(line), fp) != NULL) {
        char file_username[20];
        char courses[MAX_COURSES][MAX_COURSE_NAME_LENGTH];
        char sections[10];
        int fields_read = sscanf(line, "%19[^,],%49[^,],%c,%49[^,],%c,%49[^,],%c,%49[^,],%c,%49[^,],%c,%49[^,],%c",
                                 file_username,
                                 courses[0], &sections[0],
                                 courses[1], &sections[1],
                                 courses[2], &sections[2],
                                 courses[3], &sections[3],
                                 courses[4], &sections[4],
                                 courses[5], &sections[5]);

        if (fields_read < 3) {
            printf("Error parsing line: %s\n", line);
            continue;
        }

        if (strcmp(file_username, username) == 0) {
            for (int i = 0; i < fields_read / 2; i++) {
                if(sections[i]!='W'){


                printf("Course: %s, Section: %s%c%s\n", courses[i],greenColor, sections[i],resetColor);
                }
                else
                {
                    printf("Course: %s, Section: %s%c%s\n", courses[i],redColor, sections[i],resetColor);
                }
            }
            found = 1;
            break; // No need to continue reading once the user's courses are found
        }
    }

    if (!found) {
        printf(BOLD RED "\nNo courses allotted for this user.\n" RESET);
    }

    fclose(fp);
}

int readCoursesFromCSV(const char* filename, Course* courses, int maxCourses) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf(BOLD RED "Error opening file.\n" RESET);
        return -1;
    }

    char line[MAX_LINE_LENGTH];
    int count = 0;
    while (fgets(line, sizeof(line), file) != NULL && count < maxCourses) {
        char* token = strtok(line, ",");
        while(token != NULL && count < maxCourses) {
            strcpy(courses[count].name, token);
            count++;
            token = strtok(NULL, ",");
        }
    }
    fclose(file);
    return count;
}

void select_courses(User *user) {
    FILE *fp_core, *fp_elective;
    char line[MAX_LINE_LENGTH];
    int facultyNo, m;
    int num, check, selectedCount = 0;
    Course courses[MAX_COURSES];

    int courseCount;

    courseCount = readCoursesFromCSV("core_courses.csv", courses, MAX_COURSES);
    if (courseCount == -1) {
        return;
    }

    design(40);

    printf(BOLD RED "\nAvailable Core courses: \n" RESET);

    fp_core = fopen("core_course_f.txt", "r");
    if (fp_core == NULL) {
        printf( BOLD RED "Error opening core courses file.\n" RESET);
        return;
    }

    for (int i = 0; i < courseCount; i++) {
        printf("[%d] %s\n", i + 1, courses[i].name);
    }

    while (1) {
        printf("\n\nStudents are required to choose  %s5%s core subjects this semester: \n",greenColor,resetColor);
        num = 5;
        if (num >= 3 && num <= 5) {
            for (int i = 0; i < num; i++) {
                printf("\nEnter the course %s[%d]%s number: ",greenColor, i + 1,resetColor);
                scanf("%d", &check);

                if (check >= 1 && check <= MAX_COURSES) {
                    int alreadySelected = 0;
                    for (int j = 0; j < selectedCount; j++) {
                        if (strcmp(courses[check - 1].name, user->core_courses[j]) == 0) {
                            alreadySelected = 1;
                            break;
                        }
                    }
                    if (!alreadySelected) {
                        strcpy(user->core_courses[selectedCount], courses[check - 1].name);

                        rewind(fp_core);
                        while (fgets(line, sizeof(line), fp_core) != NULL) {
                            char *token = strtok(line, ",");
                            if (token != NULL && strcmp(token, courses[check - 1].name) == 0) {
                                printf("\nSubject: %s\n", token);
                                token = strtok(NULL, ",");
                                printf("Info of faculty:\n");
                                while (token != NULL) {
                                    printf("  %s\n", token);
                                    token = strtok(NULL, ",");
                                }
                                for (m = 0; m < 3; m++) {
                                    do {
                                        printf("\nEnter your %d preference of faculty for %s (%s1%s to %s3%s): ", m + 1, courses[check - 1].name,greenColor,resetColor,greenColor,resetColor);
                                        scanf("%d", &facultyNo);
                                        getchar();
                                        if (facultyNo > 3 || facultyNo < 1)
                                            printf(BOLD RED "Enter a valid choice [The choices of faculty is 1, 2 or 3]" RESET);
                                    } while (facultyNo < 1 || facultyNo > 3);
                                    user->core_faculty[selectedCount][m] = facultyNo;
                                }
                            }
                        }

                        rewind(fp_core); // Reset file pointer for the next iteration
                        selectedCount++;
                    } else {
                        printf(BOLD YELLOW "Course already selected. Please choose another course.\n" RESET);
                        i--;
                    }
                } else {
                    printf( BOLD RED "Invalid course number.\n" RESET);
                    i--;
                }
            }
            break;
        } else {
            printf(BOLD RED "Invalid number of courses. Please give a valid number of courses. You should choose 5 courses.\n" RESET);
        }
    }

    int echoice;
    printf("\nChoose the elective course:\n");
    printf("[%s1%s] Psychology \n[%s2%s] Film Appreciation\n",greenColor,resetColor,greenColor,resetColor);

    while (1) {
        printf("Enter the course number:");
        scanf("%d", &echoice);
        if (echoice == 1) {
            strcpy(user->elective_course, "Psychology");
            break;
        } else if (echoice == 2) {
            strcpy(user->elective_course, "Film Appreciation");
            break;
        } else {
            printf(BOLD RED "Enter a valid choice\n" RESET);
        }
    }

    fp_elective = fopen("elective_course_f.txt", "r");
    if (fp_elective == NULL) {
        printf( BOLD RED "Error opening elective course file.\n" RESET);
        return;
    }

    while (fgets(line, sizeof(line), fp_elective) != NULL) {
        char *token = strtok(line, ",");
        if (token != NULL && strcmp(token, user->elective_course) == 0) {
            printf("\nSubject: %s%s%s\n", blueColor,token,resetColor);
            printf("Info of faculty:\n");
            while (token != NULL) {
                printf("  %s\n", token);
                token = strtok(NULL, ",");
            }

            for (m = 0; m < 2; m++) {
                do {
                    printf("\nEnter your %d preference of faculty for Elective (%s1%s or %s2%s): ", m + 1,greenColor,resetColor,greenColor,resetColor);
                    scanf("%d", &facultyNo);
                    getchar();
                    if (facultyNo > 2 || facultyNo < 1)
                        printf(BOLD RED "Enter a valid choice [The choices of faculty is 1 or 2]" RESET);
                } while (facultyNo < 1 || facultyNo > 2);
                user->elective_faculty[m] = facultyNo;
            }
            user->elective_faculty[2] = 0; // No third preference for elective
        }
    }

    printf(BOLD GREEN "\nYOUR CHOICES HAVE BEEN SUCCESSFULLY REGISTERED\n " RESET);
    design(40);

    fclose(fp_core);
    fclose(fp_elective);

    FILE *fp = fopen("user_coursechoices.csv", "a");
    if (fp != NULL) {
        fprintf(fp, "%s,%s,%d,%.2f,", user->username, user->password, user->semester, user->cgpa);
        for (int i = 0; i < selectedCount; i++) {
            fprintf(fp, "%s,", user->core_courses[i]);
            for (m = 0; m < 3; m++) {
                fprintf(fp, "%d", user->core_faculty[i][m]);
                if (m < 2) {
                    fprintf(fp, ",");
                } else {
                    fprintf(fp, ",");
                }
            }
        }
        fprintf(fp, "%s,", user->elective_course);
        for (m = 0; m < 2; m++) {
            fprintf(fp, "%d", user->elective_faculty[m]);
            if (m < 1) {
                fprintf(fp, ",");
            }
        }
        fprintf(fp, "\n");
        fclose(fp);
    } else {
        printf(BOLD RED "Error opening user_coursechoices.csv.\n" RESET);
    }
}

void login_user(User* logged_in_user) {
    char log_user[100];
    char log_pass[100];

    for (int l = 1; l <= 3; l++) { // Loop for three login attempts
        printf("ENTER YOUR USERNAME: ");
        scanf("%s", log_user);

        printf("ENTER YOUR PASSWORD: ");
        scanf("%s", log_pass);

        FILE *fp;
        fp = fopen("users.csv", "r");
        char line[200];
        int logged_in = 0;
        while (fgets(line, sizeof(line), fp) != NULL) {
            char stored_user[100];
            char stored_pass[100];
            int sem;
            float cgpa;
            sscanf(line, "%[^,],%[^,],%d,%f", stored_user, stored_pass, &sem, &cgpa);
            if (strcmp(log_user, stored_user) == 0 && strcmp(log_pass, stored_pass) == 0) {
                printf(BOLD GREEN "You are logged in successfully\n" RESET);
                strcpy(logged_in_user->username, stored_user);
                strcpy(logged_in_user->password, stored_pass);
                logged_in_user->semester = sem;
                logged_in_user->cgpa = cgpa;
                fclose(fp);
                logged_in = 1;

                if (has_selected_courses(logged_in_user->username)) {
                    printf(BOLD YELLOW "\nYou have already selected your courses.\n" RESET);
                    read_and_print_alloted_courses(logged_in_user->username);
                } else {
                    select_courses(logged_in_user);
                }

                return; // Exit the function after successful login
            }
        }
        fclose(fp);
        printf( BOLD RED "\nUsername or password is wrong.\nPlease enter a valid username or password.\n" RESET);
        printf("\nYou have %s%d%s attempts remaining.\n\n",greenColor, 3 - l,resetColor);
    }
}

void read_users_from_csv(User users[], int *count) {
    FILE *fp;
    fp = fopen("users.csv", "r");
    if (fp == NULL) {
        printf(BOLD RED "Error opening file.\n" RESET);
        return;
    }

    char line[200];
    *count = 0;

    while (fgets(line, sizeof(line), fp) != NULL && *count < 100) {
        sscanf(line, "%[^,],%[^,],%d,%f",
               users[*count].username, users[*count].password,
               &users[*count].semester, &users[*count].cgpa);
        (*count)++;
    }

    fclose(fp);
}




int admin_login() {
    char username[100], password[100];
    const char correct_user[] = "admin";
    const char correct_pass[] = "password123";

    printf("ENTER ADMIN USERNAME: ");
    scanf("%s", username);

    printf("ENTER ADMIN PASSWORD: ");
    scanf("%s", password);

    if (strcmp(username, correct_user) == 0 && strcmp(password, correct_pass) == 0) {
        printf(BOLD GREEN "Admin logged in successfully.\n" RESET);
        return 1;
    } else {
        printf( BOLD RED "Admin username or password is wrong.\n" RESET);
        return 0;
    }
}

void sort_and_write_csv(const char *input_file, const char *output_file) {
    FILE *fp_in = fopen(input_file, "r");
    FILE *fp_out = fopen(output_file, "w");
    if (fp_in == NULL || fp_out == NULL) {
        printf(BOLD RED "Error opening files.\n" RESET);
        if (fp_in) fclose(fp_in);
        if (fp_out) fclose(fp_out);
        return;
    }

    User users[100];
    int count = 0;
    char line[MAX_LINE_LENGTH];

    // Read data from input file
    while (fgets(line, sizeof(line), fp_in) != NULL && count < 100) {
        sscanf(line, "%[^,],%[^,],%d,%f",
               users[count].username, users[count].password,
               &users[count].semester, &users[count].cgpa);
        // Parsing the core courses and elective course
        char *token = strtok(line, ",");
        for (int i = 0; i < 4; i++) token = strtok(NULL, ",");  // Skip to the courses

        for (int i = 0; i < 5; i++) {
            if (token != NULL) {
                strcpy(users[count].core_courses[i], token);
                token = strtok(NULL, ",");
                for (int j = 0; j < 3; j++) {
                    if (token != NULL) {
                        users[count].core_faculty[i][j] = atoi(token);
                        token = strtok(NULL, ",");
                    }
                }
            }
        }
        if (token != NULL) {
            strcpy(users[count].elective_course, token);
            token = strtok(NULL, ",");
            for (int j = 0; j < 2; j++) {
                if (token != NULL) {
                    users[count].elective_faculty[j] = atoi(token);
                    token = strtok(NULL, ",");
                }
            }
        }
        count++;
    }
    fclose(fp_in);

    // Sort users by CGPA
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (users[j].cgpa < users[j + 1].cgpa) {
                User temp = users[j];
                users[j] = users[j + 1];
                users[j + 1] = temp;
            }
        }
    }

    // Write sorted data to output file
    for (int i = 0; i < count; i++) {
        fprintf(fp_out, "%s,%s,%d,%.2f,", users[i].username, users[i].password, users[i].semester, users[i].cgpa);
        for (int j = 0; j < 5; j++) {
            if (strlen(users[i].core_courses[j]) > 0) {
                fprintf(fp_out, "%s,%d,%d,%d,", users[i].core_courses[j], users[i].core_faculty[j][0], users[i].core_faculty[j][1], users[i].core_faculty[j][2]);
            }
        }
        if (strlen(users[i].elective_course) > 0) {
            fprintf(fp_out, "%s,%d,%d", users[i].elective_course, users[i].elective_faculty[0], users[i].elective_faculty[1]);
        }
        fprintf(fp_out, "\n");
    }
    fclose(fp_out);
}

void admin_functions() {
    int choice;
    while (1) {
        printf(BOLD BLUE "\nADMIN MENU\n" RESET);
        printf("1. Sort and save user choices by CGPA\n");
        printf("2. Allot users and send remaining to waiting list\n");
        printf("3. Reallot users from waitlist \n");
        printf("4. Get the students status count \n");
        printf("5. Exit to main menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

    const char *filename = "sorted_user_coursechoices.csv";
    Student students[100];
    int num_students = 0;

    FILE *csv_read_file = fopen(filename, "r");
    char buffer[1024];

    while (fgets(buffer, sizeof(buffer), csv_read_file) != NULL) {
        sscanf(buffer, "%19[^,],%19[^,],%d,%f,%49[^,],%d,%d,%d,%49[^,],%d,%d,%d,%49[^,],%d,%d,%d,%49[^,],%d,%d,%d,%49[^,],%d,%d,%d,%49[^,],%d,%d,%d",
               students[num_students].username,
               students[num_students].password,
               &students[num_students].sem,
               &students[num_students].gpa,
               students[num_students].courses[0].name, &students[num_students].courses[0].choices[0], &students[num_students].courses[0].choices[1], &students[num_students].courses[0].choices[2],
               students[num_students].courses[1].name, &students[num_students].courses[1].choices[0], &students[num_students].courses[1].choices[1], &students[num_students].courses[1].choices[2],
               students[num_students].courses[2].name, &students[num_students].courses[2].choices[0], &students[num_students].courses[2].choices[1], &students[num_students].courses[2].choices[2],
               students[num_students].courses[3].name, &students[num_students].courses[3].choices[0], &students[num_students].courses[3].choices[1], &students[num_students].courses[3].choices[2],
               students[num_students].courses[4].name, &students[num_students].courses[4].choices[0], &students[num_students].courses[4].choices[1], &students[num_students].courses[4].choices[2],
               students[num_students].courses[5].name, &students[num_students].courses[5].choices[0], &students[num_students].courses[5].choices[1], &students[num_students].courses[5].choices[2]);

        num_students++;
    }

    fclose(csv_read_file);

    struct Core courses[10];
    int subcount = 0;

        if (choice == 1) {
            sort_and_write_csv("user_coursechoices.csv", "sorted_user_coursechoices.csv");
            printf(BOLD GREEN "User choices sorted by CGPA and saved to sorted_user_coursechoices.csv\n" RESET);
        } else if (choice == 2) {

            read_core_courses(courses, &subcount);
            int count = allocate_courses(students, num_students, courses, subcount);
            write_core_courses(courses, subcount);
            if (count>0) {
                printf("\nThe students have been successfully alloted...\n");
                printf("Allotement done for %d students.\n",count/6);
            }
            else
                printf("\nFirst sort the students...\n");


        } else if (choice == 3) {
            reallocate_waitlisted_students(students, num_students, courses, subcount);
        }
         else if (choice == 4) {
             printf("\n");
             printf(BOLD YELLOW"Check status\n" RESET);
             printf("1. Alloted Students\n");
             printf("2. Realloted Students\n");
             printf("3. Design Thinking Waiting List\n");
             printf("4. Machine Learning Waiting List\n");
             printf("5. Data Science Waiting List\n");
             printf("6. Applied Mathematics Waiting List\n");
             printf("7. Robotics Waiting List\n");
             printf("8. Theoretical Physics Waiting List\n");
             printf("9. Data Architecture Waiting List\n");
             printf("10. Psychology Waiting List\n");
             printf("11. Film Appreciation Waiting List\n");
             printf("\n\tEnter your choice: ");
             scanf("%d", &choice);

             print_student_count(choice);

        }
         else if (choice == 5) {
             break;
         }
        else {
            printf(BOLD RED "Invalid choice. Please try again.\n" RESET);
        }
    }
}


int has_alloted_courses(const char *username) {
    FILE *fp = fopen("alloted_students.csv", "r");
    if (fp == NULL) {
        return 0; // If the file doesn't exist, assume the student hasn't selected courses
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), fp) != NULL) {
        char stored_user[100];
        sscanf(line, "%[^,]", stored_user);
        if (strcmp(stored_user, username) == 0) {
            fclose(fp);
            return 1; // Student has selected courses
        }
    }

    fclose(fp);
    return 0; // Student hasn't selected courses
}


int in_waiting_list(const char *username) {
    FILE *fp = fopen("alloted_students.csv", "r");
    if (fp == NULL) {
        return 0; // If the file doesn't exist, assume the student hasn't selected courses
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), fp) != NULL) {
        char stored_user[100];
        sscanf(line, "%[^,]", stored_user);
        if (strcmp(stored_user, username) == 0) {
            if (strstr(line, ",W") != NULL) {
                fclose(fp);
                return 1; // Student is in the waiting list
            }
            fclose(fp);
            return 0; // Student is found but not in the waiting list
        }
    }
}

void read_courses_csv(Course_seat courses[], int *count) {
    FILE *file = fopen("course_seats.csv", "r");
    if (file == NULL) {
        perror(BOLD RED "Error opening courses file" RESET);
        return;
    }

    char line[MAX_LINE_LENGTH];
    *count = 0;

    while (fgets(line, sizeof(line), file)) {
        Course_seat course = {0};
        char *token = strtok(line, ",");
        strcpy(course.name, token);

        for (int i = 0; i < 3; i++) {
            token = strtok(NULL, ",");
            course.seats[i] = atoi(token);
        }

        courses[(*count)++] = course;
    }

    fclose(file);
}

void read_allotted_students_csv(AllottedStudent students[], int *count) {
    FILE *file = fopen("alloted_students.csv", "r");

    char line[MAX_LINE_LENGTH];
    *count = 0;

    while (fgets(line, sizeof(line), file)) {
        AllottedStudent student = {0};
        char *token = strtok(line, ",");
        strcpy(student.username, token);

        int i = 0;
        while ((token = strtok(NULL, ",")) != NULL && i < MAX_COURSES * 2) {
            if (i % 2 == 0) {
                strcpy(student.courses[i / 2], token);
            } else {
                student.sections[i / 2] = token[0];
            }
            i++;
        }

        students[(*count)++] = student;
    }

    fclose(file);
}

void change_alloted_csv(AllottedStudent students[], Course_seat courses[], int courses_count, char newcourse[30], char oldcourse[30], char username[30], int *count) {
    int new_course_index = -1;
    for (int i = 0; i < courses_count; i++) {
        if (strcmp(newcourse, courses[i].name) == 0) {
            new_course_index = i;
            break;
        }
    }

    int old_course_index = -1;
    for (int i = 0; i < courses_count; i++) {
        if (strcmp(oldcourse, courses[i].name) == 0) {
            old_course_index = i;
            break;
        }
    }

    if (new_course_index == -1 || old_course_index == -1) {
        fprintf(stderr, "Course not found.\n");
        return;
    }

    for (int i = 0; i < *count; i++) {
        if (strcmp(username, students[i].username) == 0) {
            for (int j = 0; j < MAX_COURSES; j++) {
                if (strcmp(oldcourse, students[i].courses[j]) == 0) {
                    strcpy(students[i].courses[j], newcourse);
                    if (students[i].sections[j] != 'W') {
                        courses[old_course_index].seats[students[i].sections[j] - '1']++;
                    }
                    for (int k = 0; k < 3; k++) {
                        if (courses[new_course_index].seats[k] > 0) {
                            students[i].sections[j] = '1' + k;
                            courses[new_course_index].seats[k]--;
                            break;
                        }
                    }
                    if (courses[new_course_index].seats[0] == 0 && courses[new_course_index].seats[1] == 0 && courses[new_course_index].seats[2] == 0) {
                        students[i].sections[j] = 'W';
                        char line[100];
                        FILE *file = fopen("users.csv", "r");
                        if (file == NULL) {
                            fprintf(stderr, "Could not open users.csv file\n");
                            return;
                        }

                        waitlist_stud targetUser;
                        int found = 0;

                        while (fgets(line, sizeof(line), file)) {
                            char name[50], pass[50];
                            int sem;
                            float cgpa;

                            sscanf(line, "%[^,],%[^,],%d,%f", name, pass, &sem, &cgpa);

                            if (strcmp(username, name) == 0) {
                                strcpy(targetUser.username, name);
                                targetUser.cgpa = cgpa;
                                found = 1;
                                break;
                            }
                        }

                        fclose(file);

                        if (found) {
                            char filename[100];
                            snprintf(filename, sizeof(filename), "%s_waiting_list.csv", newcourse);
                            FILE *waiting_file = fopen(filename, "a");
                            if (waiting_file == NULL) {
                                perror(BOLD RED "Error opening waiting list file" RESET);
                                exit(1);
                            }
                            fprintf(waiting_file, "%s,%.2f,1,2,3,0\n", targetUser.username, targetUser.cgpa);
                            fclose(waiting_file);
                        } else {
                            printf("Username %s%s%s not found\n",redColor, username,resetColor);
                        }
                    }
                }
            }
        }
    }

    FILE *temp = fopen("temp_alloted.csv", "w");
    for (int i = 0; i < *count; i++) {
        fprintf(temp, "%s", students[i].username);
        for (int j = 0; j < MAX_COURSES; j++) {
            if (students[i].courses[j][0] != '\0') {
                fprintf(temp, ",%s,%c", students[i].courses[j], students[i].sections[j]);
            }
        }
        fprintf(temp, "\n");
    }
    fclose(temp);
    remove("alloted_students.csv");
    rename("temp_alloted.csv", "alloted_students.csv");

    temp = fopen("temp_courses.csv", "w");
    for (int i = 0; i < courses_count; i++) {
        fprintf(temp, "%s,%d,%d,%d\n", courses[i].name, courses[i].seats[0], courses[i].seats[1], courses[i].seats[2]);
    }
    fclose(temp);
    remove("course_seats.csv");
    rename("temp_courses.csv", "course_seats.csv");

    printf(BOLD GREEN "Your changes have been recorded....\nLOGIN AGAIN TO CHECK RESULTS\n" RESET);
}

int count_students(Student students[]){
    int num_students = 0;
    FILE *csv_read_file = fopen("sorted_user_coursechoices.csv", "r");
    if (csv_read_file == NULL) {
        perror( BOLD RED "Error opening sorted_user_coursechoices.csv" RESET);
        return 1;
    }

    char buffer[1024];

    while (fgets(buffer, sizeof(buffer), csv_read_file) != NULL) {
        sscanf(buffer, "%19[^,],%19[^,],%d,%f,%49[^,],%d,%d,%d,%49[^,],%d,%d,%d,%49[^,],%d,%d,%d,%49[^,],%d,%d,%d,%49[^,],%d,%d,%d,%49[^,],%d,%d,%d",
               students[num_students].username,
               students[num_students].password,
               &students[num_students].sem,
               &students[num_students].gpa,
               students[num_students].courses[0].name, &students[num_students].courses[0].choices[0], &students[num_students].courses[0].choices[1], &students[num_students].courses[0].choices[2],
               students[num_students].courses[1].name, &students[num_students].courses[1].choices[0], &students[num_students].courses[1].choices[1], &students[num_students].courses[1].choices[2],
               students[num_students].courses[2].name, &students[num_students].courses[2].choices[0], &students[num_students].courses[2].choices[1], &students[num_students].courses[2].choices[2],
               students[num_students].courses[3].name, &students[num_students].courses[3].choices[0], &students[num_students].courses[3].choices[1], &students[num_students].courses[3].choices[2],
               students[num_students].courses[4].name, &students[num_students].courses[4].choices[0], &students[num_students].courses[4].choices[1], &students[num_students].courses[4].choices[2],
               students[num_students].courses[5].name, &students[num_students].courses[5].choices[0], &students[num_students].courses[5].choices[1], &students[num_students].courses[5].choices[2]);

        num_students++;
    }

    fclose(csv_read_file);
    return num_students;
}


int compare_students(const void *a, const void *b) {
    Wait_points *studentA = (Wait_points *)a;
    Wait_points *studentB = (Wait_points *)b;

    // Sort primarily by points (descending)
    return studentB->points - studentA->points;
}

void sort_waitlist(const char *input_filename, const char *output_filename) {
    FILE *file = fopen(input_filename, "r");
    if (file == NULL) {
        perror("Unable to open input file");
        return;
    }

    Wait_points students[100];
    int count = 0;
    char line[256];

    // Read file and store data in array
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%49[^,],%f,%d,%d,%d,%d",
               students[count].name,
               &students[count].gpa,
               &students[count].choice1,
               &students[count].choice2,
               &students[count].choice3,
               &students[count].points);
        count++;
    }

    fclose(file);

    // Sort the students array based on points
    qsort(students, count, sizeof(Wait_points), compare_students);

    // Write the sorted data back to the file
    file = fopen(output_filename, "w");
    if (file == NULL) {
        perror("Unable to open output file");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s,%.2f,%d,%d,%d,%d\n",
                students[i].name,
                students[i].gpa,
                students[i].choice1,
                students[i].choice2,
                students[i].choice3,
                students[i].points);
    }

    fclose(file);

    remove(input_filename);
    rename(output_filename,input_filename);
}

void update_waitlist_points(Student *students, char username[]) {
    int hackathon_points = 0, reservation_points = 0, internship_points = 0, research_projects = 0, courses_done = 0;
    char answer[4];
    int num_students = count_students(students);

    // Prompt for hackathon points
    while (1) {
        printf("\nHave you won any hackathons (%syes%s/%sno%s)? ", greenColor, resetColor, redColor, resetColor);
        scanf("%3s", answer);
        if (strcmp(answer, "yes") == 0) {
            hackathon_points = 5;
            break;
        } else if (strcmp(answer, "no") == 0) {
            hackathon_points = 0;
            break;
        } else {
            printf(BOLD_RED "\nInvalid choice. Please enter 'yes' or 'no'." RESET);
        }
    }

    // Prompt for reservation points
    while (1) {
        printf("\nAre you 7.5%% reservation (%syes%s/%sno%s)? ", greenColor, resetColor, redColor, resetColor);
        scanf("%3s", answer);
        if (strcmp(answer, "yes") == 0) {
            reservation_points = 3;
            break;
        } else if (strcmp(answer, "no") == 0) {
            reservation_points = 0;
            break;
        } else {
            printf(BOLD_RED "\nInvalid choice. Please enter 'yes' or 'no'." RESET);
        }
    }

    // Prompt for internship points
    while (1) {
        printf("\nHave you participated in any district level sports (%syes%s/%sno%s)? ", greenColor, resetColor, redColor, resetColor);
        scanf("%3s", answer);
        if (strcmp(answer, "yes") == 0) {
            internship_points = 6;
            break;
        } else if (strcmp(answer, "no") == 0) {
            internship_points = 0;
            break;
        } else {
            printf(BOLD_RED "\nInvalid choice. Please enter 'yes' or 'no'." RESET);
        }
    }

    // Prompt for research projects
    while (1) {
        printf("\nHave you done any research projects (%syes%s/%sno%s)? ", greenColor, resetColor, redColor, resetColor);
        scanf("%3s", answer);
        if (strcmp(answer, "yes") == 0) {
            research_projects = 7;
            break;
        } else if (strcmp(answer, "no") == 0) {
            research_projects = 0;
            break;
        } else {
            printf(BOLD_RED "\nInvalid choice. Please enter 'yes' or 'no'." RESET);
        }
    }

    // Prompt for extra courses
    while (1) {
        printf("\nHave you done any extra courses in online platforms like Coursera or Udemy (%syes%s/%sno%s)? ", greenColor, resetColor, redColor, resetColor);
        scanf("%3s", answer);
        if (strcmp(answer, "yes") == 0) {
            courses_done = 3;
            break;
        } else if (strcmp(answer, "no") == 0) {
            courses_done = 0;
            break;
        } else {
            printf(BOLD_RED "\nInvalid choice. Please enter 'yes' or 'no'." RESET);
        }
    }

    int total_points = hackathon_points + reservation_points + internship_points + research_projects + courses_done;

    char course_names[7][MAX_COURSE_NAME_LENGTH] = {0};
    int student_found = 0;

    for (int i = 0; i < num_students; i++) {
        if (strcmp(students[i].username, username) == 0) {
            student_found = 1;
            for (int j = 0; j < NO_COURSES; j++) {
                strcpy(course_names[j], students[i].courses[j].name);
            }
            break;
        }
    }

    if (!student_found) {
        printf(BOLD RED "Student not found." RESET "\n");
        return;
    }

    for (int i = 0; i < 7; i++) {
        char filename[100];
        snprintf(filename, sizeof(filename), "%s_waiting_list.csv", course_names[i]);

        FILE *waiting_file = fopen(filename, "r");
        if (waiting_file == NULL) {
            continue;
        }

        char temp_filename[] = "temp_waiting_list.csv";
        FILE *temp_waiting_file = fopen(temp_filename, "w");
        if (temp_waiting_file == NULL) {
            fclose(waiting_file);
            printf(BOLD RED "Failed to create temporary file." RESET "\n");
            return;
        }

        char waiting_line[1024];
        while (fgets(waiting_line, sizeof(waiting_line), waiting_file) != NULL) {
            char wait_username[20];
            float gpa;
            int choice1, choice2, choice3, points;
            sscanf(waiting_line, "%19[^,],%f,%d,%d,%d,%d", wait_username, &gpa, &choice1, &choice2, &choice3, &points);

            if (strcmp(username, wait_username) == 0) {
                points = total_points; // Update points with the new total points
            }
            fprintf(temp_waiting_file, "%s,%.2f,%d,%d,%d,%d\n", wait_username, gpa, choice1, choice2, choice3, points);
        }


        fclose(waiting_file);
        fclose(temp_waiting_file);
        char new_filename[]={"tempfile"};
        sort_waitlist(temp_filename,new_filename);
        if (remove(filename) != 0) {
            printf(BOLD RED "Error deleting original file." RESET "\n");
            remove(temp_filename);
        } else if (rename(temp_filename, filename) != 0) {
            printf(BOLD RED "Error renaming temporary file." RESET "\n");
        }

    }
}



void modify_allotted_courses(User *user) {
    AllottedStudent students[100];
    Course courses[MAX_STUDENTS];
    int count = 0;
    read_allotted_students_csv(students, &count);

    char allotted_courses[MAX_COURSES][MAX_COURSE_NAME_LENGTH] = {""};
    char sections[MAX_COURSES] = {0};
    int drop_index, add_index;
    Course available_courses[MAX_COURSES];
    int courseCount = readCoursesFromCSV("core_courses.csv", available_courses, MAX_COURSES);

    // Find the current user's allotted courses
    for (int i = 0; i < count; i++) {
        if (strcmp(students[i].username, user->username) == 0) {
            for (int j = 0; j < MAX_COURSES; j++) {
                strcpy(allotted_courses[j], students[i].courses[j]);
                sections[j] = students[i].sections[j];
            }
            break;
        }
    }

    // Display current courses
    printf("Your current %score%s courses are:\n",yellowColor,resetColor);
    for (int i = 0; i < MAX_COURSES-2; i++) {
        if (strlen(allotted_courses[i]) > 0) {
            printf("%d. %s (Section %c)\n", i + 1, allotted_courses[i], sections[i]);
        }
    }

    // Ask which course to drop
    printf("Enter the %sumber%s of the course you want to drop: ",greenColor,resetColor);
    scanf("%d", &drop_index);
    drop_index--; // Adjust for 0-based index

    if (drop_index < 0 || drop_index >= MAX_COURSES-2 || strlen(allotted_courses[drop_index]) == 0) {
        printf(BOLD RED "Invalid choice.\n"RESET );
        return;
    }

    char dropped_course[MAX_COURSE_NAME_LENGTH];
    strcpy(dropped_course, allotted_courses[drop_index]);
    allotted_courses[drop_index][0] = '\0'; // Mark as empty

    // Display available courses to add
    printf(BOLD YELLOW "Available core courses to add:\n" RESET);
    for (int i = 0; i < courseCount; i++) {
        int already_selected = 0;
        for (int j = 0; j < MAX_COURSES; j++) {
            if (strcmp(allotted_courses[j], available_courses[i].name) == 0) {
                already_selected = 1;
                break;
            }
        }
        if (!already_selected) {
            printf("%d. %s\n", i + 1, available_courses[i].name);
        }
    }

    // Ask which new course to add
    printf("Enter the %snumber%s of the course you want to add: ",greenColor,resetColor);
    scanf("%d", &add_index);
    add_index--; // Adjust for 0-based index

    if (add_index < 0 || add_index >= courseCount) {
        printf( BOLD RED "Invalid choice.\n" RESET);
        return;
    }

    char new_course[MAX_COURSE_NAME_LENGTH];
    strcpy(new_course, available_courses[add_index].name);

    int courses_count = 0;
    read_courses_csv(courses, &courses_count);
    AllottedStudent student[MAX_STUDENTS];
    read_allotted_students_csv(student, &count);
    change_alloted_csv(student, courses, courses_count, new_course, dropped_course, user->username , &count);
}

void printWelcome() {
    printf(" __        __   _                            _          _   _ \n");
    printf(" \\ \\      / /__| | ___ ___  _ __ ___   ___  | |_ ___   | | | |\n");
    printf("  \\ \\ /\\ / / _ \\ |/ __/ _ \\| '_ ` _ \\ / _ \\ | __/ _ \\  | | | |\n");
    printf("   \\ V  V /  __/ | (_| (_) | | | | | |  __/ | || (_) | |_| |_|\n");
    printf("    \\_/\\_/ \\___|_|\\___\\___/|_| |_| |_|\\___|  \\__\\___/  (_) (_)\n");
}

int main() {
    int a;
    User users[100];
    Course courses[MAX_STUDENTS];
    int user_count = 0;
    User logged_in_user;
    int user_logged_in = 0; // Flag to track if a user is logged in
    AllottedStudent students[100];
    int allotted_count = 0;

    read_allotted_students_csv(students, &allotted_count);
    printWelcome();

    while (1) {
        design(40);
        printf("\t\t\t\tUNIVERSITY COURSE SELECTION SYSTEM \n ");
        printf(BOLD YELLOW "MAIN MENU\n\n" RESET);
        printf(" 1. REGISTRATION\n");
        printf(" 2. LOGIN\n");
        printf(" 3. ADMIN LOGIN\n");
        printf(" 4. EXIT\n\n");
        printf(" Enter your choice: ");
        scanf("%d", &a);
        if (a == 1) {
            register_user();
        } else if (a == 2) {
            login_user(&logged_in_user);

            if (strlen(logged_in_user.username) > 0) {
                read_users_from_csv(users, &user_count);
                users[user_count++] = logged_in_user; // Add the logged in user with course selections
                user_logged_in = 1; // Set the flag to indicate user is logged in

                // Prompt for course modification if the user is logged in and has courses allotted
                if (has_alloted_courses(logged_in_user.username)) {
                    int modify_choice;
                    int flag5=1;
                    while(flag5==1)
                    {
                    printf("\n1.Do you want to modify your course selection?\n");
                    printf("\n2. To enter details for waitlist ranking. \n");
                    printf("\n3.Exit\n");
                    printf("\n\tEnter your choice: ");
                    scanf("%d", &modify_choice);
                    if (modify_choice == 1) {
                        modify_allotted_courses(&logged_in_user);
                    }
                    else if(modify_choice ==2) {
                        if (in_waiting_list(logged_in_user.username)==1){
                            update_waitlist_points(students,logged_in_user.username);
                        }
                        else
                            printf( BOLD YELLOW "\nYou are not in any waiting list..\n" RESET);

                    }
                    else if(modify_choice == 3){

                       flag5=0;

                    }
                    else{
                        printf(BOLD RED "invalid choice\n" RESET);
                    }
                }
               }
            }
        } else if (a == 3) {
            if (admin_login()) {
                admin_functions();
            }
        } else if (a == 4) {
            break;
        } else {
            printf(BOLD RED "Enter a valid choice.\n" RESET);
        }
    }

    return 0;
}
