import libxml2
import socket
class Command:
	def __init__(self):
		pass
	def getAnswer(self,ans):
		return ans
	def getXML(self):
		pass;
class Client:
	def __init__(self,host,port):
		self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.s.connect((host, port))
		self.lastCommand=Command();
	def send(self,command):
		self.s.send(command.getXML())
		self.lastCommand = command
		ans = self.s.recv(10000000);
		return self.lastCommand.getAnswer(ans)
	def recvLast(self):
		ans = self.s.recv(10000000);
		return self.lastCommand.getAnswer(ans)
	def disconnect(self):
		self.s.close()


######## Login and login response ####################33\
class Login(Command):
	def __init__(self,username,password):
		self.username = username
		self.password = password
		self.url="http://www.fxcorporate.com/Hosts.jsp"
		self.connection="Demo"
		self.xml=libxml2.newDoc('1.0')
		self.login()
	def getXML(self):
		return  self.xml.serialize('UTF-8')
	def login(self):
		dataRoot = libxml2.newNode('document')
	    	dataRoot.docSetRootElement(self.xml)

		# add command name
		logon=libxml2.newNode('cmd')
		logon.setProp('broker' , 'FXCM')
		logon.setProp('type' , 'login')
		dataRoot.addChild(logon)


		# add user name
		username=libxml2.newNode('username')
		username.setContent(self.username)
		logon.addChild(username)

		# add password
		password=libxml2.newNode('password')
		password.setContent(self.password)
		logon.addChild(password)

		# add url
		url=libxml2.newNode('url')
		url.setContent(self.url)
		logon.addChild(url)


		# add coonection
		connection=libxml2.newNode('connection')
		connection.setContent(self.connection)
		logon.addChild(connection)
	def getAnswer(self,ans):
		return LoginResponse(ans)

class LoginResponse:
	def __repr__(self):
		#members = [attr for attr in dir(a) if not callable(attr) and not attr.startswith("__")]
		#print "ss"
		keys = self.__dict__.keys()
		s='';
		for k in keys:
			s=s+"%s = %s\n"%(k,self.__dict__[k])
		return s
	def __init__(self,ans):
		doc = libxml2.parseDoc(ans)
		root = doc.children
		child = root.children
		if (child.properties.name=="id"):
			self.id = int (child.properties.content) ;
		if (child.properties.name=="type"):
			self.type = child.properties.content ;
		if (child.next.name=="status"):
			self.status = child.next.content;

######## GetOffersTable and login GetOffersResponse ####################33\

class GetOffersTable(Command):
	def __init__(self,login_res,get_all):
		self.id = login_res.id
		self.get_all = get_all
		self.xml=libxml2.newDoc('1.0')
		self.getOffersTable()
	def getXML(self):
		return  self.xml.serialize('UTF-8')
	def getOffersTable(self):
		dataRoot = libxml2.newNode('document')
		dataRoot.docSetRootElement(self.xml)
		# add command name
		cmd=libxml2.newNode('cmd')
		cmd.setProp('broker' , 'FXCM')
		cmd.setProp('type' , 'GetOffersTable')
		dataRoot.addChild(cmd)


		# add user name 
		# todo: impliment GetOffersTable with instrument list parameter
		# add child name like:  <Intrument> EUR/USD</Instrument>
		getall=libxml2.newNode('GetAll')
		getall.setContent(self.get_all)
		cmd.addChild(getall)

		ID=libxml2.newNode('ID')
		ID.setContent(str(self.id))
		cmd.addChild(ID)
	def getAnswer(self,ans):
		a={};
		try:	
			doc = libxml2.parseDoc(ans)
			root = doc.children
			child = root.children
			ask=list();
			bid=list();
			time=list();
			instrument=list();
			while child is not None:
				if (child.type == "element" and child.name=="Instrument" and child.properties.lsCountNode>0):
					for property in child.properties:
						 if property.type == 'attribute':	
								instrument.append(property.content);
					
					subchild = child.children;
					while subchild is not None:
						if subchild.name=="ask":
							ask.append(child.content);
						if subchild.name=="bid":
							ask.append(child.content);
						if subchild.name=="time":
							time.append(child.content);
						subchild = subchild.next


				child = child.next
			doc.freeDoc()
			a["ask"]=ask
			a["bid"]=bid
			a["time"]=time
			a["instrument"]=instrument
		except:
			pass
		return a



