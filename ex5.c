
/***********
* Roei Cohen
* 325714152
* 01
* ex5
***********/

#include "ex5.h"

struct Student
{
	char* name;
};
struct StudentItem
{
	//each student item is a chain that contains the students grade in that course
	unsigned int grade;
	//a pointer to the students data
	Student* data;
	//pointer to the next studentItem
	struct StudentItem* next;
};
struct StudentList
{
	//each course will have its own studentList member
	StudentItem* first, * last;
};
struct Course
{
	//the course name
	char* name;
	//the id
	char id[6];
	//the classlist will contain students with thier grades
	StudentList* calssList;
};
struct CourseItem
{
	//defining courseItem members
	Course* data;
	struct CourseItem* next;
};
struct CourseList
{
	//creating the linked list structure, starting with courseList at the top
	CourseItem* first, * last;
};

//0 -> freeing all allocaten memory

/******************
* Function Name: freeStudent
* Input: a pointer to a student
* Output: freeing the student memory (none)
* Function Operation: frees the member "name", then frees the structure
******************/
void freeStudent(Student* s)
{
	if (s != NULL)
	{
		free(s->name);
		free(s);
	}
}

/******************
* Function Name:freeStudentItem
* Input: a student item
* Output: frees the list throughout the item until the end of the list (none)
* Function Operation: free the current student, free the next one using reccursion
* then free the item itself
******************/
void freeStudentItem(StudentItem* item)
{
	if (item != NULL)
	{
		//free the current student
		freeStudent(item->data);
		//the reccursivly free the next student
		freeStudentItem(item->next);
		//after freeing all students, free the structure (whitch also frees the grade)
		free(item);
	}
}

/******************
* Function Name:freeStudentList
* Input:a student list
* Output: frees it (none)
* Function Operation: using the freeStudentItem function to free the whole list
******************/
void freeStudentList(StudentList* list)
{
	//free the student list from the first item
	freeStudentItem(list->first);
}

/******************
* Function Name:freeCourse
* Input: a pointer to a course
* Output:frees it (none)
* Function Operation:first, free the name, then free the student list (by the freestdlist function)
* then free the course
******************/
void freeCourse(Course* c)
{
	if (c != NULL)
	{
		//free the "name" member
		free(c->name);
		//free the associated "studentList" member
		freeStudentList(c->calssList);
		//free the structure itself
		free(c);
	}
}

/******************
* Function Name:freeCourseItem
* Input:a pointer to a course item
* Output: free the course list from the current item (none)
* Function Operation: same as "freestudentItem", reccursion
******************/
void freeCourseItem(CourseItem* item)
{
	if (item != NULL)
	{
		freeCourse(item->data);
		freeCourseItem(item->next);
		free(item);
	}
}

/******************
* Function Name:freeSystem
* Input:a course list (the whole system)
* Output: frees all allocated memory by the system (none)
* Function Operation: free the course list from the first item
******************/
void closeSystem(CourseList* list, int exitCode)
{
	//free all courses (from the first course in the chain)
	freeCourseItem(list->first);
	exit(exitCode);
}


/******************
* Function Name: createCourseList
* Input: None
* Output: an allocated course list
* Function Operation: allcates memory for a new course list, if it fails, we exit
******************/
CourseList* createCourseList()
{
	//allocating memory
	CourseList* cl = (CourseList*)malloc(sizeof(CourseList));
	//if memory allocation fails, we exit
	if (cl == NULL)
		exit(1);
	//initilizing the first and the last chains
	cl->first = NULL;
	cl->last = NULL;
	//returning the initilized list
	return cl;
}

/******************
* Function Name: addCourseToList
* Input: a pointer to a course and a pointer to a course list
* Output: adds the course to the end of the list (void function)
* Function Operation: allocates a new course item and adds it to the end of the list
******************/
void addCourseToList(Course* course, CourseList* list)
{
	//allocating memory for the new chain
	CourseItem* newCourse = (CourseItem*)malloc(sizeof(CourseItem));
	if (newCourse == NULL)
		//if allocation fails, free all memory and exit with code 1 (indicating an error)
		closeSystem(list, 1);
	//pointing the data towards the inputted course
	newCourse->data = course;
	newCourse->next = NULL;
	//if the list is empty, we assighn the first and last chains to the new course
	if (list->first == NULL)
	{
		list->first = newCourse;
		list->last = newCourse;
		return;
	}
	//assining it to the end of the list
	list->last->next = newCourse;
	//now the new chain IS the end of the list
	list->last = newCourse;
}

