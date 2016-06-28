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

		self.starttime = datetime.datetime.now();
		self.now = datetime.datetime.now();
		self.duration = self.now - self.starttime;

		self.user = args.user;
		self.character = args.character;


		# add Layers to scene
		self.labLayer = LabLayer()
		self.pacmanLayer = PacmanLayer()
		self.pacmanLayer.charRect.center = (self.labLayer.crossNodes[0].x, self.labLayer.crossNodes[0].y)
		self.ghostLayerBlue = GhostLayer("blue")
		self.ghostLayerRed = GhostLayer("red")
		self.ghostLayerOrange = GhostLayer("orange")
		self.ghostLayerPink = GhostLayer("pink")
		self.charLayers = [] # list with all five character layer
		self.charLayers.append(self.pacmanLayer)
		self.ghostLayers = [] # list with all four ghost layers
		self.ghostLayers.append(self.ghostLayerBlue)
		self.ghostLayers.append(self.ghostLayerRed)
		self.ghostLayers.append(self.ghostLayerOrange)
		self.ghostLayers.append(self.ghostLayerPink)

		self.add(self.labLayer)
		self.add(self.pacmanLayer)
		for ghostLayer in self.ghostLayers:
			ghostLayer.charRect.center = (self.labLayer.crossNodes[0].x, self.labLayer.crossNodes[0].y)
			self.add(ghostLayer)
			self.charLayers.append(ghostLayer);

		self.myLayer = self.pacmanLayer;
		self.myRect = self.pacmanLayer.charRect;


		if(args.character == "p"):
			self.myLayer = self.ghostLayerPink;
			self.myRect = self.ghostLayerPink.charRect;
		elif(args.character == "b"):
			self.myLayer = self.ghostLayerBlue;
			self.myRect = self.ghostLayerBlue.charRect;
		elif(args.character == "o"):
			self.myLayer = self.ghostLayerOrange;
			self.myRect = self.ghostLayerOrange.charRect;
		elif(args.character == "r"):
			self.myLayer = self.ghostLayerRed;
			self.myRect = self.ghostLayerRed.charRect;


		# add schedule method
		self.schedule(self.update)

		self.pressedKey = None
		self.direction = key.RIGHT

		self.crossNodes = self.labLayer.crossNodes


	# _________________________________________________________________________________________
	#
	# Change direction
	# _________________________________________________________________________________________

	# Change the direction to pressedKey if it is possible
	# (= if Character reaches a node with a neighbor-node in the pressedKey-direction)
	# return true = direction changed; false = direction didn't change
	def setDirection(self):
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
				for cn in self.crossNodes:
					if self.myRect.center == (cn.x, cn.y):
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
		self.checkBorders() #TODO: Check borders for every char
		self.pacmanLayer.update(director)
		for gL in self.ghostLayers:
			gL.update(director);


		# #command = "\x02move,user,gameid,character,positionx,positiony\x03"
		# requestString ="\x02move,";
		# requestString += args.user + ",1,";
		# requestString += args.character + ",";
		# requestString += str(self.myRect.x) + "," + str(self.myRect.y) + "\x03";

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
