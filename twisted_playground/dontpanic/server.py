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
		if(command[0] == "move"):
			print(command[1] + " is moving");
		else:
			print("Hello World!");
		return "Hello World!".encode("utf-8");



class HatmanProtocol(NetstringReceiver):

	def __init__(self):
		print("HatmanProtocol Init");

	def stringReceived(self, request):
		if ',' not in request.decode("utf-8"): #bad request
			# self.transport.loseConnection();
			return;

		command = request.decode("utf-8")[1:-1].split(",");
		# ... to something with command ...

		print("Received command.");
		print(command);

		self.sendString(self.factory.doSomeFancyMethod(command));

		# self.transport.loseConnection();


class HatmanFactory(ServerFactory):

	protocol = HatmanProtocol;

	def __init__(self, service):
		self.service = service
		print("HatmanFactory Init");

	def doSomeFancyMethod(self, command):
		print("Blablablubberfasel");
		return self.service.someFancyMethod(command);


def main():
	options = parse_args();

	service = HatmanService();

	factory = HatmanFactory(service);

	port = reactor.listenTCP(options.port or 0, factory, interface=options.iface);

	print("Hatman Server running on %s." % (port.getHost(),));

	reactor.run();



if __name__ == '__main__':
	main();