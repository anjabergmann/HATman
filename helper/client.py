import optparse;
import sys;
import time;

from twisted.internet import reactor;
from twisted.internet import defer;
from twisted.internet.protocol import Protocol, ClientFactory;
from twisted.protocols.basic import NetstringReceiver;


def parse_args():
	usage = """usage: %prog [options] [hostname]:port ..."""

	parser = optparse.OptionParser(usage)

	_, addresses = parser.parse_args()

	if len(addresses) < 1:
		print(parser.format_help())
		parser.exit()

	def parse_address(addr):
		if ':' not in addr:
			host = '127.0.0.1'
			port = addr
		else:
			host, port = addr.split(':', 1)

		if not port.isdigit():
			parser.error('Ports must be integers.')

		return host, int(port)

	return list(map(parse_address, addresses))



class HatmanClientProtocol(NetstringReceiver):

	def connectionMade(self):
		self.sendRequest(self.factory.command)


	def sendRequest(self, command):
		#print("INFO Sending data to server:", command);
		self.sendString(command.encode("utf-8"));


	def stringReceived(self, command):
		# self.transport.loseConnection();
		#print("INFO Received data from server:", command.decode("utf-8"));
		self.factory.handleString(command);




class HatmanClientFactory(ClientFactory):

	protocol = HatmanClientProtocol;

	def __init__(self, command):
		self.command = command;
		self.deferred = defer.Deferred();

	def buildProtocol(self, address):
		proto = ClientFactory.buildProtocol(self, address);
		self.connectedProtocol = proto;
		return proto;


	def handleString(self, command):
		d, self.deferred = self.deferred, defer.Deferred();
		d.callback(command);


	def clientConnectionLost(self, _, reason):
		if self.deferred is not None:
			d, self.deferred = self.deferred, None;
			d.errback(reason);

	clientConnectionFailed = clientConnectionLost



def hatmanMain():

	print("\n------------------------------------------------------------------\n");
	print("INFO HatmanClient started.");


	#command = "\x02move,user,gameid,character,positionx,positiony\x03"
	command = "\x02move,sheld0r,1,pacman,123,321\x03"
	addresses = parse_args();
	address = addresses.pop(0);
	host, port = address;


	factory = HatmanClientFactory(command)
	reactor.connectTCP(host, port, factory)
	print("INFO Connected to server {}:{}".format(host, port));
	print("\n------------------------------------------------------------------\n");


	d = factory.deferred;


	def tryToSend(command):
		print("INFO Sending data to server", command);

		def notfail(data):
			print("INFO nofail");
			factory.connectedProtocol.sendRequest("xMiau,Miox");
		def fail(err):
			print("ERROR Sending failed", file=sys.stderr);
			print(err);
			return command;
		return d.addCallbacks(notfail, fail);


	tryToSend(command);



	#reactor.run(installSignalHandlers=0);
	reactor.run();


if __name__ == '__main__':
	hatmanMain();