/******************
* Function Name: newCourse
* Input: a pointer to a course list, an id and a name
* Output: returns the course created after adding it to the list
* Function Operation: allocates memory for the new course and the fields
* then copys the values from the input, and a new student list
******************/
Course* newCourse(CourseList* list, char id[6], char* name)
{
	//allocarting memory for the new course and the name
	Course* c = (Course*)malloc(sizeof(Course));
	c->name = (char*)malloc(strlen(name) + 1);
	//allocating a new student list (the "class")
	StudentList* classList = (StudentList*)malloc(sizeof(StudentList));
	if (c == NULL || c->name == NULL || classList == NULL)
		closeSystem(list, 1);
	//initilizing the class list, as its now empty
	classList->first = NULL;
	classList->last = NULL;
	//assigning the class list to the course
	c->calssList = classList;
	//copy values
	strcpy(c->id, id);
	strcpy(c->name, name);
	//adding course to the system
	addCourseToList(c, list);
	return c;
}

/******************
* Function Name: addStdToClass
* Input: a pointer to a student and a student list
* Output: adding the pointed student to the end of the class (void)
* Function Operation: allocating a new item, then adding it to the end of the list
* (the same as adding a new course to a list)
******************/
void addStdToClass(Student* std, StudentList* list, CourseList* cList)
{	//allocating memory for a new studentItem
	StudentItem* newStd = (StudentItem*)malloc(sizeof(StudentItem));
	//if memory allocation fails, exit
	if (newStd == NULL)
		closeSystem(cList, 1);
	//copy the data from the student
	newStd->data = std;
	//set the next studentItem to null, because we add it to the end of the list
	newStd->next = NULL;
	//if the class is empty, set the first and last studentItems to newStd
	if (list->first == NULL)
	{
		list->first = newStd;
		list->last = newStd;
		return;
	}
	//set the new studentItem 1 chain after the previous last student
	list->last->next = newStd;
	//now the last student is the new student
	list->last = newStd;
}

/******************
* Function Name: newStudent
* Input: a pointer to a student list and a name
* Output: a new student
* Function Operation: allocates memory for a new student and a name, then copys the values
******************/
Student* newStudent(StudentList* list, char* name, CourseList* cList)
{
	//allocating memory for a student
	Student* s = (Student*)malloc(sizeof(Student));
	s->name = (char*)malloc(strlen(name) + 1);
	if (s == NULL || s->name == NULL)
		closeSystem(cList, 1);
	//coping values
	strcpy(s->name, name);
	addStdToClass(s, list, cList);
	return s;
}

/******************
* Function Name: addGradeToStd
* Input: a fixed grade, a pointer to a student and a course
* Output: adding the grade inputed to the student pointed for the course pointed to (void)
* Function Operation: going through the students in the class until we reatch the wanted student
* then, we initilize the grade
******************/
void addGradeToStd(unsigned int grade, Student* std, Course* c)
{
	//initilizing a pointer to the first student in the class
	StudentItem* s = c->calssList->first;

	/*
	* as long as we have data about a student:
	* - compare each student to the inputted name
	* - when matched, set the grade
	*/
	while (s != NULL)
	{
		//when the names are equal, set the grade
		if (strcmp(s->data->name, std->name) == 0)
		{
			s->grade = grade;
		}
		//going to the next chain (student)
		s = s->next;
	}
}

/******************
* Function Name: countCourses
* Input: a course list
* Output: how many courses there are
* Function Operation: iterating through the list using a while loop
******************/
int countCourses(CourseList* list)
{
	//initilizing the counter at 0
	int count = 0;
	//initilizing the first courseItem at the start of the list
	CourseItem* c = list->first;

	/*
	* -as long as we have a course
	* -raise counter by 1
	* -go to the next couse in the chain and check if it exist
	*/
	while (c != NULL)
	{
		c = c->next;
		count++;
	}
	//returning the counter
	return count;
}

/******************
* Function Name:findCourse
* Input: an id and a course list
* Output: a pointer to the course with that id, if there isnt, return null
* Function Operation: iterating through the list with a while loop
* -when we reached a course that mathces our id, return a pointer to it
* -if we didnt find a matching course, return null
******************/
Course* findCourse(char* id, CourseList* list)
{
	//initilizing to the start of the list
	CourseItem* c = list->first;
	while (c != NULL)
	{
		//if both ids match, return a pointer
		if (strcmp(c->data->id, id) == 0)
			return c->data;
		c = c->next;
	}
	//if we didnt find a course that mathces the id inputted, return null
	return NULL;
}

