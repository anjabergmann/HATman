import json
from random import randint
from helper.node import LabNode

import optparse

from twisted.internet import reactor
from twisted.internet.protocol import ServerFactory
from twisted.protocols.basic import NetstringReceiver

########################################################

def parse_args():
	usage = "usage: %prog [options]"

	parser = optparse.OptionParser(usage)

	help = "The port to listen on. Default to a random available port."
	parser.add_option('--port', type='int', help=help)

	help = "The interface to listen on. Default is localhost."
	parser.add_option('--iface', help=help, default='localhost')

	options, args = parser.parse_args()

	if len(args) != 0:
		parser.error('Bad arguments.')

	return options;


########################################################

class HatmanService(object):
	def __init__(self):
		self.nodes = self.createAllNodes()
		self.crossNodes = self.chooseCrossNodes(25)


#-------------------------------------------------------
# someFancyMethod
#-------------------------------------------------------
	def someFancyMethod(self, command):
		returnString = str(command);

		try:
			return returnString.encode("utf-8");
		except:
			return returnString;



#-------------------------------------------------------
# methods to create all cross nodes
#-------------------------------------------------------
	def createAllNodes(self):
		print("creating all nodes")
		nodes = [[0 for x in range(20)] for y in range(29)]

		# write every node of the Labyrinth into the list
		for j in range(0, 20):
			for i in range(0, 29):
				tempNode = LabNode(x=i * 20 + 40, y=j * 20 + 40)
				nodes[i][j] = tempNode

		return nodes


	def chooseCrossNodes(self, num):
		print("choosing new crossNodes")
		# choose more or less random crossnodes
		cNodes = []

		# add corners to crossnodes and mark as crossNodes
		self.nodes[0][0].sort = "cross"		#links unten
		self.nodes[0][19].sort = "cross"	#links oben
		self.nodes[28][0].sort = "cross"	#rechts unten
		self.nodes[28][19].sort = "cross"	#rechts oben
		cNodes.append(self.nodes[0][0])
		cNodes.append(self.nodes[0][19])
		cNodes.append(self.nodes[28][0])
		cNodes.append(self.nodes[28][19])


		# list of possible (allowed) coordinates
		xpos = [0, 28]
		ypos = [0, 19]
		# list of impossible (because too close) coordinates
		ximpos = [1, 27]
		yimpos = [1, 18]

		x = 0
		y = 0

		for i in range(num):
			# alternate between fixed x or y
			# 'fixed' means it is chosen from the list of already existing crossNodes
			if (i % 2 == 0):
				x = xpos[randint(0,len(xpos)-1)]
				# calculate new random int until one fits
				while (True):
					y = randint(0,19)
					#check if random position is too close to another crossNode
					if y not in yimpos:
						if y not in ypos:
							# add if not marked as impossible and not already in possible
							ypos.append(y)
							# mark the two adjoining nodes as impossible
							yimpos.append(y-2)
							yimpos.append(y-1)
							yimpos.append(y+1)
							yimpos.append(y+2)
						break

			else:
				y = ypos[randint(0,len(ypos)-1)]
				# calculate new random int until one fits
				while (True):
					x = randint(0,28)
					# check if random position is too close to another crossNode
					if x not in ximpos:
						if x not in xpos:
							# add to possible if it hasn't been already and is not in impossible
							xpos.append(x)
							# mark the adjoining nodes as impossible
							ximpos.append(x-2)
							ximpos.append(x-1)
							ximpos.append(x+1)
							ximpos.append(x+2)
						break

			# set sort to cross and add to crossNodes
			if self.nodes[x][y] not in cNodes:
				self.nodes[x][y].sort = "cross"
				cNodes.append(self.nodes[x][y])

		return cNodes




	def sendCrossNodes(self):
		print("sending crossNodes")
		coords = list(map(str, self.crossNodes))
		string = "\x02nodes,"+json.dumps(coords)+",\x03"

		try:
			return string.encode("utf-8")
		except:
			return string


########################################################


# An instance is created whenever a new client connects
# The protocol is created by the factory when the reactor receives a request
# A protocol basically has the methods: 
# connectionMade, connectionLost, dataReceived
# The NetstringReceiver also has the method stringReceived
# --> is called if a string is received, not only the first characters or something
# --> with dataReceived you'd have to buffer the data received to get the whole string
# DON'T implement dataReceived AND stringReceived --> will be tricky
# Docu: http://twistedmatrix.com/documents/current/api/twisted.protocols.basic.NetstringReceiver.html
class HatmanProtocol(NetstringReceiver):

	def connectionMade(self):
		self.factory.clients.append(self);
		print("\nINFO A new client connected.")
		print("INFO Number of clients connected: {}\n".format(len(self.factory.clients)));

	def connectionLost(self, reason):
		self.factory.clients.remove(self)
		#print("\nINFO A client disconnected.", reason);
		print("\nINFO A client disconnected.");
		print("INFO Number of clients connected: {}\n".format(len(self.factory.clients)));


	def stringReceived(self, request):
		if ',' not in request.decode("utf-8"): #bad request
			# self.transport.loseConnection();
			return;

		command = request.decode("utf-8")[1:-1].split(",");
		#print("INFO Received command", command);

		if (command[0] == "bye"):
			self.sendString("Bye!".encode("utf-8"));
			self.transport.loseConnection();
		elif(command[0] == "hi"):
			self.sendString("Hello, Client!".encode("utf-8"));
		elif(command[0] == "nodes"):
			print("got node request")
			self.factory.doSendCrossNodes()
		else:
			#self.sendString(self.factory.doSomeFancyMethod(command));
			self.factory.doSomeFancyMethod(request.decode("utf-8"));





# An instance is created once at the beginning
# If a client sends a request to the port the reactor is listening on, 
# the factory has the purpose to return a protocol.
# So each connection gets its own protocol object. 
# If the factory doesn't return a protocol, the connection is rejected. 
class HatmanFactory(ServerFactory):

	protocol = HatmanProtocol;

	def __init__(self, service):
		self.service = service

	def doSomeFancyMethod(self, command):
		index = 1;
		stringToSend = self.service.someFancyMethod(command);
		for client in self.clients:
			#print("INFO Writing to client #" + str(index));
			client.sendString(stringToSend);
			index += 1;
		return stringToSend;

	def doSendCrossNodes(self):
		stringToSend = self.service.sendCrossNodes();
		for client in self.clients:
			client.sendString(stringToSend);
		return stringToSend;


def main():
	options = parse_args();

	service = HatmanService();

	factory = HatmanFactory(service);

	factory.clients = []


	# The reactor (= event loop) listens on a port
	port = reactor.listenTCP(options.port or 0, factory, interface=options.iface);

	print("\n------------------------------------------------------------------\n");
	print("Hatman Server running on %s." % (port.getHost(),));
	print("\n------------------------------------------------------------------\n");

	reactor.run();



if __name__ == '__main__':
	main();