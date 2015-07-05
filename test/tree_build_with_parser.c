/*
 * =====================================================================================
 *
 *       Filename:  tree_build_with_parser.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/30/15 14:05:21
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include "parser.h"
#include "node.h"
#include "stack.h"


const char *text="\
node  MM {\
		input min1,min2  ;\
		output mout1,mout2;\
}\ 
node  NN {\
		input nin1,nin2  ;\
		output nout1,nout2;\
}\ 
node  KK {\
		input kin1,kin2  ;\
		output kout1,kout2,kout3;\
		MM mm1(a,b,c,d,e=4),mm2(a,b,c),mm3(c=4,d=\"KKK\");\
		mm2.mout2->kout1;\
		mm3.mout2->kout3;\
		kin1->mm2.min1;\
}\
node  AA {\
	input ain1,ain2,ain3 ;\
	output aout1,aout2;\
	node  BB {\
		input bin1,bin2  ;\
		output bout1,bout2,bout3;\
		KK kkkk1(a,b,c,d,e=4),kkkk2(a,b,c),kkkk3(c=4,d=\"KKK\");\
		kkkk2.kout2 -> bout2 ;\
		bin1 -> kkkk1.kin2 ;\
		bin1 -> kkkk1.kin1 ;\
		kkkk1.kout3 -> bout1 ;\
		kkkk3.kout3 -> bout3;\
	}\
	input ain4,ain5,ain6 ;\
	AA aaaaa1(a,b,c,d,e=4),aaaaa2(a,b,c),aaaaa3(c=4,d=\"KKK\");\
	CC cc1(a,b,c,d,e=4),cc2(a,b,c),cc3(c=4,d=\"KKK\");\
	BB bb1,bb2(a,b,c,d=5.1234),bb3(c=3,d=\"ddd\");\	
	KK kk1(a,b,c,d,e=4),kk2(a,b,c),kk3(c=4,d=\"KKK\");\
	bb1.bout2 -> kk2.kin2;\
	bb2.bout1 -> bb3.bin1; \
	ain1->kk1.kin1;\
	bb1.bout2 -> aa1.ain55;\	
}\
AA aa1(a,b,c,d,e=4),aa2(a,b,c),aa3(c=4,d=\"KKK\");\
KK kk1(r,s,t);\
aa1.bb1.bout2 -> aa1.ain2;\
aa1.bb1.wedwedwed -> aa1.ain2;\
kk1.kout1 -> aa1.bb1.bin1;\
NN nn1;\
MM mm1;\
mm1.mout2->nn1.nin1;\
nn1.nout1->mm1.min2;\
nn1.nout2->mm1.min1;\	
";



int main()
{
	int col,line;
	char *msg;
	parse(text,"res",&col,&line,&msg);
	printf("stop at line %d\n",line);
	return 0;
}
