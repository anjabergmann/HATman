from __future__ import division, print_function, unicode_literals
import os, sys, threading, time, datetime

from pyglet.gl import *
from pyglet.window import key

from cocos.director import director
from cocos.scene import Scene
from cocos.text import Label

from layers.lab import LabLayer
from layers.pacman import PacmanLayer
from layers.ghost import GhostLayer

from helper import parse, client
from helper.client import HatmanClientProtocol, HatmanClientFactory
from helper.node import LabNode


sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))


args = parse.parseArgs()
host = args.host
port = args.port
user = args.user
character = args.character

init = "\x02hi,Hello Server!\x03"

factory = HatmanClientFactory(init)
d = factory.deferred

serverNodes = []



# Contains all needed Layers
class GameScene(Scene):
	def __init__(self):
		super().__init__() 

		self.turn = True;	# variable that checks if we should send a command to server; if True: send command; if False: we already sent a command and wait until we received for other commanads (from the other players) before we sent an own command again
		self.commands = [];	# list of received commands for every turn (= always contains 0 to 5 commands except if the server sends nodes)

		self.turns = {"r":100, "p":100, "o":100, "b":100, "pac":100};

		# variables for measuring time (for debbuging purposes)
		self.starttime = datetime.datetime.now();
		self.now = datetime.datetime.now();
		self.duration = self.now - self.starttime;


		self.user = args.user;              # username
		self.character = args.character;    # character the user is playing


		self.pressedKey = None
		self.direction = key.RIGHT

		#---------------------------------------------------------------------------------------------
		# create lablayer with nodes from server
		print(serverNodes)
		self.labLayer = LabLayer(serverNodes)

		#---------------------------------------------------------------------------------------------
		# create char Layers
		self.pacmanLayer = PacmanLayer()
		self.ghostLayerBlue = GhostLayer("blue")
		self.ghostLayerRed = GhostLayer("red")
		self.ghostLayerOrange = GhostLayer("orange")
		self.ghostLayerPink = GhostLayer("pink")

		#---------------------------------------------------------------------------------------------
		self.charLayers = []    # list with all five character layers
		self.others = []        # list with char layers that are not played by this user
		
		#---------------------------------------------------------------------------------------------
		# add character layers to lists
		self.charLayers.append(self.pacmanLayer)
		self.charLayers.append(self.ghostLayerBlue)
		self.charLayers.append(self.ghostLayerOrange)
		self.charLayers.append(self.ghostLayerPink)
		self.charLayers.append(self.ghostLayerRed)

		self.others = self.charLayers;

		#---------------------------------------------------------------------------------------------
		# label for score and lives
		self.statslabel = Label('Score: {}\t\t\t Lives: {}'.format(self.pacmanLayer.score, self.pacmanLayer.lives), 
		   font_name='Arial', 
		   font_size=16, anchor_x='center', 
		   anchor_y='center')
		  # set the title-label at the top center of the screen
		self.statslabel.position = 320,460
		self.add(self.statslabel)

		#---------------------------------------------------------------------------------------------
		# add layers to the scene
		# choose positions 'randomly'
		#self.pacmanLayer.charRect.center = (self.labLayer.crossNodes[len(self.labLayer.crossNodes)-1].x, self.labLayer.crossNodes[len(self.labLayer.crossNodes)-1].y)
		self.add(self.pacmanLayer)

		for i in range(1,len(self.charLayers)):
		#	self.charLayers[i].charRect.center = (self.labLayer.crossNodes[0].x, self.labLayer.crossNodes[0].y)
			self.add(self.charLayers[i]);

		#---------------------------------------------------------------------------------------------
		# set myLayer to the layer of the users character
		self.charMapping = {"r":self.ghostLayerRed, "b":self.ghostLayerBlue, "p":self.ghostLayerPink, "o":self.ghostLayerOrange, "pac":self.pacmanLayer}
		self.myLayer = self.charMapping.get(character);
		self.others.remove(self.myLayer)

		#---------------------------------------------------------------------------------------------
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

		self.pressedKey = self.myLayer.pressedKey

		# if direction and pressed key are the same -> don't do anything
		if self.pressedKey != self.direction:

			# invert direction --> always possible
			if (self.pressedKey == key.RIGHT and self.direction == key.LEFT) or (
					self.pressedKey == key.LEFT and self.direction == key.RIGHT) or (
						self.pressedKey == key.DOWN and self.direction == key.UP) or (
							self.pressedKey == key.UP and self.direction == key.DOWN):
				self.direction = self.pressedKey
				self.myLayer.direction = self.direction
				return True;

			# pressed key is not moving direction and not opposite
			else:
				# current position
				x = self.myLayer.charRect.center[0]
				y = self.myLayer.charRect.center[1]
				
				# change of direction is only possible when standing on a node
				#   -> check if position is a node
				if x % 20 == 0.0 and y % 20 == 0.0:
					
					# get le node and save in var
					node = self.labLayer.nodes[int(x/20)-2][int(y/20)-2]
					
					if node.sort == "cross":
						way = False
						# check if the there is a way in the direction of the pressed key
						if ((self.pressedKey == key.RIGHT and node.nodeRight != None) or
								(self.pressedKey == key.LEFT and node.nodeLeft != None) or
									(self.pressedKey == key.UP and node.nodeUp != None) or
										(self.pressedKey == key.DOWN and node.nodeDown != None)):
							way = True
						# if there is a way, change direction
						if way:
							self.direction = self.pressedKey
							self.myLayer.direction = self.direction
						return way

		return False;



	# _________________________________________________________________________________________
	#
	# Stand still
	# _________________________________________________________________________________________

	# Check if char reaches border or blind end
	# = if char reaches a node where neither "direction" nor "pressedKey" is a possible option
	def checkBorders(self):

		for char in self.charLayers:
			# current position
			x = char.charRect.center[0]
			y = char.charRect.center[1]
		
			# if position is a node
			if (x % 20 == 0.0 and y % 20 == 0.0):

				# get le node and save in var
				node = self.labLayer.nodes[int(x/20)-2][int(y/20)-2]

				# if le node is cossNode and has no neighbour in the desired direction -> STOP
				if node.sort == "cross":
					if (char.direction != None):
						if ((char.direction == key.RIGHT and node.nodeRight == None) or
								(char.direction == key.LEFT and node.nodeLeft == None) or
									(char.direction == key.UP and node.nodeUp == None) or
										(char.direction == key.DOWN and node.nodeDown == None)):
							char.direction = None



	# _________________________________________________________________________________________
	#
	# Eat dots
	# _________________________________________________________________________________________

	# Remove wayNodes and wayNodeSprites if pacman reaches them
	def eatDots(self):
		# current position
			x = self.pacmanLayer.charRect.center[0]
			y = self.pacmanLayer.charRect.center[1]
		
			# if position is a node
			if (x % 20 == 0.0 and y % 20 == 0.0):

				for nodeSprite in self.labLayer.nodeSprites:
					if (x,y) == (nodeSprite.x, nodeSprite.y):
						self.labLayer.remove(nodeSprite)
						self.labLayer.nodeSprites.remove(nodeSprite)
						self.pacmanLayer.updateScore(1)
						self.statslabel.element.text = 'Score: {}\t\t\t Lives: {}'.format(self.pacmanLayer.score, self.pacmanLayer.lives)
						break

