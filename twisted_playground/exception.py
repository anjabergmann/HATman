from twisted.internet import reactor;

def falldown():
	raise Exception("I fall down.");

def upagain():
	print("But i get up again.");
	reactor.stop();

# After the exception ...
reactor.callWhenRunning(falldown);

# ... the second callback is executed anyway
# And the moral of this story:
# The reactor keeps going, even if a callback function fails
reactor.callWhenRunning(upagain);


print("Starting the reactor.");
reactor.run();
