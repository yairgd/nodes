/*
 * =====================================================================================
 *
 *       Filename:  test1.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/28/14 13:53:31
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov
 *   Organization:  
 *
 * =====================================================================================
 */



#include "node.h"
#include <stdlib.h>






void node_action(node_t *node,void *prm)
{
	char *action = prm;
	void *a ;	
	void *b ;
	double *c;

	c = node_get_output_value(node,0);
	//if ( 1) // node_get_input(node,0,&a) && node_get_input(node,1,&b))
	{
		//	printf("%s: %f %f\n",(char*)node->prm,*(double*)a,*(double*)b);
		node_get_input_value(node,0,&a) ;
		node_get_input_value(node,1,&b) ; 
		switch (*action)
		{
			case '+':
				*c=*(double*)a + *(double*)b;
				break;
			case '-':
				*c=*(double*)a - *(double*)b;
				break;

			case '/':
				*c=*(double*)a / *(double*)b;
				break;

			case '*':
				*c=*(double*)a * *(double*)b;
				break;
		}
	//}
//	node_trig_output(node,0,&c);
 
//	node_release_input(node,0);
//	node_release_input(node,1);

	}
}

void node_random(node_t *node,void *prm)
{
	double *aa = node_get_output_value(node,0);

	*aa = 1.0 * rand();
//	node_trig_output(node,0,&aa);
}

void node_random1(node_t *node,void *prm)
{
	double *aa = node_get_output_value(node,0);

	*aa = 1.0 * rand();

}



void node_log1(node_t *node,void *prm)
{
	void *ref ;
	void *aa ;
	void *bb ;

	//	if (node_get_input(node,0,&ref) && node_get_input(node,1,&aa) && node_get_input(node,2,&bb) )
	{

		node_get_input_value(node,0,&ref) ; 
		node_get_input_value(node,1,&aa) ; 
		node_get_input_value(node,2,&bb) ;
		double test = *(double*)aa + *(double*)bb;
		//printf("%s:   %lf \n",(char *)prm,test  - *(double*)ref);
		printf("%s:   %lf  %lf\n",(char *)prm,test  - *(double*)ref, *(double*)aa);

		//		node_release_input(node,0);
		//		node_release_input(node,1);
		//		node_release_input(node,2);

	}

	//	node_trig_output(node,0);

}
void node_log2a(node_t *node,void *prm)
{
	void *ref ;//= node_get_input(node,0);
	void *aa ;//=  node_get_input(node,1);
	void *bb ;//=  node_get_input(node,2);

	//if (aa && bb && ref)
//	if (node_get_input(node,0,&ref) && node_get_input(node,1,&aa) && node_get_input(node,2,&bb) ) 
//	{
	
node_get_input_value(node,0,&ref) ; node_get_input_value(node,1,&aa) ; node_get_input_value(node,2,&bb);
		double test = *(double*)aa - *(double*)bb;
	printf("%s:   %lf \n",(char *)prm,test  - *(double*)ref);

	//	printf("%s: test-ref:%f  test:%f  ref:%f a:%f b:%f\n",(char *)prm, test - *ref,test,*ref,*aa,*bb);
//	node_release_input(node,0);
// 	node_release_input(node,1);
// 	node_release_input(node,2);

	
//	}

//	node_trig_output(node,0);

}
void node_log3(node_t *node,void *prm)
{
	void *ref ;//= node_get_input(node,0);
	void *aa ;//=  node_get_input(node,1);
	void *bb ;//=  node_get_input(node,2);

	//if (aa && bb && ref)
//	if (node_get_input(node,0,&ref) && node_get_input(node,1,&aa) && node_get_input(node,2,&bb) )
//<t_úX>	{

	node_get_input_value(node,0,&ref) ; node_get_input_value(node,1,&aa) ; node_get_input_value(node,2,&bb);
		double test = (*(double*)aa - *(double*)bb) * (*(double*)aa + *(double*)bb);

		printf("%s:   %lf \n",(char *)prm,test  - *(double*)ref);
//		if ( (test  - *(double*)ref))
//			exit(0);
		
//		node_release_input(node,0);
//		node_release_input(node,1);
//		node_release_input(node,2);


		//	printf("%s: test-ref:%f  test:%f  ref:%f a:%f b:%f\n",(char *)prm, test - *ref,test,*ref,*aa,*bb);
//	}

	//	node_trig_output(node,0);

}

//node_t a,b,c,d,e,log1,log2a,log3;
void test1_init(node_manager_t *node_manager )

{
	
node_t *a,*b,*c,*d,*e,*log1,*log2a,*log3;


	node_init(&a,0,1,5,node_manager); //node_random,"a");
	node_add_callback(a,10,node_random,"a",0);
	NODE_INIT_OUTPUT(*a,0,double,10);
	
	node_init(&b,0,1,5,node_manager);//node_random1,"b");
	node_add_callback(b,10,node_random1,"b",0);
	NODE_INIT_OUTPUT(*b,0,double,10);
	
	node_init(&c,2,1,5,node_manager);//node_action,"+");
	node_add_callback(c,10,node_action,"+",0);
	NODE_INIT_OUTPUT(*c,0,double,10);
	
	node_init(&d,2,1,5,node_manager);//node_action,"-");
	node_add_callback(d,10,node_action,"-",0);
	NODE_INIT_OUTPUT(*d,0,double,10);

	node_init(&e,2,1,5,node_manager);//node_action,"*");
	node_add_callback(e,10,node_action,"*",0);
	NODE_INIT_OUTPUT(*e,0,double,10);


	node_init(&log1 ,3,0,5,node_manager);//node_log1 ,"logger(+)");
	node_add_callback(log1,10,node_log1,"logger(+)",0);
	
	node_init(&log2a,3,0,5,node_manager);//node_log2a,"logger(-)");
	node_add_callback(log2a,10,node_log2a,"logger(-)",0);

	node_init(&log3 ,3,0,5,node_manager);//node_log3 ,"logger(*)");
	node_add_callback(log3,10,node_log3,"logger(*)",0);


	node_connect(a,0,c,0,10);
		
	
	node_connect(b,0,c,1,10);
	node_connect(c,0,log1,0,10);
	node_connect(a,0,log1,1,10);
	node_connect(b,0,log1,2,10);

	node_connect(a,0,d,0,10);
	node_connect(b,0,d,1,10);
	node_connect(d,0,log2a,0,10);
	node_connect(a,0,log2a,1,10);
	node_connect(b,0,log2a,2,10);


	node_connect(c,0,e,0,10);
	node_connect(d,0,e,1,10);
	node_connect(e,0,log3,0,10);
	node_connect(a,0,log3,1,10);
	node_connect(b,0,log3,2,10);





}

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
 
int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;
 
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
 
  ch = getchar();
 
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);
 
  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }
 
  return 0;
}
 

// gcc test1.c node.c thread.c list.c -o test1 -g -lpthread 
int main()
{
	node_manager_t *node_manager;

	srand(time(NULL));
	node_manager_init(&node_manager,255,20); //test1_init);

	test1_init(node_manager);


	node_manager_thread_prm_t node_manager_thread_prm;
	node_manager_thread_prm.num_of_steps = 59000;
	node_manager_thread_prm.node_manager = node_manager;

	node_manager_start(node_manager,&node_manager_thread_prm);



	 while(1)
	 {
		if (kbhit())
			break;
	 }


}

