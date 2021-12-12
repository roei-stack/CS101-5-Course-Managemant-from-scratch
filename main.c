#include "ex5.h"

/******************
* Function Name: main
* Input: none
* Output: the whole godamm program
* Function Operation: gives instruction to how to start the program
* First -> create the course list, using the createCourseList function
* Secondly -> print the full version of the menu for the first time
* Thirdly -> call the choiceToAction() function which will take care of the rest
******************/
int main()
{
	CourseList* courses = createCourseList();
	menu(0);
	choiceToAction(courses);
	return 0;
}
//end