/******************
* Function Name: updateCourseName
* Input: a name, an id and a course list
* Output: updates the course with that id to the new name (void)
* Function Operation: reallocating memory for the new course name, and changing it
******************/
void updateCourseName(char* name, char* id, CourseList* list)
{
	//trying to find the course that we want to change the name to
	Course* c = findCourse(id, list);
	//if c != null, it means that such course exist and we can change its name
	if (c != NULL)
	{
		//allocating memory for the new name
		c->name = (char*)realloc(c->name, strlen(name) + 1);
		//if failed, free all memory and exit with code 1
		if (c->name == NULL)
			closeSystem(list, 1);
		//copy the new name
		strcpy(c->name, name);
	}
}

/******************
* Function Name:validateName
* Input:a name and in indicator if we check a student name(1) or a course name(0)
* Output:1 if valid, 0 if not
* Function Operation:checks if the lenght is valid and if all characters are only alphabetical or numbers or " "
******************/
int validateName(char* name, int isStudent)
{
	//initilize the lenght of the name
	int len = strlen(name);

	//if isStudent == 0, it means the function was sent to check a course name
	if ((isStudent == 0) && len > COURSE_NAME_MAX_SIZE || len < COURSE_NAME_MIN_SIZE)
		//returning 0 which means the inputted name is invalid
		return 0;

	//if isStudent == 1, it means the function was sent to check a student name
	if ((isStudent == 1) && len > STD_NAME_MAX_SIZE || len < STD_NAME_MIN_SIZE)
		return 0;

	/*
	* for every character, make sure:
	* -if we check a course name - only ' ', alphabet characters and numbers are allowded
	* -when checking a student name, only ' ' and alphabet characters are allowded
	*/
	for (int i = 0; i < len; i++)
	{
		if ((isStudent == 0) && name[i] == ' ' || isalnum(name[i]) != 0)
			//if the current character is valid, movev on to the next one
			continue;
		if ((isStudent == 1) && name[i] == ' ' || isalpha(name[i]) != 0)
			continue;
		//if we didnt enter the if statement, it means the character was invalid, so we return 0
		return 0;
	}
	//returning 1 which means the name is valid as a student/course name
	return 1;
}

/******************
* Function Name:validatenum
* Input:a name and in indicator if we check an id(1) or a grade(0)
* Output:1 if valid, 0 if not
* Function Operation:checks if the lenght is valid and if all characters are only numbers
******************/
int validateNum(const char* id, int isId)
{
	//initilize the lenght of the number (id/grade)
	int len = strlen(id);
	//if its an id, the lenght has to be 5
	if (isId == 1 && len != COURSE_NUM_SIZE)
		return 0;

	/*
	- for each character, make sure its a digit
	*/
	for (int i = 0; i < len; i++)
		if (isdigit(id[i]) == 0)
			return 0;
	//return 1 = input number is valid
	return 1;
}

/******************
* Function Name: findStudent
* Input: a student name, a course id, and a course list
* Output: a pointer to a student with that name in that course
* Function Operation: compares all student names in that course with the input name
* if we find a match, return a pointer to that student
* elsewize, return null
******************/
Student* findStudent(char* name, char id[6], CourseList* list)
{
	//initilizing a pointer to that course
	Course* c = findCourse(id, list);
	//if we tried finding a student from an unexisting course, return null
	if (c == NULL)
		return NULL;
	//initilizing a studentItem at the start of the list
	StudentItem* s = c->calssList->first;

	/*
	* -loop throught the students in that class, and compare each student's name with the name inputted
	* -if the names match, return a pointer to that student
	*/
	while (s != NULL)
	{
		if (strcmp(s->data->name, name) == 0)
			return s->data;
		s = s->next;
	}
	//if couldnt find a match, return null
	return NULL;
}

/******************
* Function Name:updateGrade
* Input: a student, a course and the new grade
* Output: changes the grade (void, no return)
* Function Operation: looping through the list, and changing the wanted studentItem grade
******************/
void updateGrade(Student* std, Course* c, unsigned int newGrade)
{
	//'grade' is a studentItem member
	StudentItem* s = c->calssList->first;
	while (s != NULL)
	{
		//if names match, change grade
		if (strcmp(s->data->name, std->name) == 0)
		{
			s->grade = newGrade;
			return;
		}
		//move to the next studentItem
		s = s->next;
	}
}

/******************
* Function Name: removeLeadingSpaces
* Input: a pointer to a pointer to char
* Output: changes the array and trims all leading whitespaces
* Function Operation: calling the address of the pointer
* -as long as we have a spacebar, raise i by 1, no return value
* -move the pointer 'i' steps foward, therefore making an array without spaces in the begining
******************/
void removeLeadingSpaces(char** ch)
{
	int i;
	//raise i by 1 each time
	for (i = 0; (*ch)[i] == ' '; i++) {}
	//i = amount of spaces at the start
	*ch += i;
}

//1

