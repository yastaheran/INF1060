#include <stdbool.h>
#include <stdio.h>

struct person {
	char name;
	int age;
	char haircolor;
	char eyecolor;
	int height;	
};

//Set-functions
void set_name(struct person* p, char n) {
	if(strlen(n) <= 30) {
		p->name = n;
	} else {
		printf("name can't be longer than 30 characters long.");
	}
}

void set_age(struct person* p, int a) {
	if(age <= 200) {
		p->age = a;
	} else {
		printf("age can't be over 200 years.");
	}
}

void set_haircolor(struct person* p, char h) {
	p->haircolor = h;
}

void set_eyecolor(struct person* p, char e) {
	p->eyecolor = e;
}

void set_height(struct person* p, int h) {
	p->height = h;
}

//Get-fucntions
char get_name(struct person* p) {
	return p->name;
}

int get_age(struct person* p) {
	return p->age;
}

void get_haircolor(struct person* p) {
	return p->haircolor;
}

void get_eyecolor(struct person* p) {
	return p->eyecolor;
}

void get_height(struct person* p) {
	return p->height;
}

//Check-functions
int check_name(struct person* p) {
	if(p->name != NULL)	return 0;
	return -1;
}

int check_age(struct person* p) {
	if(p->age != 0) return 0;
	return -1;
}

char toString(struct person* p) {
	char str[80];
	sprintf(str, "Name: %s \n Age: %s \n Haircolor: %s \n Eyecolor: %s \n Height: %s \n", p->name, p->age, p->haircolor, p->eyecolor, p->height);
	puts(str);
	return str;
}

struct person lagstruct(char string) {
	struct person nyPerson;
	char info[100];
	strcpy(inf, string);
	sscanf(inf, "%s %d %s %s %d", p->name, p->age, p->haircolor, p->eyecolor, p->height)
	
	return nyPerson;
}