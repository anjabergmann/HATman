from __future__ import division
from __future__ import print_function
from __future__ import unicode_literals

import os
import sys
import threading
import time

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

		self.user = args.user;
		self.character = args.character;

		self.touchcount = 0


		# add Layers to scene
		self.labLayer = LabLayer()
		self.pacmanLayer = PacmanLayer()
		self.pacmanLayer.pacmanRect.center = (self.labLayer.crossNodes[0].x, self.labLayer.crossNodes[0].y)
		self.ghostLayerBlue = GhostLayer("blue")
		self.ghostLayerRed = GhostLayer("red")
		self.ghostLayerOrange = GhostLayer("orange")
		self.ghostLayerPink = GhostLayer("pink")
		self.ghostLayers = []
		self.ghostLayers.append(self.ghostLayerBlue)
		self.ghostLayers.append(self.ghostLayerRed)
		self.ghostLayers.append(self.ghostLayerOrange)
		self.ghostLayers.append(self.ghostLayerPink)

		self.add(self.labLayer)
		self.add(self.pacmanLayer)
		for ghostLayer in self.ghostLayers:
			ghostLayer.ghostRect.center = (self.labLayer.crossNodes[0].x, self.labLayer.crossNodes[0].y)
			self.add(ghostLayer)

		self.myLayer = self.pacmanLayer;
		self.myRect = self.pacmanLayer.pacmanRect;

		if(args.character == "p"):
			self.myLayer = self.ghostLayerPink;
			self.myRect = self.ghostLayerPink.ghostRect;
		elif(args.character == "b"):
			self.myLayer = self.ghostLayerBlue;
			self.myRect = self.ghostLayerBlue.ghostRect;
		elif(args.character == "o"):
			self.myLayer = self.ghostLayerOrange;
			self.myRect = self.ghostLayerOrange.ghostRect;
		elif(args.character == "r"):
			self.myLayer = self.ghostLayerRed;
			self.myRect = self.ghostLayerRed.ghostRect;


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
			else:
				for cn in self.crossNodes:
					if self.myRect.center == (cn.x, cn.y):
						if self.pressedKey == key.RIGHT and cn.nodeRight != None:
							self.direction = self.pressedKey
							self.myLayer.direction = self.direction
						elif self.pressedKey == key.LEFT and cn.nodeLeft != None:
							self.direction = self.pressedKey
							self.myLayer.direction = self.direction
						elif self.pressedKey == key.UP and cn.nodeUp != None:
							self.direction = self.pressedKey
							self.myLayer.direction = self.direction
						elif self.pressedKey == key.DOWN and cn.nodeDown != None:
							self.direction = self.pressedKey
							self.myLayer.direction = self.direction

	# _________________________________________________________________________________________
	#
	# Stand still
	# _________________________________________________________________________________________

	# Check if pacman reaches border or blind end
	# = if pacman reaches a node where neither "direction" nor "pressedKey" is a possible option
	def checkBorders(self):
		for cNode in self.labLayer.crossNodes:
			# only check if pacman reaches a crossNode (blind ends HAVE to be crossNodes)
			if self.myRect.center == (cNode.x, cNode.y):
				if self.myLayer.direction == key.RIGHT:
					if cNode.nodeRight == None:
						self.myLayer.direction = None
				elif self.myLayer.direction == key.LEFT:
					if cNode.nodeLeft == None:
						self.myLayer.direction = None
				elif self.myLayer.direction == key.UP:
					if cNode.nodeUp == None:
						self.myLayer.direction = None
				elif self.myLayer.direction == key.DOWN:
					if cNode.nodeDown == None:
						self.myLayer.direction = None

	# _________________________________________________________________________________________
	#
	# Eat dots
	# _________________________________________________________________________________________

	# Remove wayNodes and wayNodeSprites if pacman reaches them
	# If all dots are eaten Pacman wins
	def eatDots(self):
		for nodeSprite in self.labLayer.nodeSprites:
			if self.myRect.center == (nodeSprite.x, nodeSprite.y):
				self.labLayer.remove(nodeSprite)
				self.labLayer.nodeSprites.remove(nodeSprite)
				self.myLayer.updateScore(1)

		if (len(self.labLayer.nodeSprites)<1):
			print("pacman wins")
			#TODO: display some message
			exit() # or start new level

	# _________________________________________________________________________________________
	#
	# Update method (called on every new frame)
	# _________________________________________________________________________________________

	def update(self, director):
		if (self.myLayer == self.pacmanLayer):
			self.eatDots()

		#if any ghost touches pacman
		for ghost in self.ghostLayers:
			if(ghost.ghostRect.center == self.pacmanLayer.pacmanRect.center):
			#every touch loses pacman one life
				self.touchcount += 1
				if (self.touchcount > 20):
					#don't take a life for every frame they touch (entprellen)
					self.pacmanLayer.lives = self.pacmanLayer.lives-1
					#reset touchcount
					self.touchcount = 0
					#if no lives are dead --> pacman dies
					if self.pacmanLayer.lives == 0:
						#TODO: display some message here
						print("ghosts win")
						exit()

		self.setDirection()
		self.checkBorders()
		self.myLayer.update(director)

		requestString ="\x02move,";
		requestString += args.user + ",";
		requestString += args.character + ",";
		requestString += "1,1,1\x03";

		#print(requestString);

		factory.connectedProtocol.sendRequest(requestString);



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


	print("\n------------------------------------------------------------------\n");
	print("INFO HatmanClient started.");


	#command = "\x02move,user,gameid,character,positionx,positiony\x03"
	init = "\x02hi,Hello Server!\x03"
	command = "\x02move,sheld0r,1,pacman,123,321\x03"
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
			factory.connectedProtocol.sendRequest("xMiau,Miox");
		def fail(err):
			print("ERRBACK Initial sending failed", file=sys.stderr);
			print(err);
			return init;
		return d.addCallbacks(notfail, fail);


	tryToSend(init);



	thread = networkThread();
	thread.daemon = True
	thread.start();



	director.init(resizable=False, caption="HATman")
	# director.window.set_fullscreen(True)
	director.run(GameScene())
