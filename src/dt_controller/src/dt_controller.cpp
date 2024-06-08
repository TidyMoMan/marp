#include <cstdio>

void timerCallback();

int main(int argc, char ** argv)
{
  (void) argc;
  (void) argv;

  printf("hello world dt_controller package\n");


  return 0;
}

void timerCallback(){
  printf("called back! \n");
}
