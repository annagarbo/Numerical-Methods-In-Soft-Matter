/* compile to create an executable "EX2",
   for example, with
     cc example-C-01.c -o EX2

   With this example you will learn:
   - while, do-while
   - functions
   - pointers
   - AND, OR, NOT
*/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define C2 10. // constant of type double 



void Print_int(int i, int j){
  if(i==j)  printf("(%d) - (%4d) - (%04d)\n",i,i,i);
  // logic operations with integers: 0=False, otherwise True
  if(i)     printf("i=%d means True\n",i); 
}


double SupSum(double x, double y){
  x = x+y;
  // fabs: absolute value for floats
  if(fabs(x)<C2) return x;
  else           return 10;
}



/* if one keeps the main as the last "function",
   it sees the functions above, which thus do
   not need to be "redefined" above it
*/
int main(int argn, char **argc){
  int i,j;
  double x=0,y=10.;

  printf("(1)----------\n");
  while(x<y){
    printf("x=%lf\n",x);
    x++;
  }

  printf("(2)----------\n");
  x=0.;
  while(x<=y){
    printf("x=%g\n",x);
    x+=2.;
  }
  printf("(2b)----------\n");
  x=12.;
  while(x<=y){
    printf("x=%g\n",x);
    x+=2.;
  }

  printf("(3)----------\n");
  x=0.;
  do{
    printf("x=%g\n",x);
    x+=2.;
  }while(x<=y);
  
  printf("(3b)----------\n");
  x=12.;
  do{
    printf("x=%g\n",x);
    x+=2.;
  }while(x<=y);
  
  printf("\nend part 1\n\n");

  

  printf("(4)----------  AND --> &&\n");
  x=0.;
  i=0;
  /* AND --> && */
  while(x<20 && i<10){
    printf("x=%3g  i=%2d\n",x,i);
    x = SupSum(x,2);
    i++;
  }

  
  printf("(5)----------  OR--> ||\n");
  x=0.;
  i=0;
  /* OR--> || */
  while(x<8 || i<10){
    printf("x=%3g  i=%2d\n",x,i);
    x = SupSum(x,2);
    i++;
  }
  
  printf("(6)----------  NOT --> !\n");
  x=0.;
  i=0;
  /* NOT --> ! */
  while(!i){
    printf("x=%3g  i=%2d\n",x,i);
    x = SupSum(x,2);
    i++;
  }
  
  printf("\nend part 2\n");

  
  printf("(7)----------  passing values of variables:\n      functions cannot change them outside\n");
  // e.g. x = x+y in SumSup does not change x outside the function
  x=1.;
  y = SupSum(x,2);
  printf("x=%.1lf, y=%.1lf\n",x,y);
}
