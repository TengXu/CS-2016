#include <stdio.h>

void
chartest(signed char *x, signed char *y)
{
  if (*x < *y) 
    printf("c: %hhd < %hhd ", *x, *y);
  else
    printf("c: %hhd > %hhd ", *x, *y);
}

void
uchartest(unsigned char *x, unsigned char *y)
{
  if (*x < *y) 
    printf("uc: %hhu < %hhu ", *x, *y);
  else
    printf("uc: %hhu > %hhu ", *x, *y);
}

void
shorttest(short *x, short *y)
{
  if (*x < *y) 
    printf("s: %hd < %hd ", *x, *y);
  else
    printf("s: %hd > %hd ", *x, *y);  
}

void
ushorttest(unsigned short *x, unsigned short *y)
{
  if (*x < *y) 
    printf("us: %hu < %hu ", *x, *y);
  else
    printf("us: %hu > %hu ", *x, *y);  
}

void
inttest(int *x, int *y)
{
  if (*x < *y) 
    printf("i: %d < %d ", *x, *y);
  else
    printf("i: %d > %d ", *x, *y);  
}

void
uinttest(unsigned int *x, unsigned int *y)
{
  if (*x < *y) 
    printf("ui: %u < %u ", *x, *y);
  else
    printf("ui: %u > %u ", *x, *y);  
}

void
lltest(long long int *x, long long int *y)
{
  if (*x < *y) 
    printf("ll: %lld < %lld ", *x, *y);
  else
    printf("ll: %lld > %lld ", *x, *y);  
}

void
ulltest(unsigned long long int *x, unsigned long long int *y)
{
  if (*x < *y) 
    printf("ull: %llu < %llu ", *x, *y);
  else
    printf("ull: %llu > %llu ", *x, *y);  
}

int
main(int argc, char **argv)
{
  //  printf("Testing number ");
  {
    signed char a=-1;
    signed char b=1;
    chartest(&a, &b);
  }
  {
    unsigned char a=-1;
    unsigned char b=1;
    uchartest(&a, &b);
  }
  {
    short a=-1;
    short b=1;
    shorttest(&a, &b);
  }
  {
    unsigned short a=-1;
    unsigned short b=1;
    ushorttest(&a, &b);
  }
  {
    int a=-1;
    int b=1;
    inttest(&a, &b);
  }
  {
    unsigned int a=-1;
    unsigned int b=1;
    uinttest(&a, &b);
  }
  {
    long long int a=-1;
    long long int b=1;
    lltest(&a, &b);
  }
  {
    unsigned long long int a=-1;
    unsigned long long int b=1;
    ulltest(&a, &b);
  }
  {
    signed char c=0xff;
    printf("c=%hhd (0x%02hhx) ", c, c);
  }
  {
    signed short s=0xffff;
    printf("s=%hd (0x%04hx) ",s,s);
  }
  {
    signed int i=0xffffffff;
    printf("i=%d (0x%08x) ",i,i);
  }
  {
    signed long l=-1;
    printf("l=%ld (0x%08lx) ",l,l);
  }
  {
    signed long long ll=0xffffffffffffffffUL;
    printf("ll=%lld (0x%016llx)\n",ll,ll);
  }

  return 0;
}
