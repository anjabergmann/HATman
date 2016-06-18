from twisted.web import server, resource
from twisted.internet import reactor, endpoints #Reactor= Twisted event loop within Twisted
from config import port, protocol


print("INFO Server running on port " + port);

class Counter(resource.Resource):
	isLeaf = True
	numberRequests = 0

	def render_GET(self, request):
		print("INFO Server received a GET request");
		self.numberRequests += 1
		request.setHeader(b"content-type", b"text/plain")
		# content = u"I am request #{}\n".format(self.numberRequests)
		# return content.encode("ascii")
		print(request);
		return(request);

	def render_POST(self, request):
		print("INFO Server received a POST request");
		self.numberRequests += 1
		request.setHeader(b"content-type", b"text/plain")
		# content = u"I am request #{}\n".format(self.numberRequests)
		# return content.encode("ascii")
		print(request);
		return(request);

endpoints.serverFromString(reactor, protocol + ":" + port).listen(server.Site(Counter()))
reactor.run()