import optparse

from twisted.internet import reactor
from twisted.internet.protocol import ServerFactory
from twisted.protocols.basic import NetstringReceiver

def parse_args():
	usage = """usage: %prog [options]

This is the Poetry Transform Server.
Run it like this:

  python transformedpoetry.py

If you are in the base directory of the twisted-intro package,
you could run it like this:

  python twisted-server-1/transformedpoetry.py --port 11000

to provide poetry transformation on port 11000.
"""

	parser = optparse.OptionParser(usage)

	help = "The port to listen on. Default to a random available port."
	parser.add_option('--port', type='int', help=help)

	help = "The interface to listen on. Default is localhost."
	parser.add_option('--iface', help=help, default='localhost')

	options, args = parser.parse_args()

	if len(args) != 0:
		parser.error('Bad arguments.')

	return options;



class HatmanService(object):

	def someFancyMethod(self, command):
		#print("INFO Doing someFancyMethod with command", command);
		returnString = "";
		if(command[0] == "move"):
			#character is moving
			#todo
			returnString = str(command);
		elif(command[0] == "die"):
			#character is dying
			#todo
			returnString = str(command);
		else:
			#another command
			#todo
			returnString = "Hello World!".encode("utf-8");
		try:
			return returnString.encode("utf-8");
		except:
			return returnString;


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

		#self.sendString(self.factory.doSomeFancyMethod(command));
		self.factory.doSomeFancyMethod(command);




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