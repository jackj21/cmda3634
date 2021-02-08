#include <stdio.h>
#include <math.h>

typedef struct Timestamp {
	int days;
	int hours;
	int minutes;
	int seconds;
} Date;

/**
 * Prints the contents of the timestamp
 */
void print_timestamp (Date a) {
	printf("Days: %d\n", a.days);
	printf("Hours: %d\n", a.hours);
	printf("Minutes: %d\n", a.minutes);
	printf("Seconds: %d\n", a.seconds);


}

Date timestamp_diff (Date a, Date b) {
	Date diff;

	diff.days = b.days - a.days;
	diff.hours = b.hours - a.hours;
	diff.minutes = b.minutes - a.minutes;
	diff.seconds = b.seconds - a. seconds;

	return diff;
}

int main() {
	Date jan;	
	jan.days = 18666;
	jan.hours = jan.days * 24;
	jan.minutes = jan.hours * 60;
	jan.seconds = jan.minutes * 60;
	
	print_timestamp(jan); 	


	return 0;
}