/******************
* THIS IS THE "MAIN" FUNCTION FOR QUESTION 1
* Function Name:newCourseInput
* Input:gets the input and a course list
* Output: adds/updates a new course if the input is valid (no return value)
* Function Operation: using the strtok function to seperate the id from the rest of the input
* then use "srttol() function" to get the rest of the input, delete leading whitespaces, and validate it as a name
******************/
void newCourseInput(char str[], CourseList* list)
{
	//initilizing the id
	char id[6], saveStr[MAX], * name;
	//saving the input for later, because strtok() corrupts it (so we can use function "addSpaces")
	strcpy(saveStr, str);
	//token now holds the id
	char* token = strtok(str, " ");

	if (validateNum(token, 1) == 0)
	{
		//if the id is invalid, free name allocation and get out
		puts("Error: invalid class number or name.");
		return;
	}
	//if id is valid, copy the id to the id array
	strcpy(id, token);
	//name variable now holds the value of the course name
	strtol(saveStr, &name, 10);
	//remove leading spaces from the name that will occour
	removeLeadingSpaces(&name);
	//validating the course name
	if (validateName(name, 0) == 0)
	{
		puts("Error: invalid class number or name.");
		return;
	}
	//if the course already exist, we just need to update the name
	if (findCourse(id, list) != NULL)
	{
		updateCourseName(name, id, list);
		printf("Class \"%s %s\" updated.\n", id, name);
		return;
	}
	//if the system is full (5), we print error massage - we dont have space (unless you go premium)
	if (countCourses(list) == COURSE_NUM_SIZE)
	{
		puts("Unfortunately, there is no room for more classes.");
		return;
	}
	//creating the new course, adding it to the list, and saving a pointer to it
	Course* s = newCourse(list, id, name);
	printf("Class \"%s %s\" added.\n", s->id, s->name);
}

//2

/******************
* Function Name: checkInputFor2
* Input: the input string for question 2 and the course list
* Output: how many courses there are in the input string
* if 0 is returned, either there are 0 courses in the input, or one grade/name/id are invalid
* Function Operation:
******************/
int checkInputFor2(char str[], CourseList* list)
{
	//initilizing the count variable to 0
	int count = 0;
	//declaring 'token' and setting it to the name of the student using strtok()
	char* token;
	token = strtok(str, ":");
	//if the value token points to is not a proper name, return 0
	if (validateName(token, 1) == 0)
		return 0;

	/*
	* as long as token isnt null:
	* go to the next course, if the id is invalid or the course doesnt exist - return 0
	* get to the grade using strtok with " ;" - if the value isnt a valid grade, return 0
	*/
	while (token != NULL)
	{
		token = strtok(NULL, " ,");
		//if token is null it means we reached the end, so we break out of the loop
		if (token == NULL)
			break;
		if (validateNum(token, 1) == 0 || findCourse(token, list) == NULL)
			return 0;
		token = strtok(NULL, " ;");
		if (token == NULL || validateNum(token, 0) == 0 || atoi(token) > 100 || atoi(token) < 0)
			return 0;
		//raising the counter each time we got a valid set od a course id and a grade
		count++;
	}
	//returning the counter, any value besides 0 indicates a proper input
	return count;
}

/******************
* THIS IS THE "MAIN" FUNCTION FOR QUESTION 2
* Function Name: AssighnStdToCourses
* Input: a string inputted by the user and trimmed by "scanf", and a course list
* Output: assighning the student to the courses according to the input, if the input is valid
* Function Operation: using the checkInputFor2() function (no return value)
* to make sure the input is valid and saving the return value in count
* getting the name using strtok(), then looping COUNT times where each time we create a new student in the course
* with the wanted grade
******************/
void AssighnStdToCourses(char str[], CourseList* list)
{
	//saving str as strtok() will corrupt the original input
	char saveStr[MAX];
	strcpy(saveStr, str);
	//saving the return value of "checkInput" in count
	int count = checkInputFor2(str, list);
	//if 0 is returned, either there are 0 courses in the input, or one grade/name/id are invalid
	if (count == 0)
	{
		puts("Error: invalid name, class number or grade.");
		return;
	}
	unsigned int grade;
	//allocating name
	char* name = (char*)calloc(STD_NAME_MAX_SIZE, sizeof(char)), courseId[6], * token;
	if (name == NULL)
		closeSystem(list, 1);
	//getting the name using strtok(), and copying the value to "name"
	token = strtok(saveStr, ":");
	strcpy(name, token);
	//reallocating name to save memory
	name = (char*)realloc(name, strlen(name) + 1);
	if (name == NULL)
		closeSystem(list, 1);

	/*
	* for each pair of course id and a grade, do:
	* -if the student with that name already exist in that course, change thier grade
	* -if not, create a new student
	*/
	for (int i = 0; i < count; i++)
	{
		//getting the course id
		token = strtok(NULL, " ,");
		strcpy(courseId, token);
		//getting the grade
		token = strtok(NULL, " ;");
		//saving the grade
		grade = (unsigned int)atoi(token);
		//gaining access to the course using findCourse()
		Course* c = findCourse(courseId, list);
		//tring to see if the student exist in that course()
		Student* s = findStudent(name, courseId, list);
		if (s != NULL)
		{
			//changing the grade
			updateGrade(s, c, grade);
			printf("Student \"%s\" updated on class \"%s %s\" with grade %d.\n",
				s->name, c->id, c->name, grade);
			//going for the next pair as we already printed the correct output
			continue;
		}
		//creating a new student and saving the return pointer in s
		s = newStudent(c->calssList, name, list);
		//adding the grade 'grade' to the student 's' in course 'c'
		addGradeToStd(grade, s, c);
		//output
		printf("Student \"%s\" added to class \"%s %s\" with grade %d.\n",
			s->name, c->id, c->name, grade);
	}
	//freeing allocated memory
	free(name);
}