# _________________________________________________________________________________________
	#
	# initialize crossnodes
	# _________________________________________________________________________________________


	def initNodes(self, commandlist):
		print("NODEREQUEST")
		# reconstruct array
		for i in range(1,len(commandlist)-1):
			# strip the command of quotes
			command = commandlist[i][2:-1]
			command = command.strip('"')
			
			# extract the coordinates of the node
			coords = command.split(';')
			x = float(coords[0])
			y = float(coords[1])

			serverNodes.append(LabNode(x, y))

		game = GameScene();
		# start the cocos2d director
		director.run(game)


	# _________________________________________________________________________________________
	#
	# update characters
	# _________________________________________________________________________________________


	def updateChars(self, info):

		commandlist = info.decode("utf-8")[1:-1].split(",");
		print(commandlist[0])
		if (commandlist[0] == "nodes"):
			self.initNodes(commandlist)

		elif (commandlist[0] == "move"):

			char = commandlist[3];
			posx = float(commandlist[4]);
			posy = float(commandlist[5]);

			self.charMapping.get(char).setPosition(director, posx, posy);

			#add command to commandBuffer of appropriate character
			self.charMapping.get(info.decode("utf-8")[1:-1].split(",")[3]).commandBuffer.append(info);


			for thing in self.others:
				if (len(thing.commandBuffer) > 0 and self.turns.get(info.decode("utf-8")[1:-1].split(",")[3]) > 0):
					self.turns.__setitem__(info.decode("utf-8")[1:-1].split(",")[3], (self.turns.get(info.decode("utf-8")[1:-1].split(",")[3]) - 1))
					print("DEBUG max:", max(self.turns, key=lambda k: self.turns[k]));
					print("DEBUG min:", min(self.turns, key=lambda k: self.turns[k]));
					commandlist = thing.commandBuffer.pop().decode("utf-8")[1:-1].split(",");
					#print("DEBUG Commandlist:", commandlist);
					char = commandlist[3];
					posx = float(commandlist[4]);
					posy = float(commandlist[5]);

					self.charMapping.get(char).setPosition(director, posx, posy);




	# _________________________________________________________________________________________
	#
	# Update method (called on every new frame)
	# _________________________________________________________________________________________

	def update(self, director):

		if(self.turns.get(character) > 0):
			self.turns.__setitem__(character, (self.turns.get(character) - 1))

			self.eatDots()
			self.setDirection()
			self.checkBorders()
			self.myLayer.update(director);


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
			print("sending node request")
			factory.connectedProtocol.sendRequest("\x02nodes,Please send nodes!\x03")
			newDeferred();
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


	# start the reactor for the networking stuff
	thread = networkThread();
	thread.daemon = True
	thread.start();

	while(len(serverNodes) == 0):
		time.sleep(0.01);

	game.labLayer = LabLayer(serverNodes);
	game.add(game.labLayer);

	# start the director for the gui stuff
	director.run(game)


# ------------------- end of main() -------------------------------



if __name__ == "__main__":
	main();