class GetHistoricalData(Command):
	def __init__(self,login_res,instrument,time_frame,size):
		self.instrument = instrument
		self.time_frame = time_frame
		self.id = login_res.id
		self.size = size
		self.xml=libxml2.newDoc('1.0')
		self.getHistoricalData()
	def getXML(self):
		return  self.xml.serialize('UTF-8')
	def getHistoricalData(self):
		dataRoot = libxml2.newNode('document')
		dataRoot.docSetRootElement(self.xml)
		
		cmd=libxml2.newNode('cmd')
		cmd.setProp('broker' , 'FXCM')
		cmd.setProp('type' , 'GetHistoricalData')
		dataRoot.addChild(cmd)

		instrument=libxml2.newNode('Instrument')
		instrument.setContent(self.instrument)
		cmd.addChild(instrument)

		time_frame=libxml2.newNode('TimeFrame')
		time_frame.setContent(self.time_frame)
		cmd.addChild(time_frame)

		ID=libxml2.newNode('ID')
		ID.setContent(str(self.id))
		cmd.addChild(ID)

		size=libxml2.newNode('Size')
		size.setContent(self.size)
		cmd.addChild(size)

	def getAnswer(self,ans):
		a={};
		a["AskOpen"]=list();
		a["AskHigh"]=list();
		a["AskLow"]=list();
		a["AskClose"]=list();
		a["BidOpen"]=list();
		a["BidHigh"]=list();
		a["BidLow"]=list();
		a["BidClose"]=list();
		a["id"]=list();
		try:
			doc = libxml2.parseDoc(ans)
			root = doc.children
			row = root.children.next.next
			while row is not None:
				for property in row.properties:
					if property.name in a.keys():
						a[property.name].append(float(property.content))
				row=row.next
		except:
			pass
		return a

class OpenPosition(Command):
	def __init__(self,id,instrument):
		self.instrument = instrument
		self.id = id
		self.xml=libxml2.newDoc('1.0')
		self.openPosition()
	def getXML(self):
		return  self.xml.serialize('UTF-8')
	def openPosition(self):
		dataRoot = libxml2.newNode('document')
		dataRoot.docSetRootElement(self.xml)
		
		cmd=libxml2.newNode('cmd')
		cmd.setProp('broker' , 'FXCM')
		cmd.setProp('type' , 'OpenPosition')
		dataRoot.addChild(cmd)

		instrument=libxml2.newNode('Instrument')
		instrument.setContent(self.instrument)
		cmd.addChild(instrument)

		
		ID=libxml2.newNode('ID')
		ID.setContent(str(self.id))
		cmd.addChild(ID)

	
	def getAnswer(self,ans):
		return ans


class ClosePosition(Command):
	def __init__(self,id,instrument):
		self.instrument = instrument
		self.id = id
		self.xml=libxml2.newDoc('1.0')
		self.openPosition()
	def getXML(self):
		return  self.xml.serialize('UTF-8')
	def openPosition(self):
		dataRoot = libxml2.newNode('document')
		dataRoot.docSetRootElement(self.xml)
		
		cmd=libxml2.newNode('cmd')
		cmd.setProp('broker' , 'FXCM')
		cmd.setProp('type' , 'ClosePosition')
		dataRoot.addChild(cmd)

		instrument=libxml2.newNode('Instrument')
		instrument.setContent(self.instrument)
		cmd.addChild(instrument)

		
		ID=libxml2.newNode('ID')
		ID.setContent(str(self.id))
		cmd.addChild(ID)

	
	def getAnswer(self,ans):
		return ans


def a():
	client = fxcm.Client ('127.0.0.1',50006)
	loginres = client.send(fxcm.Login("D172145730001","5300"))
	a = client.send(fxcm.GetOffersTable(loginres,"yes"))
	a= client.send(fxcm.GetHistoricalData(loginres,"EUR/USD","m5","500"))



	client.disconnect()

