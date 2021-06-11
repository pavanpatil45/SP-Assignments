#include<stdio.h>  
#include<string.h>  
char inp[30];  
int ptr=0;  
int A()

{
  if (inp[ptr] == 'c') {
    ptr++;
    if (inp[ptr] == 'd') {
      ptr++;
    } else return 0;
  }
  return 1;
}
int S() {
  int s;
  if (inp[ptr] == 'a') {
    ptr++;
    s = A();
    if (A() != 0) {
      if (inp[ptr] == 'b') {
        ptr++;
        if (inp[ptr] == '$') {
          return 1;
        } else return 0;
      } else return 0;
    }
  } else return 0;
}
int main() {
  int length;
  printf("\n S:aAb \n");
  printf("\n A:cd \n");
  printf("\n A:c \n");
  printf("\n Enter the string : ");
  scanf("%s", inp);
  length = strlen(inp);
  inp[length] = '$';
  if (S() != 0) {
    printf("\n Successful completion of parsing");
  } else {
    printf("\n Unsuccessful parsing");
  }
} //end main 
