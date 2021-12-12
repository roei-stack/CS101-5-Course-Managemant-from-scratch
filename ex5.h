
/***********
* Roei Cohen
* 325714152
* 01
* ex5
***********/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

#pragma warning(disable:4996)

#define MAX 201
#define COURSE_NUM_SIZE 5
#define COURSE_NAME_MAX_SIZE 30
#define COURSE_NAME_MIN_SIZE 1
#define STD_NAME_MAX_SIZE 20
#define STD_NAME_MIN_SIZE 1

#ifndef HEADERNAME_DEFINED
#define HEADERNAME_DEFINED

typedef struct CourseList CourseList;
typedef struct CourseItem CourseItem;
typedef struct Course Course;
typedef struct StudentList StudentList;
typedef struct StudentItem StudentItem;
typedef struct Student Student;

CourseList* createCourseList();
void choiceToAction(CourseList*);
void menu(int);
#endif // HEADERNAME_DEFINED
