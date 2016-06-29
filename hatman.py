from __future__ import division, print_function, unicode_literals
import os, sys, threading, time, datetime

from pyglet.gl import *
from pyglet.window import key

from cocos.director import director
from cocos.scene import Scene

from layers.lab import LabLayer
from layers.pacman import PacmanLayer
from layers.ghost import GhostLayer

from helper import parse, client
from helper.client import HatmanClientProtocol, HatmanClientFactory


sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))


args = parse.parseArgs()
host = args.host
port = args.port
user = args.user
character = args.character

init = "\x02hi,Hello Server!\x03"

factory = HatmanClientFactory(init)
d = factory.deferred



# Contains all needed Layers
class GameScene(Scene):
	def __init__(self):
		super().__init__() 

		self.counter = 0;	# counts number of received commands in updateChars()
		self.turn = True;	# variable that checks if we should send a command to server; if True: send command; if False: we already sent a command and wait until we received for other commanads (from the other players) before we sent an own command again
		self.commands = [];	# list of received commands for every turn (= always contains 0 to 5 commands)

		self.bufferRed = [];
		self.bufferPink = [];
		self.bufferOrange = [];
		self.bufferBlue = [];
		self.bufferPac = [];
		self.turns = {"r":100, "p":100, "o":100, "b":100, "pac":100};

		# variables for measuring time (for debbuging purposes)
		self.starttime = datetime.datetime.now();
		self.now = datetime.datetime.now();
		self.duration = self.now - self.starttime;


		self.pressedKey = None
		self.direction = key.RIGHT


		# add Layers to scene
		self.labLayer = LabLayer()
		self.pacmanLayer = PacmanLayer()
		self.ghostLayerBlue = GhostLayer("blue")
		self.ghostLayerRed = GhostLayer("red")
		self.ghostLayerOrange = GhostLayer("orange")
		self.ghostLayerPink = GhostLayer("pink")


		self.charLayers = []	# list with all five character layers
		self.others = []		# list with char layers that are not played by this user

		self.charLayers.append(self.pacmanLayer)
		self.charLayers.append(self.ghostLayerBlue)
		self.charLayers.append(self.ghostLayerOrange)
		self.charLayers.append(self.ghostLayerPink)
		self.charLayers.append(self.ghostLayerRed)

		self.others = self.charLayers;


		# add layers to the scene
		self.add(self.labLayer)
		for char in self.charLayers:
			char.charRect.center = (self.labLayer.crossNodes[0].x, self.labLayer.crossNodes[0].y)
			self.add(char);


		self.charMapping = {"r":self.ghostLayerRed, "b":self.ghostLayerBlue, "p":self.ghostLayerPink, "o":self.ghostLayerOrange, "pac":self.pacmanLayer}


		# set myLayer to the layer of the users character
		self.myLayer = self.charMapping.get(character);

		self.myLayer = self.pacmanLayer;

		self.others.remove(self.myLayer)


		# add schedule method
		self.schedule(self.update)


	#---------------------- init end ---------------------------------



	# _________________________________________________________________________________________
	#
	# Change direction
	# _________________________________________________________________________________________

	# Change the direction to pressedKey if it is possible
	# (= if Character reaches a node with a neighbor-node in the pressedKey-direction)
	# return true = direction changed; false = direction didn't change
	def setDirection(self):

		#idea for refactoring: 
		# if (positionx und positiony mod 20 = 0 (wenn überhaupt an einem Knoten))
		#		if knotenArray[positionx/20 - 1][positionny/20 -1] ist crossNode and
		#					knoten hat nachbarknoten in entsprechender Richtung):
		#							wechsle Richtung

		self.pressedKey = self.myLayer.pressedKey
		if self.pressedKey != self.direction:
			# invert direction --> always possible
			if (self.pressedKey == key.RIGHT and self.direction == key.LEFT) or (
							self.pressedKey == key.LEFT and self.direction == key.RIGHT) or (
							self.pressedKey == key.DOWN and self.direction == key.UP) or (
							self.pressedKey == key.UP and self.direction == key.DOWN):
				self.direction = self.pressedKey
				self.myLayer.direction = self.direction
				return True;
			else:
				for cn in self.labLayer.crossNodes:
					if self.myLayer.charRect.center == (cn.x, cn.y):
						if self.pressedKey == key.RIGHT and cn.nodeRight != None:
							self.direction = self.pressedKey
							self.myLayer.direction = self.direction
							return True;
						elif self.pressedKey == key.LEFT and cn.nodeLeft != None:
							self.direction = self.pressedKey
							self.myLayer.direction = self.direction
							return True;
						elif self.pressedKey == key.UP and cn.nodeUp != None:
							self.direction = self.pressedKey
							self.myLayer.direction = self.direction
							return True;
						elif self.pressedKey == key.DOWN and cn.nodeDown != None:
							self.direction = self.pressedKey
							self.myLayer.direction = self.direction
							return True;
		return False;



	# _________________________________________________________________________________________
	#
	# Stand still
	# _________________________________________________________________________________________

	# Check if char reaches border or blind end
	# = if char reaches a node where neither "direction" nor "pressedKey" is a possible option
	def checkBorders(self):
		for cNode in self.labLayer.crossNodes:
			# only check if char reaches a crossNode (blind ends HAVE to be crossNodes)
			for char in self.charLayers:
				if (char.charRect.center == (cNode.x, cNode.y)):
					if char.direction == key.RIGHT:
						if cNode.nodeRight == None:
							char.direction = None
					elif char.direction == key.LEFT:
						if cNode.nodeLeft == None:
							char.direction = None
					elif char.direction == key.UP:
						if cNode.nodeUp == None:
							char.direction = None
					elif char.direction == key.DOWN:
						if cNode.nodeDown == None:
							char.direction = None

	# _________________________________________________________________________________________
	#
	# Eat dots
	# _________________________________________________________________________________________

	# Remove wayNodes and wayNodeSprites if pacman reaches them
	def eatDots(self):
		for nodeSprite in self.labLayer.nodeSprites:
			if self.pacmanLayer.charRect.center == (nodeSprite.x, nodeSprite.y):
				self.labLayer.remove(nodeSprite)
				self.labLayer.nodeSprites.remove(nodeSprite)
				self.pacmanLayer.updateScore(1)




	def updateChars(self, info):

		self.counter += 1; # increase number of received commands
		self.commands.append(info); # add received command to commandlist


		# number of commands received
		# TODO: Replace counter with len(commandlist)
		if (self.counter == 5):

			self.counter = 0;
			self.turn = True;

			for c in self.commands:

				commandlist = c.decode("utf-8")[1:-1].split(",");
				print("DEBUG Commandlist:", commandlist);
				if(commandlist[0] == "move"):
					char = commandlist[3];
					posx = float(commandlist[4]);
					posy = float(commandlist[5]);

					self.charMapping.get(char).setPosition(director, posx, posy);

			self.commands = [];

	# _________________________________________________________________________________________
	#
	# Update method (called on every new frame)
	# _________________________________________________________________________________________

	def update(self, director):

		#print("DEBUG update()");

		#if (self.turns.get(character) > 0):
		if(self.turn):
			self.turns.__setitem__(character, (self.turns.get(character) - 1))
			#print("DEBUG update() if (self.turn)");
			self.turn = False

			self.eatDots()
			self.setDirection()
			self.checkBorders()

			# if(self.setDirection()):
			# 	self.starttime = datetime.datetime.now();
				# requestString="\x02changeDirection," + args.user + ",1," + character + "," + str(self.myLayer.direction) + "\x03";
				# factory.connectedProtocol.sendRequest(requestString);

			self.myLayer.update(director);

			#if(self.myLayer.update(director)):
			#	print(datetime.datetime.now())

			# for char in self.others:
			# 	char.update(director)


			#command = "\x02move,user,gameid,character,positionx,positiony\x03"
			requestString ="\x02move,";
			requestString += args.user + ",1,";
			requestString += args.character + ",";
			requestString += str(self.myLayer.charRect.x) + "," + str(self.myLayer.charRect.y) + "\x03";

			#print("DEBUG RequestString:", requestString);

			factory.connectedProtocol.sendRequest(requestString);





class networkThread(threading.Thread):
	def __init__(self):
		threading.Thread.__init__(self);

	def run(self):
		try:
			client.reactor.run(installSignalHandlers=0);
		except:
			client.reactor.run();



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

	def newDeferred():

		def doCallback(data):
			#print("CALLBACKCALLBACK");
			d = factory.deferred;
			d.addCallback(game.updateChars);
			d.addCallback(doCallback);
		return d.addCallback(doCallback);


	tryToSend(init);
	newDeferred();



	# start the reactor for the networking stuff
	thread = networkThread();
	thread.daemon = True
	thread.start();

	# start the director for the gui stuff
	director.run(game)

# ------------------- end of main() -------------------------------



if __name__ == "__main__":
	main();