//3

/******************
* Function Name: delStudentFromCourse
* Input: a pointer to a student and a pointer to the course
* Output: removes all records of the student from that course ONLY (no return value)
* Function Operation:finding the studentItem location, disconnecting it from the chain of students by "s->next = null"
* then we free the item
******************/
void delStudentFromCourse(Student* std, Course* c)
{
	//initilizing a studentItem at the begining of the list
	StudentItem* item = c->calssList->first;
	//special case - if we need to remove the first student:
	if (strcmp(item->data->name, std->name) == 0)
	{
		c->calssList->first = item->next;
		if (c->calssList->first == NULL)
			c->calssList->last = c->calssList->first;
		item->next = NULL;
		freeStudentItem(item);
		return;
	}
	while (item != NULL)
	{
		//check if we reached the wanted student
		if (strcmp(item->next->data->name, std->name) == 0)
		{
			//saving a pointer to the student item
			StudentItem* temp = item->next;
			//disconnecting the student from the chain
			item->next = temp->next;
			temp->next = NULL;
			freeStudentItem(temp);
			return;
		}
		//moving to the next student
		item = item->next;
	}
}

/******************
* Function Name: countCoursesForStd
* Input: a student name and a course list
* Output: the amount of courses the student with that name is assighned to
* Function Operation: for each course in the list, check if you can find the student in that course
* if so-raise counter by 1
******************/
int countCoursesForStd(char* name, CourseList* list)
{
	//initilizing the count to 0 and the course item to the start of the list
	int count = 0;
	CourseItem* c = list->first;
	while (c != NULL)
	{
		//check if the student is in the course 'c->data'
		if (findStudent(name, c->data->id, list) != NULL)
			//if so, raise counter by 1
			count++;
		c = c->next;
	}
	//returning the counter
	return count;
}

/******************
* Function Name: checkFormat3
* Input: an input string and a course list
* Output: 0 if format is valid, otherwize returns the amount of courses in the input
* Function Operation: recives the name and the course id's using strtok()
* and making sure the name and all id's are valid. for each id checked, raise counter by 1
******************/
int checkFormat3(char str[], CourseList* list)
{
	//initilizing the counter
	int n = 0;
	char* token, name[STD_NAME_MAX_SIZE];
	//getting the name
	token = strtok(str, ":");
	//if name is invalid input is invalid so we return 0
	if (validateName(token, 1) == 0)
		return 0;
	strcpy(name, token);
	while (token != NULL)
	{
		//get the course id
		token = strtok(NULL, " ,");
		//if token is null, it means we reached the end so we break
		if (token == NULL)
			break;
		//if the id isnt valid or the course with that id does not exist return 0
		if (validateNum(token, 1) == 0 || findCourse(token, list) == NULL)
			return 0;
		//raising the counter
		n++;
	}
	//returning the counter
	return n;
}

/******************
* Function Name: ifStudentIsInAllClasses
* Input: the input string, the course list and the amount of courses in the input
* Output: if the input contains a course that the student doesnt belong to, return its pointer
* otherwize, return null
* Function Operation: get the ids using strtok(), for each id-check if the student exist in that course
******************/
Course* ifStudentIsInAllClasses(char str[], CourseList* list, int count)
{
	char* token, name[STD_NAME_MAX_SIZE];
	//getting the name
	token = strtok(str, ":");
	strcpy(name, token);
	for (int i = 0; i < count; i++)
	{
		//getting the course id
		token = strtok(NULL, " ,");
		//if there is not student "name" in course "id", return the pointer to that course
		if (findStudent(name, token, list) == NULL)
		{
			Course* c = findCourse(token, list);
			return c;
		}
	}
	//othewize, return null
	return NULL;
}

