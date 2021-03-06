import Algo
class MM(Algo.Nodes.Node.Node):
	def __init__(self,node_manager):
		super(MM,self).__init__(2,2,1,node_manager)
		pass
	def callback(self,min2,min1):
		pass
class NN(Algo.Nodes.Node.Node):
	def __init__(self,node_manager):
		super(NN,self).__init__(2,2,1,node_manager)
		pass
	def callback(self,nin2,nin1):
		pass



##{Generator 
# initiate node_manager with 8 workers and 367 nodes 
class top_res:
	def __init__(self):
		self.node_manager=Algo.Nodes.Node.NodeManager(367,8)
		#Initiates nodes
		self.mm1 = MM(self.node_manager)
		self.nn1 = NN(self.node_manager)
		self.kk1_mm3 = MM(self.node_manager)
		self.kk1_mm2 = MM(self.node_manager)
		self.kk1_mm1 = MM(self.node_manager)
		self.aa3_kk3_mm3 = MM(self.node_manager)
		self.aa3_kk3_mm2 = MM(self.node_manager)
		self.aa3_kk3_mm1 = MM(self.node_manager)
		self.aa3_kk2_mm3 = MM(self.node_manager)
		self.aa3_kk2_mm2 = MM(self.node_manager)
		self.aa3_kk2_mm1 = MM(self.node_manager)
		self.aa3_kk1_mm3 = MM(self.node_manager)
		self.aa3_kk1_mm2 = MM(self.node_manager)
		self.aa3_kk1_mm1 = MM(self.node_manager)
		self.aa3_bb3_kkkk3_mm3 = MM(self.node_manager)
		self.aa3_bb3_kkkk3_mm2 = MM(self.node_manager)
		self.aa3_bb3_kkkk3_mm1 = MM(self.node_manager)
		self.aa3_bb3_kkkk2_mm3 = MM(self.node_manager)
		self.aa3_bb3_kkkk2_mm2 = MM(self.node_manager)
		self.aa3_bb3_kkkk2_mm1 = MM(self.node_manager)
		self.aa3_bb3_kkkk1_mm3 = MM(self.node_manager)
		self.aa3_bb3_kkkk1_mm2 = MM(self.node_manager)
		self.aa3_bb3_kkkk1_mm1 = MM(self.node_manager)
		self.aa3_bb2_kkkk3_mm3 = MM(self.node_manager)
		self.aa3_bb2_kkkk3_mm2 = MM(self.node_manager)
		self.aa3_bb2_kkkk3_mm1 = MM(self.node_manager)
		self.aa3_bb2_kkkk2_mm3 = MM(self.node_manager)
		self.aa3_bb2_kkkk2_mm2 = MM(self.node_manager)
		self.aa3_bb2_kkkk2_mm1 = MM(self.node_manager)
		self.aa3_bb2_kkkk1_mm3 = MM(self.node_manager)
		self.aa3_bb2_kkkk1_mm2 = MM(self.node_manager)
		self.aa3_bb2_kkkk1_mm1 = MM(self.node_manager)
		self.aa3_bb1_kkkk3_mm3 = MM(self.node_manager)
		self.aa3_bb1_kkkk3_mm2 = MM(self.node_manager)
		self.aa3_bb1_kkkk3_mm1 = MM(self.node_manager)
		self.aa3_bb1_kkkk2_mm3 = MM(self.node_manager)
		self.aa3_bb1_kkkk2_mm2 = MM(self.node_manager)
		self.aa3_bb1_kkkk2_mm1 = MM(self.node_manager)
		self.aa3_bb1_kkkk1_mm3 = MM(self.node_manager)
		self.aa3_bb1_kkkk1_mm2 = MM(self.node_manager)
		self.aa3_bb1_kkkk1_mm1 = MM(self.node_manager)
		self.aa2_kk3_mm3 = MM(self.node_manager)
		self.aa2_kk3_mm2 = MM(self.node_manager)
		self.aa2_kk3_mm1 = MM(self.node_manager)
		self.aa2_kk2_mm3 = MM(self.node_manager)
		self.aa2_kk2_mm2 = MM(self.node_manager)
		self.aa2_kk2_mm1 = MM(self.node_manager)
		self.aa2_kk1_mm3 = MM(self.node_manager)
		self.aa2_kk1_mm2 = MM(self.node_manager)
		self.aa2_kk1_mm1 = MM(self.node_manager)
		self.aa2_bb3_kkkk3_mm3 = MM(self.node_manager)
		self.aa2_bb3_kkkk3_mm2 = MM(self.node_manager)
		self.aa2_bb3_kkkk3_mm1 = MM(self.node_manager)
		self.aa2_bb3_kkkk2_mm3 = MM(self.node_manager)
		self.aa2_bb3_kkkk2_mm2 = MM(self.node_manager)
		self.aa2_bb3_kkkk2_mm1 = MM(self.node_manager)
		self.aa2_bb3_kkkk1_mm3 = MM(self.node_manager)
		self.aa2_bb3_kkkk1_mm2 = MM(self.node_manager)
		self.aa2_bb3_kkkk1_mm1 = MM(self.node_manager)
		self.aa2_bb2_kkkk3_mm3 = MM(self.node_manager)
		self.aa2_bb2_kkkk3_mm2 = MM(self.node_manager)
		self.aa2_bb2_kkkk3_mm1 = MM(self.node_manager)
		self.aa2_bb2_kkkk2_mm3 = MM(self.node_manager)
		self.aa2_bb2_kkkk2_mm2 = MM(self.node_manager)
		self.aa2_bb2_kkkk2_mm1 = MM(self.node_manager)
		self.aa2_bb2_kkkk1_mm3 = MM(self.node_manager)
		self.aa2_bb2_kkkk1_mm2 = MM(self.node_manager)
		self.aa2_bb2_kkkk1_mm1 = MM(self.node_manager)
		self.aa2_bb1_kkkk3_mm3 = MM(self.node_manager)
		self.aa2_bb1_kkkk3_mm2 = MM(self.node_manager)
		self.aa2_bb1_kkkk3_mm1 = MM(self.node_manager)
		self.aa2_bb1_kkkk2_mm3 = MM(self.node_manager)
		self.aa2_bb1_kkkk2_mm2 = MM(self.node_manager)
		self.aa2_bb1_kkkk2_mm1 = MM(self.node_manager)
		self.aa2_bb1_kkkk1_mm3 = MM(self.node_manager)
		self.aa2_bb1_kkkk1_mm2 = MM(self.node_manager)
		self.aa2_bb1_kkkk1_mm1 = MM(self.node_manager)
		self.aa1_kk3_mm3 = MM(self.node_manager)
		self.aa1_kk3_mm2 = MM(self.node_manager)
		self.aa1_kk3_mm1 = MM(self.node_manager)
		self.aa1_kk2_mm3 = MM(self.node_manager)
		self.aa1_kk2_mm2 = MM(self.node_manager)
		self.aa1_kk2_mm1 = MM(self.node_manager)
		self.aa1_kk1_mm3 = MM(self.node_manager)
		self.aa1_kk1_mm2 = MM(self.node_manager)
		self.aa1_kk1_mm1 = MM(self.node_manager)
		self.aa1_bb3_kkkk3_mm3 = MM(self.node_manager)
		self.aa1_bb3_kkkk3_mm2 = MM(self.node_manager)
		self.aa1_bb3_kkkk3_mm1 = MM(self.node_manager)
		self.aa1_bb3_kkkk2_mm3 = MM(self.node_manager)
		self.aa1_bb3_kkkk2_mm2 = MM(self.node_manager)
		self.aa1_bb3_kkkk2_mm1 = MM(self.node_manager)
		self.aa1_bb3_kkkk1_mm3 = MM(self.node_manager)
		self.aa1_bb3_kkkk1_mm2 = MM(self.node_manager)
		self.aa1_bb3_kkkk1_mm1 = MM(self.node_manager)
		self.aa1_bb2_kkkk3_mm3 = MM(self.node_manager)
		self.aa1_bb2_kkkk3_mm2 = MM(self.node_manager)
		self.aa1_bb2_kkkk3_mm1 = MM(self.node_manager)
		self.aa1_bb2_kkkk2_mm3 = MM(self.node_manager)
		self.aa1_bb2_kkkk2_mm2 = MM(self.node_manager)
		self.aa1_bb2_kkkk2_mm1 = MM(self.node_manager)
		self.aa1_bb2_kkkk1_mm3 = MM(self.node_manager)
		self.aa1_bb2_kkkk1_mm2 = MM(self.node_manager)
		self.aa1_bb2_kkkk1_mm1 = MM(self.node_manager)
		self.aa1_bb1_kkkk3_mm3 = MM(self.node_manager)
		self.aa1_bb1_kkkk3_mm2 = MM(self.node_manager)
		self.aa1_bb1_kkkk3_mm1 = MM(self.node_manager)
		self.aa1_bb1_kkkk2_mm3 = MM(self.node_manager)
		self.aa1_bb1_kkkk2_mm2 = MM(self.node_manager)
		self.aa1_bb1_kkkk2_mm1 = MM(self.node_manager)
		self.aa1_bb1_kkkk1_mm3 = MM(self.node_manager)
		self.aa1_bb1_kkkk1_mm2 = MM(self.node_manager)
		self.aa1_bb1_kkkk1_mm1 = MM(self.node_manager)
		#Create connections
top = top_res()
