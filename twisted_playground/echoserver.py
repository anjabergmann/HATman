from twisted.internet import protocol, reactor, endpoints
from config import port

class Echo(protocol.Protocol):
	def dataReceived(self, data):
		self.transport.write(data)

class EchoFactory(protocol.Factory):
	def buildProtocol(self, addr):
		return Echo()

endpoints.serverFromString(reactor, "tcp:" + port).listen(EchoFactory())
reactor.run()