import random
import  Algo
import math

delta = 0.0001
class Plus(Algo.Nodes.Node.Node):
	def __init__(self,node_manager,A,B):
   		super(Plus, self).__init__(2,1,1,node_manager)
		self.setup(A,B);
	def callback1(self,a,b):
        #        print "plus"
		try:
			self.val = a+b
	#		print "plus: {0}+{1}={2}".format(a,b,a+b)
			return self.val;
		except:
	#		print a,b
			return 0
	def setup(self,A,B):
		self.setInput(A.getOutput(0),0)
		self.setInput(B.getOutput(0),1)
		self.setCallback(self.callback1,[0, 1],[0])
	def __repr__(self):
		return Algo.Nodes.Node.Node.__repr__(self);

class Minus(Algo.Nodes.Node.Node):
	def __init__(self,node_manager,A,B):
   		super(Minus, self).__init__(2,1,1,node_manager)
		self.setup(A,B);
	def callback1(self,a,b):
                #print "minus"
		try:
			self.val = a-b
	#		print "minus: {0}-{1}={2}".format(a,b,a-b)
			return self.val;
		except:
	#		print a,b
			return 0
	def setup(self,A,B):
		self.setInput(A.getOutput(0),0)
		self.setInput(B.getOutput(0),1)
		self.setCallback(self.callback1,[0, 1],[0])
	def __repr__(self):
		return Algo.Nodes.Node.Node.__repr__(self);


class Mul(Algo.Nodes.Node.Node):
	def __init__(self,node_manager,A,B):
   		super(Mul, self).__init__(2,1,1,node_manager)
		self.setup(A,B);
	def callback1(self,a,b):
                #print "mul"
		try:
			self.val = a*b
	#		print "mul: {0}*{1}={2}".format(a,b,a*b)
			return self.val;
		except:
	#		print a,b
			return 0
	def setup(self,A,B):
		self.setInput(A.getOutput(0),0)
		self.setInput(B.getOutput(0),1)
		self.setCallback(self.callback1,[0, 1],[0])
	def __repr__(self):
		return Algo.Nodes.Node.Node.__repr__(self);




class Random1(Algo.Nodes.Node.Node):
	def __init__(self,node_manager,name):
   		super(Random1, self).__init__(0,1,1,node_manager)
		self.name = name
		self.setup()
                self.idx=0;
	def callback(self):
                #print "random1"
	#	print type(h)
		try:
			self.val = random.randint(0,100)
                        self.idx = self.idx+1;
		#	print "rand {1}: {0}".format(self.val,self.name)
			return self.val;
		except:
		#	print "aaaa"
			return 0
	def setup(self):
		self.setCallback(self.callback,[],[0])
	def __repr__(self):
		return Algo.Nodes.Node.Node.__repr__(self);


class Log1(Algo.Nodes.Node.Node):
	def __init__(self,node_manager,A,B,C,D,MUL):
   		super (Log1,self).__init__(5,0,1,node_manager)
		self.setInput(A.getOutput(0),0)
		self.setInput(B.getOutput(0),1)
		self.setInput(C.getOutput(0),2)
		self.setInput(D.getOutput(0),3)
		self.setInput(MUL.getOutput(0),4)
		self.setCallback(self.callback,[0,1,2,3,4 ],[])
		self.MUL=MUL
	def callback(self,a,b,c,d,mul):
             #   print "log"
		try:
			self.val=(a+b)*(c-d);
		 # 	print "log1: {0},{1},{2}".format(self.val,self.val-mul,self.val-self.MUL.val)
                        print "log1: {0},{1},{2}".format(self.val,mul,self.MUL.val)
		  #	self.val=0;
		  #	print self.val
			return self.val;
	  	except:
                     #   print "err arg"
                        pass
			#print "dd"
	def __repr__(self):
		return Algo.Nodes.Node.Node.__repr__(self);
class Exp(Algo.Nodes.Node.Node):
	def __init__(self,node_manager,A):
   		super(Exp, self).__init__(2,1,1,node_manager)
		self.setup(A);
                self.val=delta;
                self.t=0;
	def callback1(self,a,b):
		try:
       		        #import pdb; pdb.set_trace()  # XXX BREAKPOINT

			self.val = a*b
                        self.t=self.t+delta
                        print "time {0}, calc exp:{1}, real exp:{2}".format(self.t,self.val/delta,math.exp(self.t))
	#		print "mul: {0}*{1}={2}".format(a,b,a*b)
			return self.val;
		except:
	#		print a,b
			return 0.0001
	def setup(self,A):
		self.setInput(A.getOutput(0),0)
                self.setInput(self.getOutput(0),1)
		self.setCallback(self.callback1,[0,1],[0])
	def __repr__(self):
		return Algo.Nodes.Node.Node.__repr__(self);
class Const(Algo.Nodes.Node.Node):
	def __init__(self,node_manager,n):
   		super(Const, self).__init__(0,1,1,node_manager)
                self.val = n
                self.setup();
	def callback1(self):
               	    return self.val
	def setup(self):
		self.setCallback(self.callback1,[],[0])
	def __repr__(self):
		return Algo.Nodes.Node.Node.__repr__(self);


node_manager=Algo.Nodes.Node.NodeManager(11,5)
random1=Random1(node_manager,"A");
random2=Random1(node_manager,"B");
random3=Random1(node_manager,"C");
random4=Random1(node_manager,"D");
const = Const (node_manager,1+delta);
exp = Exp (node_manager,const);

plus = Plus(node_manager,random1,random2)
minus = Minus(node_manager,random3,random4)
mul = Mul(node_manager,plus,minus)
#log1 = Log1(node_manager,random1,random2,random3,random4,mul);


node_manager.step(10001);
