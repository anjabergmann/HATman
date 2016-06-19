import traceback;
from twisted.internet import reactor;

def hello():
	print("Hello from the reactor loop!");
	print("The python stack:");
	traceback.print_stack();


#define hello as a callback function that is executed as soon as the reactor starts running
reactor.callWhenRunning(hello);


print("Starting the reactor.");
reactor.run();