/******************
* THIS IS THE "MAIN" FUNCTION FOR QUESTION 3
* Function Name: removeStudent
* Input: an input string and the course list
* Output: removes the student from the courses provided, if possible (no return value)
* Function Operation: validate the input using the functions above
* then, get the name ids using strtok() and remove the studentItem from these courses
******************/
void removeStudent(char str[], CourseList* list)
{
	//the assisted functions will corrupt the input string, so we need to save it
	char saveStr[MAX], saveStr2[MAX], * token;
	strcpy(saveStr, str);
	strcpy(saveStr2, str);
	//the amount of course ids (0 == invalid input/0 courses)
	int count = checkFormat3(str, list);
	if (count == 0)
	{
		puts("Error: invalid name or class number.");
		return;
	}
	//allocating memory for std name
	char* name = (char*)calloc(STD_NAME_MAX_SIZE, sizeof(char));
	if (name == NULL)
		closeSystem(list, 1);
	strcpy(name, str);
	//reallocating
	name = (char*)realloc(name, strlen(name) + 1);
	if (name == NULL)
		closeSystem(list, 1);

	/*
	* if the amount of courses the student is assighned to is
	* equal to the amount of courses we want to delete him from
	* we print an error massage because we cant delete a student from all courses
	*/
	if (countCoursesForStd(name, list) == count)
	{
		puts("Error: student cannot be removed from all classes.");
		free(name);
		return;
	}
	//trying too see if one of the ids are a course the student does not belong to in the first place
	Course* c = ifStudentIsInAllClasses(saveStr, list, count);
	if (c != NULL)
	{
		printf("Error: student is not a member of class \"%s %s\".\n", c->id, c->name);
		return;
	}
	token = strtok(saveStr2, ":");
	for (int i = 0; i < count; i++)
	{
		//getting the id
		token = strtok(NULL, " ,");
		//getting a pointer to the student and the course we want to delete him from
		Student* s = findStudent(name, token, list);
		Course* c = findCourse(token, list);
		printf("Student \"%s\" removed from class \"%s %s\".\n", s->name, c->id, c->name);
		delStudentFromCourse(s, c);
	}
	free(name);
}

//4

/******************
* Function Name: sizeClass
* Input: a course pointer
* Output: the amount of students assighned to that course
* Function Operation: initilize a counter and raise it using a while loop
******************/
double sizeClass(StudentList* list)
{
	double counter = 0;
	StudentItem* s = list->first;

	/*
	* as long as c!= null (the courseItem exist):
	* raise counter
	* move to the next course
	*/
	while (s != NULL)
	{
		counter++;
		s = s->next;
	}
	//returning the counter
	return counter;
}

/******************
* Function Name: swapStudentItems
* Input: 2 pointers to 2 student items and a course list in case allocation fails
* Output: swap the data between these 2 students (no return value)
* Function Operation:
******************/
void swapStudentItems(StudentItem* s1, StudentItem* s2, CourseList* list)
{
	//initilize len1 and len2 as the lenght of the names
	int len1 = strlen(s1->data->name), len2 = strlen(s2->data->name);
	//reallocating memory for the new names
	s1->data->name = (char*)realloc(s1->data->name, len2 + 1);
	s2->data->name = (char*)realloc(s2->data->name, len1 + 1);
	if (s1 == NULL || s2 == NULL)
		closeSystem(list, 1);
	//temp = s1->grade - temp variables so we dont lose the data
	unsigned int tempGrade = s1->grade;
	char tempName[STD_NAME_MAX_SIZE];
	strcpy(tempName, s1->data->name);
	//changing s1 to s2
	s1->grade = s2->grade;
	strcpy(s1->data->name, s2->data->name);
	//chainging s2 to temp that held data for s1
	s2->grade = tempGrade;
	strcpy(s2->data->name, tempName);
}

/******************
* Function Name: sort
* Input: a student list and a course list pointers
* Output: sorts the student list by their ascii values (no return value)
* Function Operation: using bubble sort (swaping neibours until we dont need to)
******************/
void sort(StudentList* list, CourseList* listt)
{
	int switched = 1;
	StudentItem* item;
	//as long as we made a swap, we continue with the bubble sort
	while (switched == 1)
	{
		switched = 0;
		item = list->first;
		while (item->next != NULL)
		{
			if (strcmp(item->data->name, item->next->data->name) > 0)
			{
				switched = 1;
				swapStudentItems(item, item->next, listt);
			}
			//moving to the next chain
			item = item->next;
		}
	}
}

