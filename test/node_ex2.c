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








void node_random(node_t *node,void *prm)
{
	double *aa = node_get_output_value(node,0);
	*aa = 1.0 * rand();
}

void node_random1(node_t *node,void *prm)
{
	double *aa = node_get_output_value(node,0);
	*aa = 1.0 * rand();
}




void ref_log1(node_t *node,void *prm)
{
	void *a,*b,*c,*d,*test;
	static int  ii=0;
	//usleep(50000);
	node_get_input_value(node,0,&a);
	node_get_input_value(node,1,&b);
	node_get_input_value(node,2,&c);
	node_get_input_value(node,3,&d);
	node_get_input_value(node,4,&test);
	double ref = (*(double*)a + *(double*)b) * (*(double*)c - *(double*)d);
	printf("%d  %s:   %lf  %lf\n",ii++,(char *)prm,ref  , *(double*)test);

//	if (ref!=*(double*)test)
//		exit(0);
	//	
//	printf("%s:   %lf %f %f %f %f %f %f \n",(char *)prm,ref  - *(double*)test,*(double*)a,*(double*)b,*(double*)c,*(double*)d,ref, *(double*)test);


}

void action_plus(node_t *node,void *prm)
{
	void *a,*b;
	node_get_input_value(node,0,&a);
	node_get_input_value(node,1,&b);
	double *res = node_get_output_value(node,0);
	*res = (*(double*)a + *(double*)b);


//	printf("+: %f\n",*res);


}


void action_minus(node_t *node,void *prm)
{
	void *a,*b;
	node_get_input_value(node,2,&a);
	node_get_input_value(node,3,&b);
	double *res = node_get_output_value(node,1);
	*res = (*(double*)a - *(double*)b);
//	printf("-: %f %f-%f\n",res ,*(double*)a,*(double*)b  );
//<t_úX>	printf("-: %f\n",*res);


}

void action_mul(node_t *node,void *prm)
{
	void *a,*b;
	node_get_input_value(node,0,&a);
	node_get_input_value(node,1,&b);
	double *res = node_get_output_value(node,0);
	*res = (*(double*)a * *(double*)b);

}
//	node_t a,b,c,d,mul,action,log1;

void test1_init(node_manager_t *node_manager )
{
	

	node_t *a,*b,*c,*d,*mul,*action,*log1;



	node_init(&a,0,1,5,node_manager); //node_random,"a");
	node_add_callback(a,10,node_random,"a",0);
	NODE_INIT_OUTPUT(*a,0,double,10);
	
	node_init(&b,0,1,5,node_manager);//node_random1,"b");
	node_add_callback(b,10,node_random1,"b",0);
	NODE_INIT_OUTPUT(*b,0,double,10);
	
        node_init(&c,0,1,5,node_manager); //node_random,"a");
	node_add_callback(c,10,node_random,"c",0);
	NODE_INIT_OUTPUT(*c,0,double,10);
	
	node_init(&d,0,1,5,node_manager);//node_random1,"b");
	node_add_callback(d,10,node_random1,"d",0);
	NODE_INIT_OUTPUT(*d,0,double,10);
//	node_set_output_id(d,0,10);



	node_init(&action,4,2,5,node_manager);//node_action,"+");
	NODE_INIT_OUTPUT(*action,0,double,10);
	NODE_INIT_OUTPUT(*action,1,double,11);
	node_add_callback(action,10,action_plus,"+",0);
	node_add_callback(action,11,action_minus,"-",0);

        node_init(&mul,2,1,5,node_manager);//node_action,"+");
	NODE_INIT_OUTPUT(*mul,0,double,10);
	node_add_callback(mul,10,action_mul,"*",0);
//	node_set_output_id(mul,0,10);

//	node_set_input_id(mul,0,10);
//	node_set_input_id(mul,0,10);




	node_init(&log1,5,1,5,node_manager);//node_action,"+");
	NODE_INIT_OUTPUT(*log1,0,double,10);		
	node_add_callback(log1,10,ref_log1,"ref",0);

		

	node_connect(a,0,action,0,10);
	node_connect(b,0,action,1,10);
	node_connect(c,0,action,2,11);
	node_connect(d,0,action,3,11);

	node_connect(action,0,mul,0,10);
	node_connect(action,1,mul,1,10);


        node_connect(a,0,log1,0,10);
	node_connect(b,0,log1,1,10);
	node_connect(c,0,log1,2,10);
	node_connect(d,0,log1,3,10);
	node_connect(mul,0,log1,4,10);


	


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
 

// gcc test3.c node.c thread.c list.c -o test3 -g -lpthread 
int main()
{
	node_manager_t *node_manager;

	srand(time(NULL));
	node_manager_init(&node_manager,255,1); //test1_init);
	test1_init(node_manager);

//	node_manager_step(node_manager,59000);
//	return 0;

	node_manager_thread_prm_t node_manager_thread_prm;
	node_manager_thread_prm.num_of_steps = 100000;
	node_manager_thread_prm.node_manager = node_manager;

	node_manager_start(node_manager,&node_manager_thread_prm);



	 while(!node_manager->tid.suspended)
	 {
		if (kbhit())
			break;
	 }

	return 0;
}

