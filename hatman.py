from __future__ import division
from __future__ import print_function
from __future__ import unicode_literals

import os
import sys
import threading
import time
import datetime

from pyglet.gl import *
from pyglet.window import key

from cocos.director import director
from cocos.scene import Scene
from cocos.text import Label

from layers.lab import LabLayer
from layers.pacman import PacmanLayer
from layers.ghost import GhostLayer

import client
from client import HatmanClientProtocol
from client import HatmanClientFactory

import parse

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))


# Contains all needed Layers
class GameScene(Scene):
	def __init__(self):
		super().__init__() 

		# variables for measuring time (for debbuging purposes)
		self.starttime = datetime.datetime.now();
		self.now = datetime.datetime.now();
		self.duration = self.now - self.starttime;


		self.user = args.user;              # username
		self.character = args.character;    # character the user is playing


		self.pressedKey = None
		self.direction = key.RIGHT


		# create char Layers
		self.labLayer = LabLayer()
		self.pacmanLayer = PacmanLayer()
		self.ghostLayerBlue = GhostLayer("blue")
		self.ghostLayerRed = GhostLayer("red")
		self.ghostLayerOrange = GhostLayer("orange")
		self.ghostLayerPink = GhostLayer("pink")


		self.charLayers = []    # list with all five character layers
		self.others = []        # list with char layers that are not played by this user

		self.charLayers.append(self.pacmanLayer)
		self.charLayers.append(self.ghostLayerBlue)
		self.charLayers.append(self.ghostLayerOrange)
		self.charLayers.append(self.ghostLayerPink)
		self.charLayers.append(self.ghostLayerRed)


#------------------------------------------------------------
# label for score and lives
		self.statslabel = Label('Score: {}\t\t\t Lives: {}'.format(self.pacmanLayer.score, self.pacmanLayer.lives), 
		   font_name='Arial', 
		   font_size=16, anchor_x='center', 
		   anchor_y='center')
		  # set the title-label at the top center of the screen
		self.statslabel.position = 320,460
		self.add(self.statslabel)
#------------------------------------------------------------

		# add layers to the scene
		self.add(self.labLayer)

		# choose pacman position 'randomly'
		self.pacmanLayer.charRect.center = (self.labLayer.crossNodes[len(self.labLayer.crossNodes)-1].x, self.labLayer.crossNodes[len(self.labLayer.crossNodes)-1].y)
		self.add(self.pacmanLayer)

		for i in range(1,len(self.charLayers)):
			self.charLayers[i].charRect.center = (self.labLayer.crossNodes[0].x, self.labLayer.crossNodes[0].y)
			self.add(self.charLayers[i]);


		# set myLayer to the layer of the users character
		self.myLayer = self.pacmanLayer;

		if(args.character == "p"):
			self.myLayer = self.ghostLayerPink;
		elif(args.character == "b"):
			self.myLayer = self.ghostLayerBlue;
		elif(args.character == "o"):
			self.myLayer = self.ghostLayerOrange;
		elif(args.character == "r"):
			self.myLayer = self.ghostLayerRed;


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




	def updateChars(self, info):
		self.now = datetime.datetime.now();
		self.duration = self.now - self.starttime;
		print(self.duration);
		infolist = info.decode("utf-8").split(",");
		if (infolist[0] == "move"):
			char = infolist[3];
			posx = float(infolist[4]);
			posy = float(infolist[5]);
			key = int(infolist[6]);
			print("update", char);
			if(char == "pac" and self.myLayer != self.pacmanLayer):
				self.pacmanLayer.charRect.position = posx, posy;
				self.pacmanLayer.pacman1.position = self.pacmanLayer.charRect.center;
				self.pacmanLayer.pacman2.position = self.pacmanLayer.charRect.center;
			elif (char == "o" and self.myLayer != self.ghostLayerOrange):
				self.ghostLayerOrange.charRect.position = posx, posy;
				self.ghostLayerOrange.ghost1.position = self.ghostLayerOrange.charRect.center;
				self.ghostLayerOrange.ghost2.position = self.ghostLayerOrange.charRect.center;
			elif (char == "p" and self.myLayer != self.ghostLayerPink):
				self.ghostLayerPink.charRect.position = posx, posy;
				self.ghostLayerPink.ghost1.position = self.ghostLayerPink.charRect.center;
				self.ghostLayerPink.ghost2.position = self.ghostLayerPink.charRect.center;
			elif (char == "r" and self.myLayer != self.ghostLayerRed):
				self.ghostLayerRed.charRect.position = posx, posy;
				self.ghostLayerRed.ghost1.position = self.ghostLayerRed.charRect.center;
				self.ghostLayerRed.ghost2.position = self.ghostLayerRed.charRect.center;
			elif (char == "b" and self.myLayer != self.ghostLayerBlue):
				self.ghostLayerBlue.charRect.position = posx, posy;
				self.ghostLayerBlue.ghost1.position = self.ghostLayerBlue.charRect.center;
				self.ghostLayerBlue.ghost2.position = self.ghostLayerBlue.charRect.center;
		elif(infolist[0] == "changeDirection"):
			print("{} changed direction".format(char));
			if(char == "pac" and self.myLayer != self.pacmanLayer):
				self.pacmanLayer.direction = key;
			elif(char == "o" and self.myLayer != self.ghostLayerOrange):
				self.ghostLayerOrange.direction = key;
			elif(char == "p" and self.myLayer != self.ghostLayerPink):
				self.ghostLayerPink.direction = key;
			elif(char == "r" and self.myLayer != self.ghostLayerRed):
				self.ghostLayerRed.direction = key;
			elif(char == "b" and self.myLayer != self.ghostLayerBlue):
				self.ghostLayerBlue.direction = key;

	# _________________________________________________________________________________________
	#
	# Update method (called on every new frame)
	# _________________________________________________________________________________________

	def update(self, director):
		self.eatDots()
		if(self.setDirection()):
			self.starttime = datetime.datetime.now();
			requestString="\x02changeDirection," + args.user + ",1," + args.character + "," + str(self.myLayer.direction) + "\x03";
			factory.connectedProtocol.sendRequest(requestString);
		self.checkBorders()
		for char in self.charLayers:
			char.update(director);


		# #command = "\x02move,user,gameid,character,positionx,positiony\x03"
		# requestString ="\x02move,";
		# requestString += args.user + ",1,";
		# requestString += args.character + ",";
		# requestString += str(self.myLayer.charRect.x) + "," + str(self.myRect.y) + "\x03";

		# #print(requestString);

		# factory.connectedProtocol.sendRequest(requestString);



class networkThread(threading.Thread):
	def __init__(self):
		threading.Thread.__init__(self);
		print("networkThread");

	def run(self):
		try:
			client.reactor.run(installSignalHandlers=0);
		except:
			client.reactor.run();






if __name__ == "__main__":

	args = parse.parseArgs();
	director.init(resizable=False, caption="HATman")
	# director.window.set_fullscreen(True)
	game = GameScene();


	print("\n------------------------------------------------------------------\n");
	print("INFO HatmanClient started.");


	init = "\x02hi,Hello Server!\x03"
	host = args.host;
	port = args.port;


	factory = HatmanClientFactory(init)
	client.reactor.connectTCP(host, port, factory)
	print("INFO Connected to server {}:{}".format(host, port));
	print("\n------------------------------------------------------------------\n");


	d = factory.deferred;


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



	thread = networkThread();
	thread.daemon = True
	thread.start();




	director.run(game)

