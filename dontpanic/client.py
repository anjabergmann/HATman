import optparse, sys

from twisted.internet import reactor
from twisted.internet import defer
from twisted.internet.protocol import Protocol, ClientFactory
from twisted.protocols.basic import NetstringReceiver


def parse_args():
	usage = """usage: %prog [options] [hostname]:port ...

This is the Get Poetry Now! client, Twisted version 6.0
Run it like this:

  python get-poetry.py xform-port port1 port2 ...

If you are in the base directory of the twisted-intro package,
you could run it like this:

  python twisted-client-6/get-poetry.py 10001 10002 10003

to grab poetry from servers on ports 10002, and 10003 and transform
it using the server on port 10001.

Of course, there need to be appropriate servers listening on those
ports for that to work.
"""

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
		self.sendRequest(self.factory.command.encode("utf-8"))


	def sendRequest(self, command):
		self.sendString(command);


	def stringReceived(self, command):
		# self.transport.loseConnection();
		print("INFO Received data from server:", command.decode("utf-8"));
		self.factory.handleString(command);




class HatmanClientFactory(ClientFactory):

	protocol = HatmanClientProtocol;

	def __init__(self, command):
		self.command = command;
		self.deferred = defer.Deferred();

	def handleString(self, command):
		d, self.deferred = self.deferred, None;
		#d.callback(command);

	def clientConnectionLost(self, _, reason):
		if self.deferred is not None:
			d, self.deferred = self.deferred, None;
			d.errback(reason);

	clientConnectionFailed = clientConnectionLost


class HatmanProxy(object):
	"""
	I proxy requests to a transformation service.
	"""

	def __init__(self, host, port):
		self.host = host
		self.port = port

	def xform(self, command):
		factory = HatmanClientFactory(command)
		reactor.connectTCP(self.host, self.port, factory)
		print("INFO Connected to server {}:{}".format(self.host, self.port));
		print("\n------------------------------------------------------------------\n");
		return factory.deferred



def hatmanMain():


	print("\n------------------------------------------------------------------\n");
	print("INFO HatmanClient started.");

	addresses = parse_args();
	address = addresses.pop(0);

	#command = "\x02command,param1,param2,param3\x03"
	#move = "\x02move,user,gameid,character,positionx,positiony\x03"
	#die = "\x02die,user,gameid,scorepacman,scoreghosts\x03"
	command = "\x02move,sheld0r,param2,param3\x03"

	proxy = HatmanProxy(*address);

	d = proxy.xform(command);

	def try_to_send(command):
		print("INFO Sending data to server", command);

		def fail(err):
			print("ERROR Sending failed", file=sys.stderr);
			print(err);
			return command;
		return d.addErrback(fail);

	host, port = address;
	try_to_send(command);

	reactor.run();



if __name__ == '__main__':
	hatmanMain();