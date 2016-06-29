from cocos.director import director
from cocos.scene import Scene

def main():

	director.init(resizable=False, caption="HATman")
	# director.window.set_fullscreen(True)
	game = GameScene();



	print("\n------------------------------------------------------------------\n");
	print("INFO HatmanClient started.");

	client.reactor.connectTCP(host, port, factory)

	print("INFO Connected to server {}:{}".format(host, port));
	print("\n------------------------------------------------------------------\n");



	def tryToSend(init):
		print("INFO Sending initial data to server", init);

		def notfail(data):
			print("CALLBACK Initial sending succeded.");
			#factory.connectedProtocol.sendRequest("xMiau,Miox");
		def fail(err):
			print("ERRBACK Initial sending failed", file=sys.stderr);
			print(err);
			return init;
		return d.addCallbacks(notfail, fail);

	def doSomething():

		def doCallback(data):
			#print("CALLBACKCALLBACK");
			d = factory.deferred;
			d.addCallback(game.updateChars);
			d.addCallback(doCallback);
		return d.addCallback(doCallback);


	tryToSend(init);
	doSomething();



	# start the reactor for the networking stuff
	thread = networkThread();
	thread.daemon = True
	thread.start();

	# start the director for the gui stuff
	director.run(game)

