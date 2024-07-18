#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void* compute(void* argument) {
  long value,v1,v2;
  int res;
  pthread_t t1,t2;

  value = (long) argument;
  printf("Starting Thread for %li\n",value);
  if (value>1) {
    res=pthread_create(&t1, NULL, compute, (void*)(value-1) );
    assert(!res);
    res=pthread_create(&t2, NULL, compute, (void*)(value-2) );
    assert(!res);
    res=pthread_join(t1, (void**) &v1);
    assert(!res);
    res=pthread_join(t2, (void**) &v2);
    assert(!res);
    value=v1+v2;
  }

  return (void*) value;
}

int main(int argc, char** argv) {
  if(argc != 2) {
    printf("Please specify a number to calculate in the first argument\n");
    exit(-1);
  }
  long value,result;
  value=atoi(argv[1]);
  result=(long) compute((void*)value);
  printf("Result is %li\n",result);
  exit(0);
}
