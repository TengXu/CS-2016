#define BUFSIZE 256
volatile char output[BUFSIZE];
volatile int bytes;
volatile int func2bytes;

void cpystr(char *str) __attribute__ ((noinline));
int func2 (char *) __attribute__ ((noinline));
void func1 (void) __attribute__ ((noinline));

void
cpystr(char *str)
{
  while (*str) {
    output[bytes]=*str;
    str++;
    bytes++;
  }
}

int
func2(char *s)
{
  int i = bytes;
  cpystr(s);
  return bytes-i;
}

void
func1()
{
  cpystr("func1 called\n");
  func2bytes = func2("func2 called\n");
}


int
main(int argc, char **argv)
{

  bytes=0;
  func2bytes=0;
  func1();

  return 0;
}
