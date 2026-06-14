#pragma once

#include"student.h"

int loadCSV(char* filename, Student** head);
int saveCSV(char* filename, Student* head);
