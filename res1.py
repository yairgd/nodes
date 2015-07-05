import Algo

class MM(Algo.Nodes.Node.Node):
	def __init__(self,node_manager):
                super(MM, self).__init__(2,2,1,node_manager)
		self.setCallback(self.callback,[0,1 ],[0,1])
		pass
	def callback(self,a,b):
		pass
            
class NN(Algo.Nodes.Node.Node):
        def __init__(self,node_manager):
                super(NN, self).__init__(2,2,1,node_manager)            
		self.setCallback(self.callback,[0,1 ],[0,1])
		pass
	def callback(self,a,b):
		pass



##{Generator 
# initiate node_manager with 8 workers and 367 nodes 

class top_res:
	#Initiates nodes
	def __init__(self):
		self.node_manager=Algo.Nodes.Node.NodeManager(367,8)
		self.mm1_realzie = MM(self.node_manager)
		self.nn1_realzie = NN(self.node_manager)
		self.kk1_mm3_realzie = MM(self.node_manager)
		self.kk1_mm2_realzie = MM(self.node_manager)
		self.kk1_mm1_realzie = MM(self.node_manager)
		self.kk1_mm1_realzie.setInput(self.kk1_mm3_realzie.getOutput(0),0)


top = top_res()