/******************
*THIS IS THE "MAIN" FUNCTION FOR QUESTION 4
* Function Name: printCourse
* Input: an input string and a course list
* Output: prints the students in that course after sorting it by ascii values (no return value)
* Function Operation: if the course inputted is bad -> print an error massage
* if there are no students -> print an error massage
* othewize, sort the student list of that course with the sort function above and print the students
* names with thier grades
******************/
void printCourse(char str[], CourseList* list)
{
	//if the input is an invalid id or an id that doesnt exist in the system, print an error massage
	if (validateNum(str, 1) == 0 || findCourse(str, list) == NULL)
	{
		puts("Error: invalid class number.");
		return;
	}
	//getting a pointer to the course
	Course* c = findCourse(str, list);
	int studentsInClass = (int)sizeClass(c->calssList);
	//if there are no students in that course, print an error massage
	if (studentsInClass == 0)
	{
		printf("Class \"%s %s\" has no students.\n", c->id, c->name);
		return;
	}
	//otherwize, sort the course's classList using the sort() function
	sort(c->calssList, list);
	//getting a pointer to the first studentItem
	StudentItem* s = c->calssList->first;
	printf("Class \"%s %s\" students:\n", c->id, c->name);
	while (s != NULL)
	{
		//printing the students with thier grades
		printf("%s, %d\n", s->data->name, s->grade);
		//moving to the next student
		s = s->next;
	}
}

//5

/******************
* Function Name: avg
* Input: a student list pointer
* Output: the average grade
* Function Operation: access each of the studentItems by initilizng a studentItem pointer
* at the begining of the list and using a while loop. sum the grades and devide it by the count
******************/
double avg(StudentList* list)
{
	//initilizing count and sum
	int count = 0;
	double sum = 0;
	StudentItem* s = list->first;
	while (s != NULL)
	{
		sum += s->grade;
		count++;
		s = s->next;
	}
	//returning sum/count = average
	return sum / count;
}

/******************
* Function Name: max
* Input: a student list pointer
* Output: the highest grade
* Function Operation: access each of the studentItems by initilizng a studentItem pointer
* at the begining of the list and using a while loop, and look for the highest grade
******************/
double max(StudentList* list)
{
	//initilizing the current maximun at the first grade
	double max = list->first->grade;
	StudentItem* s = list->first;
	while (s != NULL)
	{
		//if we encounter a higher grade, save it to max
		if (max < s->grade)
			max = s->grade;
		s = s->next;
	}
	//return max
	return max;
}

/******************
* Function Name: min
* Input: a student list pointer
* Output: the lowest grade
* Function Operation: access each of the studentItems by initilizng a studentItem pointer
* at the begining of the list and using a while loop, and look for the highest grade
******************/
double min(StudentList* list)
{
	//initilizing the current minimum
	double min = list->first->grade;
	StudentItem* s = list->first;
	while (s != NULL)
	{
		if (min < s->grade)
			min = s->grade;
		s = s->next;
	}
	return min;
	//please no//
}

/******************
* Function Name: sum
* Input: a student list pointer
* Output: the sum of all grades
* Function Operation: access each of the studentItems by initilizng a studentItem pointer
* at the begining of the list and using a while loop. sum the grades at each run
******************/
double sum(StudentList* list)
{
	//initilizing sum
	int sum = 0;
	StudentItem* s = list->first;
	while (s != NULL)
	{
		sum += s->grade;
		s = s->next;
	}
	return sum;
}

//count is done by sizeClass (line 873)

/******************
* Function Name: calcAndPrint
* Input: a course list and a pointer to a function
* Output: calculate grades, according tov the function (no return value)
* Function Operation: if the class is empty -> print an error massage
* if its not -> print each course with its calculated grade
******************/
typedef double(*calculatedGrade)(StudentList*);
void calcAndPrint(CourseList* list, calculatedGrade func)
{
	//initilizing the courseItem to the start of the list
	CourseItem* c = list->first;
	//for each course, do:
	while (c != NULL)
	{
		//if no students are assighned to that course, print an error massage
		if (sizeClass(c->data->calssList) == 0)
			printf("Class \"%s %s\" has no students.\n", c->data->id, c->data->name);
		//otherwize, calculate the final class grade according to the function
		else
		{
			int result = (int)func(c->data->calssList);
			printf("%s %s, %d\n", c->data->id, c->data->name, result);
		}
		//move on to the next chain (course)
		c = c->next;
	}
}

