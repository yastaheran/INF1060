#include <stdio.h>
#include <stdlib.h>

//Oppgave 3a
struct datetime {
	//Date
	int year;
	int month;
	int date;
	
	//Time
	int hour;
	int min;
	int sec;
	
};

//Oppgave 3d
struct timerange {
	struct datetime start;
	struct datetime slutt;
	int id;
};

//Oppgave 3b
void init_datetime(struct datetime* dt) {
	//initiating new datetime struct
	dt->year = 0;
	dt->month = 0;
	dt->date = 0;
	
	dt->hour = 0;
	dt->min = 0;
	dt->sec = 0;
}

//Oppgave 3c
void datetime_set_date(struct datetime* dt, int y, int m, int d) {
	//set date
	dt->year = y;
	dt->month = m;
	dt->date = d;

}

void datetime_set_time(struct datetime* dt, int h, int m, int s) {
	//set time
	dt->hour = h;
	dt->min = m;
	dt->sec = s;
}

//Oppgave 3e
int main() {
	
	struct datetime dt1;
	init_datetime(&dt1);
	printf("initiated datetime 1: \n \t year: %d month: %d date: %d with hour: %d min: %d sec: %d\n", dt1.year,dt1.month,dt1.date,dt1.hour,dt1.min,dt1.sec);
	
	datetime_set_date(&dt1, 1995, 6, 24);
	printf("\t datetime date set to year: %d month: %d date: %d\n", dt1.year,dt1.month,dt1.date);
	
	datetime_set_time(&dt1, 14, 06, 5);
	printf("\t datetime time set to hour: %d min: %d sec: %d\n", dt1.hour,dt1.min,dt1.sec);
	
	struct datetime dt2;
	init_datetime(&dt2);
	printf("initiated datetime 2: \n \t year: %d month: %d date: %d with hour: %d min: %d sec: %d\n", dt2.year,dt2.month,dt2.date,dt2.hour,dt2.min,dt2.sec);
	
	datetime_set_date(&dt2, 2000, 5, 23);
	printf("\t datetime date set to year: %d month: %d date: %d\n", dt1.year,dt1.month,dt1.date);
	
	datetime_set_time(&dt2, 3, 16, 59);
	printf("\t datetime time set to hour: %d min: %d sec: %d\n", dt1.hour,dt1.min,dt1.sec);
	
	struct datetime dt3;
	init_datetime(&dt3);
	printf("initiated datetime 3: \n \t year: %d month: %d date: %d with hour: %d min: %d sec: %d\n", dt3.year,dt3.month,dt3.date,dt3.hour,dt3.min,dt3.sec);
	
	datetime_set_date(&dt3, 1984, 2, 13);
	printf("\t datetime date set to year: %d month: %d date: %d\n", dt1.year,dt1.month,dt1.date);
	
	struct timerange tr;
	tr.start = dt3;
	tr.slutt = dt1;
	tr.id = 1;
	printf("timerange id: %d \n \t start year: %d end year: %d \n", tr.id, tr.start.year, tr.slutt.year);
	
	
}