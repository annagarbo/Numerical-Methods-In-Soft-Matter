/* compile to create an executable "EX1",
   for example, with
     cc example-C-01.c -o EX1

   This example is introductory to C programming language.
   Specifically, with this example you will learn:
   - "include"
   - "define"
   - global variables
   - "void" functions
   - some input/output basics
   - indentation
   - variable types
   - "if"
   
   */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define C1 10  // constant of type int 
#define C2 10. // constant of type double 

/* global variables, accessible to all functions in this module,
   unless they contain a variable with the same name */

/* integer */
int A=3;
/* float, double precision */
double Q;
/* array, double precision, with C1 elements */
double V[C1];
/* character */
char c='w';
/* string */
char s[1000]="this is a string";


void Print_int(int i, int j){
  if(i==j)  fprintf(stdout,"(%d) - (%4d) - (%04d)\n",i,i,i);
  // logic operations with integers: 0=False, otherwise True
  if(i)     fprintf(stdout,"i=%d means True\n",i); 
}


void Print_double_e(double x, double X){
  if(x==X){
    fprintf(stdout,"printing double with \%e or \%g\n");
    fprintf(stdout,"[%e] - [%.4e] - [%10.4e]\n",x,x,x);
    fprintf(stdout,"[%g] - [%.4g] - [%10.4g]\n",x,x,x);
  }
  return;
}

void Print_double(double x, double X){
  if(x==X) fprintf(stdout,"[%lf] - [%.4lf] - [%10.4lf]\n",x,x,x);
  return;
}



/* if one keeps the main as the last "function",
   it sees the functions above, which thus do
   not need to be redefined
*/
int main(int argn, char **argc){
  int i,j;
  double x,y,v[10];
printf("indentation does not matter but it is better to use it\n");
 printf("indentation does not matter but it is better to use it\n");
  printf("indentation does not matter but it is better to use it\n");
   printf("indentation does not matter but it is better to use it\n");
   // fprintf(stdout,...) is equiv to printf(...)
   // but is more flexible: e.g., replacing stdout with stderr, or with a file
    printf("again:\n");
fprintf(stdout,"indentation does not matter but it is better to use it\n");
 fprintf(stdout,"indentation does not matter but it is better to use it\n");
  fprintf(stdout,"indentation does not matter but it is better to use it\n");
   fprintf(stdout,"indentation does not matter but it is better to use it\n");

   i=10;
   Print_int(i,C1);

   fprintf(stdout,"\nplease input an integer 'i'\n");
   /*  &i is the memory address of "i" */
   fscanf(stdin,"%d",&i);
   Print_int(i,C1);

   x=10.;
   Print_double(x,C1);
   Print_double(C1,C2);
   Print_double(C2,C1);

   Print_double_e(x,x);
   x = 10.2;
   Print_double_e(x,x);
   
   fprintf(stdout,"\nplease input a double 'x'\n");
   fscanf(stdin,"%lf",&x);
   Print_double(x,C2);
   fprintf(stdout,"\nthe end\n");

   return 1;
}
