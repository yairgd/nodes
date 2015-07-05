input qqWWW123,WW,GG,HH12abc ;
input   QQ,WW,GG,HH ;
	output AA,BB,CC;

//wedweewd
node AA {
	input QQ,WW,GG,HH;
	output AA,BB ,CC;
	
	
	node  BB {
	input QQ,WW,GG,HH ;
		output AA,BB,CC;

	}
	
	
	node CC {
		input QQ,WW,GG,HH;
		output AA,BB,CC ;
	
		node EE {
			input QQ,WW,GG,HH;
			output AA,BB,CC;
		}
	}

//	CC cc1(a,b),cc2(a)
}



node DD {
		input QQ,WW,GG,HH;
		output AA,BB,CC;
	}

DD.CC=AA.QQ;