/******************
* THIS IS THE "MAIN" FUNCTION FOR QUESTION 5
* Function Name: printCoursesWithScores
* Input: a course list, the function also asks for an operation
* Output: does the operation selected for each course in the course list
* and prints the calculated grade (no return value)
* Function Operation: if there are no courses -> print an error massage
* otherwize -> ask for the aggregation method, and apply it on each course
* using a switch case and the functions above, if possible (course can not be empty)
******************/
void printCoursesWithScores(CourseList* list)
{
	//if the system is empty -> error massage
	if (countCourses(list) == 0)
	{
		puts("Error: there are no classes.");
		return;
	}
	//asking the user for the operation
	char choice;
	puts("Please select the aggregation method:");
	puts("\ta. Average Grade.");
	puts("\tb. Maximal Grade.");
	puts("\tc. Minimal Grade.");
	puts("\td. Sum.");
	puts("\te. Count.");
	puts("\t0. Return to the main menu.");
	scanf("%c", &choice);
	switch (choice)
	{
		//case 0 -> return to menu
	case '0':
	{
		return;
	}
	//case a -> do avg
	case 'a':
	{
		calcAndPrint(list, avg);
		return;
	}
	case 'b':
	{
		calcAndPrint(list, max);
		return;
	}
	case 'c':
	{
		calcAndPrint(list, min);
		return;
	}
	case 'd':
	{
		calcAndPrint(list, sum);
		return;
	}
	case 'e':
	{
		//sizeClass is at line 873
		calcAndPrint(list, sizeClass);
		return;
	}
	}
}

/******************
* Function Name: menu
* Input: int type
* Output: prints a menu to the user (no return value)
* Function Operation:
* type == 1 -> print short menu
* type == 0 -> print full menu
******************/
void menu(int type)
{
	//MENU TYPE 1 - SHORT MENU
	if (type == 1)
	{
		puts("Select the next operation (insert 6 for the entire menu):");
		return;
	}
	//MENU TYPE 0 - FULL MENU
	puts("Please select an operation:");
	puts("\t0. Exit.");
	puts("\t1. Insert or update a class.");
	puts("\t2. Insert or update a student.");
	puts("\t3. Remove a student from classes.");
	puts("\t4. Print a class.");
	puts("\t5. Print all classes with their computed scores.");
	puts("\t6. Print the menu.");
}

/******************
* Function Name: getChoice
* Input: none, it asks for input
* Output: invalid -> -1, valid -> 0-6
* Function Operation: scanf the input from the user
* if the user entered values from '0'-'6' -> return 0-6
* otherwize, return -1
******************/
int getChoice()
{
	char choice;
	//scanf with a space so it ignores all whitespaces before the input
	scanf(" %c", &choice);
	//if the entered choice is valid, return choice - '0' (the coresponding number)
	if (choice >= '0' && choice <= '6')
	{
		return choice - '0';
	}
	return -1;
}

/******************
* Function Name: choiceToAction
* Input: a course list
* Output: gets us to the wanted operation (no return value)
* Function Operation: imagine this as the 'information' stand at the university:
* -	first, it understands what operation you want by the assist of the "getChoice()" function
* -	then, using a switch case, it dirercts you to the operation you want to do
******************/
void choiceToAction(CourseList* list)
{
	//gets the choice
	int choice = getChoice();
	printf("choice = %d\n", choice); //debugging remove
	//initilizing the input
	char str[MAX] = { '\0' };
	switch (choice)
	{
	case -1:
	{
		//if getChoice() returned -1, the input is invalid
		puts("Error: unrecognized operation.");
		//printing the short version of the menu
		menu(1);
		//recalling the function for the next operation
		choiceToAction(list);
	}
	case 0:
	{
		//0 is "exit", so we free all allocated memory and close the program with code 0
		closeSystem(list, 0);
	}
	case 1:
	{
		//cleaning the buffer
		scanf("%c");
		//getting the input string
		scanf(" %[^\n]s", str);
		//calling the desired function
		newCourseInput(str, list);
		//recalling the menu
		menu(1);
		choiceToAction(list);
	}
	case 2:
	{
		scanf("%c");
		scanf(" %[^\n]s", str);
		AssighnStdToCourses(str, list);
		menu(1);
		choiceToAction(list);
	}
	case 3:
	{
		scanf("%c");
		scanf(" %[^\n]s", str);
		removeStudent(str, list);
		menu(1);
		choiceToAction(list);
	}
	case 4:
	{
		scanf("%c");
		scanf(" %[^\n]s", str);
		printCourse(str, list);
		menu(1);
		choiceToAction(list);
	}
	case 5:
	{
		scanf("%c");
		printCoursesWithScores(list);
		menu(1);
		choiceToAction(list);
	}
	case 6:
	{
		//case 6 -> print the full version of the menu
		menu(0);
		choiceToAction(list);
	}
	}
}
//end
