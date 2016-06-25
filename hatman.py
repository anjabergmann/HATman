from __future__ import division, print_function, unicode_literals

import os
import sys
import threading

from pyglet.gl import *
from pyglet.window import key

from cocos.director import director
from cocos.scene import Scene

from layers.lab import LabLayer
from layers.pacman import PacmanLayer
from layers.ghost import GhostLayer

import client


sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))

#
# Game Scene --> Contains all needed Layers [not yet, but ...]
#
class GameScene(Scene):
	def __init__(self):
		super(GameScene, self).__init__()  # MaKno says: In Python 3, "super.__init__()" us sufficient.

		self.reactor = False;

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
			ghostLayer.ghostRect.center = (self.labLayer.crossNodes[2].x, self.labLayer.crossNodes[2].y)
			self.add(ghostLayer)

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
	# (= if Pacman reaches a node with a neighbor-node in the pressedKey-direction)
	def setDirection(self):
		self.pressedKey = self.pacmanLayer.pressedKey
		if self.pressedKey != self.direction:
			# invert direction --> always possible
			if (self.pressedKey == key.RIGHT and self.direction == key.LEFT) or (
							self.pressedKey == key.LEFT and self.direction == key.RIGHT) or (
							self.pressedKey == key.DOWN and self.direction == key.UP) or (
							self.pressedKey == key.UP and self.direction == key.DOWN):
				self.direction = self.pressedKey
				self.pacmanLayer.direction = self.direction
			else:
				for cn in self.crossNodes:
					if self.pacmanLayer.pacmanRect.center == (cn.x, cn.y):
						if self.pressedKey == key.RIGHT and cn.nodeRight != None:
							self.direction = self.pressedKey
							self.pacmanLayer.direction = self.direction
						elif self.pressedKey == key.LEFT and cn.nodeLeft != None:
							self.direction = self.pressedKey
							self.pacmanLayer.direction = self.direction
						elif self.pressedKey == key.UP and cn.nodeUp != None:
							self.direction = self.pressedKey
							self.pacmanLayer.direction = self.direction
						elif self.pressedKey == key.DOWN and cn.nodeDown != None:
							self.direction = self.pressedKey
							self.pacmanLayer.direction = self.direction

	# _________________________________________________________________________________________
	#
	# Stand still
	# _________________________________________________________________________________________

	# Check if pacman reaches border or blind end
	# = if pacman reaches a node where neither "direction" nor "pressedKey" is a possible option
	def checkBorders(self):
		for cNode in self.labLayer.crossNodes:
			# only check if pacman reaches a crossNode (blind ends HAVE to be crossNodes)
			if self.pacmanLayer.pacmanRect.center == (cNode.x, cNode.y):
				if self.pacmanLayer.direction == key.RIGHT:
					if cNode.nodeRight == None:
						self.pacmanLayer.direction = None
				elif self.pacmanLayer.direction == key.LEFT:
					if cNode.nodeLeft == None:
						self.pacmanLayer.direction = None
				elif self.pacmanLayer.direction == key.UP:
					if cNode.nodeUp == None:
						self.pacmanLayer.direction = None
				elif self.pacmanLayer.direction == key.DOWN:
					if cNode.nodeDown == None:
						self.pacmanLayer.direction = None

	# _________________________________________________________________________________________
	#
	# Eat dots
	# _________________________________________________________________________________________

	# Remove wayNodes and wayNodeSprites if pacman reaches them
	def eatDots(self):
		for nodeSprite in self.labLayer.nodeSprites:
			if self.pacmanLayer.pacmanRect.center == (nodeSprite.x, nodeSprite.y):
				self.labLayer.remove(nodeSprite)
				self.labLayer.nodeSprites.remove(nodeSprite)
				self.pacmanLayer.updateScore(1)

	# _________________________________________________________________________________________
	#
	# Update method (called on every new frame)
	# _________________________________________________________________________________________

	def update(self, director):
		self.eatDots()
		self.setDirection()
		self.checkBorders()
		self.pacmanLayer.update(director)
		# if (self.reactor == False):
		# 	client.hatmanMain();
		# 	self.reactor = True;



class networkThread(threading.Thread):
	def __init__(self):
		threading.Thread.__init__(self);
		print("networkThread");

	def run(self):
		client.hatmanMain();

if __name__ == "__main__":

	thread = networkThread();
	thread.start();


	director.init(resizable=False, caption="HATman")
	# director.window.set_fullscreen(True)
	director.run(GameScene())
