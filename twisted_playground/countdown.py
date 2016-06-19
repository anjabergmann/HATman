from twisted.internet import reactor;

class Countdown(object):
	counter = 5;

	def count(self):
		if self.counter == 0:
			reactor.stop();
		else:
			print (self.counter);
			self.counter -= 1;
			# The first argument of callLater is the number of seconds in the future where the callback will run
			# The second argument is the callback function itself
			reactor.callLater(1, self.count);

reactor.callWhenRunning(Countdown().count);
reactor.callLater(0.33, Countdown().count);
reactor.callLater(0.66, Countdown().count);


print ("start");
reactor.run();
print ("stop");