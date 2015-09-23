#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#define nthreads 4

FILE *infile;
pthread_mutex_t joy=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t joy1=PTHREAD_MUTEX_INITIALIZER;
void *Threads();

/* Dictionary struct holds a single word and a count, it points to the next word in the list. */
typedef struct dict {
  char *word;
  int count;
  struct dict *next;
} dict_t;

dict_t *d;

/* Allocate a word */
char *
make_word( char *word ) {
  return strcpy( malloc( strlen( word )+1 ), word );
}

/* Construct a dictionary with an input word. */
dict_t *
make_dict(char *word) {
  dict_t *nd = (dict_t *) malloc( sizeof(dict_t) );
  nd->word = make_word( word );
  nd->count = 1;
  nd->next = NULL;
  return nd;
}

void
insert_word(char *word ) {
  
  //   Insert word into dict or increment count if already there
  //   return pointer to the updated dict
  
  dict_t *nd;
  dict_t *pd = NULL;		// prior to insertion point 
  dict_t *di = d;		// following insertion point
  // Search down list to find if present or point of insertion
  while(di && ( strcmp(word, di->word ) >= 0) ) { 
    if( strcmp( word, di->word ) == 0 ) { 
      di->count++;		// increment count 
      return ;			// return head 
    }
    pd = di;			// advance ptr pair
    di = di->next;
  }
  nd = make_dict(word);		// not found, make entry 
  nd->next = di;		// entry bigger than word or tail 
  if (pd) {
    pd->next = nd;
    return ;			// insert beond head 
  }
	d=nd;
  return ;
}

/* iterate through all dicts and print the list. */
void print_dict() {
  while (d) {
    printf("[%d] %s\n", d->count, d->word);
    d = d->next;
  }
}

/* Gets another word. note fgetc remembers position, so after we exit this function the next call will continue from where it left off. */
int
get_word( char *buf,int n, FILE *infile) {
  int inword = 0;
  int c;  
  while( (c = fgetc(infile)) != EOF ) {
    if (inword && !isalpha(c)) {
      buf[inword] = '\0';	// terminate the word string
      return 1;
    } 
    if (isalpha(c)) {
      buf[inword++] = c;
    }
  }
  return 0;			// no more words
}



#define MAXWORD 1024
/*dict_t *
words( FILE *infile ) {
  dict_t *wd = NULL;
  char wordbuf[MAXWORD];
  while( get_word( wordbuf, MAXWORD, infile ) ) {
    wd = insert_word(wd, wordbuf); // add to dict
  }
  return wd;
}*/

int
main( int argc, char *argv[] ) {
  d = NULL;
  infile = stdin;
  pthread_t thr[nthreads];
  
  int i;
  int r;
  if (argc >= 2) {
    infile = fopen (argv[1],"r");
  }
  if( !infile ) {
    printf("Unable to open %s\n",argv[1]);
    exit( EXIT_FAILURE );
  }
  

  //d = words( infile );

	for (i=0; i<nthreads; i++)
	{
   	 	r = pthread_create(&thr[i],NULL, Threads,NULL); 
  	}


  
  	for(i=0; i<nthreads; i++)
  	{
     		pthread_join(thr[i],NULL);
  	}

  print_dict();
  fclose( infile );
}

/*void *pmsfun(void *ptr);

main()
{
	pthread_t thread1,thread2;
	const char *msg1="Thread 1";
	const char *msg2="Thread 2";
	int iret1,iret2;
	
	iret1=pthread_create(&thread1,NULL,pmsfun,(void*)msg1);
	if(iret1)
	{
		fprintf(stderr,"Error - pthread_create() return code: %d\n",iret1);
		exit(EXIT_FAILURE);
	}

	iret2=pthread_create(&thread2,NULL,pmsfun,(void*)msg2);
	if(iret2)
	{
		fprintf(stderr,"Error - pthread_create() return code: %d\n",iret2);
		exit(EXIT_FAILURE);
	}

	printf("pthread_create() for thread1 returns: %d\n",iret1);
	printf("pthread_create() for thread2 returns: %d\n",iret2);



	pthread_join(thread1,NULL);
	pthread_join(thread2,NULL);

	exit(EXIT_SUCCESS);
}

void *pmsfun(void *ptr)
{
	char *msg;
	msg=(char *)ptr;
	printf("%s \n",msg);
}*/

void *Threads()
{	
	int v=1;
	char w[MAXWORD];
	while(v){
		pthread_mutex_lock(&joy);
    		v=get_word(w,v,infile);
    		pthread_mutex_unlock(&joy);
    		if (v==0)	
		{
			 break;
		}
    		pthread_mutex_lock(&joy1);
    		insert_word(w);
    		pthread_mutex_unlock(&joy1);
	}
	pthread_exit(NULL);
}


 
