
//benjamin stacey || G00402833
#include<stdio.h>
//colour functions
void black();void red();void green();void yellow();void blue();void purple();void cyan();void white();
void reset();void bold();void underline();void blink();void reverse();void concealed();
void blackB();void redB();void greenB();void yellowB();void blueB();void purpleB();void cyanB();void whiteB();
void clear();

//colour functions
void black()
{
	printf("\033[0;30m");
}
void red()
{
	printf("\033[0;31m");
}
void green()
{
	printf("\033[0;32m");
}
void yellow()
{
	printf("\033[0;33m");
}
void blue()
{
	printf("\033[0;34m");
}
void purple()
{
	printf("\033[0;35m");
}
void cyan()
{
	printf("\033[0;36m");
}
void white()
{
	printf("\033[0;37m");
}

//BOLD functions
void blackB()
{
	printf("\033[1;30m");
}
void redB()
{
	printf("\033[1;31m");
}
void greenB()
{
	printf("\033[1;32m");
}
void yellowB()
{
	printf("\033[1;33m");
}
void blueB()
{
	printf("\033[1;34m");
}
void purpleB()
{
	printf("\033[1;35m");
}
void cyanB()
{
	printf("\033[1;36m");
}
void whiteB()
{
	printf("\033[1;37m");
}

//reset function
void reset()
{
	printf("\033[0m");
}

//bold function
void bold()
{
	printf("\033[1m");
}

//underline function
void underline()
{
	printf("\033[4m");
}

//blink function
void blink()
{
	printf("\033[5m");
}

//reverse function
void reverse()
{
	printf("\033[7m");
}

//concealed function
void concealed()
{
	printf("\033[8m");
}

//clear screen function
void clear()
{
	printf("\033[2J");
}

