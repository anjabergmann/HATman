from __future__ import division, print_function, unicode_literals

import sys
import os
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..'))

from pyglet import font
from pyglet.gl import *
from pyglet.window import key

from cocos.actions import *
from cocos.director import director
from cocos.layer import Layer
from cocos.scene import Scene
from cocos.sprite import Sprite
from cocos.rect import Rect




class LabNode():
	def __init__(self, x, y, nodeRight = None, nodeLeft = None, nodeUp = None, nodeDown = None):
		
		#position of the node
		self.x = x;
		self.y = y;

		#neighbours
		self.nodeRight = nodeRight;
		self.nodeLeft = nodeLeft;
		self.nodeUp = nodeUp;
		self.nodeDown = nodeDown;

	#_________________________________________________________________
	#
	# Find out if it is possible to go from this knot to direction x
	#_________________________________________________________________

	def goRight(self):
		if self.nodeRight == None:
			return False;
		return True;

	def goLeft(self):
		if self.nodeLeft == None:
			return False;
		return True;

	def goUp(self):
		if self.nodeUp == None:
			return False;
		return True;

	def goDown(self):
		if self.nodeDown == None:
			return False;
		return True;

	#_________________________________________________________________


	def __str__(self):
		return "labNode [x: " + str(self.x) + ", y: " + str(self.y) + "]";






#Game Scene --> Contains all needed Layers [not yet, but ...]
class GameScene(Scene):


	def __init__(self):
		super(GameScene, self).__init__();
		
		#add Layers to scene
		self.labLayer = LabLayer();
		self.pacmanLayer = PacmanLayer();

		self.add(self.labLayer);
		self.add(self.pacmanLayer);

		#add schedule method
		self.schedule(self.update);

		self.pressedKey = None;
		self.direction = key.RIGHT;

		self.crossNodes = self.labLayer.crossNodes;

	#TODO:
	# Überprüfen, ob Pacman sich an einem Node befindet,
	# an dem er in die gewünschte Richtung abbiegen kann
	# Wenn möglich: Direction auf die gewünschte Direction ändern
	# Sonst in die aktuelle Direction weiterfahren bis anstehen
	def setDirection(self):
		self.pressedKey = self.pacmanLayer.pressedKey;
		if self.pressedKey != self.direction:
			if self.pressedKey == (key.RIGHT and self.direction == key.LEFT) or (self.pressedKey == key.LEFT and self.direction == key.RIGHT) or (self.pressedKey == key.DOWN and self.direction == key.UP) or (self.pressedKey == key.UP and self.direction == key.DOWN):
				self.direction = self.pressedKey;
				self.pacmanLayer.direction = self.direction;
			else:
				for cn in self.crossNodes:
					if self.pacmanLayer.pacmanRect.x == cn.x and self.pacmanLayer.pacmanRect.y == cn.y:
						if self.pressedKey == key.RIGHT and cn.nodeRight != None:
							self.direction = self.pressedKey;
							self.pacmanLayer.direction = self.direction;
						elif self.pressedKey == key.LEFT and cn.nodeLeft != None:
							self.direction = self.pressedKey;
							self.pacmanLayer.direction = self.direction;
						elif self.pressedKey == key.UP and cn.nodeUp != None:
							self.direction = self.pressedKey;
							self.pacmanLayer.direction = self.direction;
						elif self.pressedKey == key.DOWN and cn.nodeDown != None:
							self.direction = self.pressedKey;
							self.pacmanLayer.direction = self.direction;



	#Check if pacman reaches border 
	#or [TODO] deadlock
	#	if pacman reaches Node where neither "direction" nor "pressedKey"
	#	is a possible option
	# --> stand still
	def checkBorders(self):
		if self.pacmanLayer.direction == key.RIGHT:
			#if self.pacmanLayer.pacmanRect.right >= self.labLayer.labRect.right:
			if self.pacmanLayer.pacmanRect.x >= 600:
				self.pacmanLayer.direction = None;
		elif self.pacmanLayer.direction == key.LEFT:
			#if self.pacmanLayer.pacmanRect.left <= self.labLayer.labRect.left:
			if self.pacmanLayer.pacmanRect.x <= 40:
				self.pacmanLayer.direction = None;
		elif self.pacmanLayer.direction == key.UP:
			#if self.pacmanLayer.pacmanRect.top >= self.labLayer.labRect.top:
			if self.pacmanLayer.pacmanRect.y >= 420:
				self.pacmanLayer.direction = None;
		elif self.pacmanLayer.direction == key.DOWN:
			#if self.pacmanLayer.pacmanRect.bottom <= self.labLayer.labRect.bottom:
			if self.pacmanLayer.pacmanRect.y <= 40:
				self.pacmanLayer.direction = None;

	def update(self, director):
		self.setDirection();
		self.checkBorders();
		self.pacmanLayer.update(director);



#____________________________________________________________________


class LabLayer(Layer):

	def __init__(self):
		super(LabLayer, self).__init__();

		self.labRect = Rect(20, 40, 600, 400);
		self.potentialNodes = []; #Zweidimensionale Arrays wären vielleicht besser ...
		self.crossNodes = [];

		# write every "point" of the Labyrinth into a list
		for i in range (2, 22):
			for j in range (2, 31): 
				tempNode = LabNode(x = j*20, y = i*20);
				self.potentialNodes.append(tempNode);

		#TODO Choose some Nodes that are crossNodes (random?)
		#self.crossNodes = self.potentialNodes;

		#For testing purposes only four nodes in the corners are crossNodes
		self.crossNodes.append(self.potentialNodes[0]); #links unten
		self.crossNodes.append(self.potentialNodes[28]); #rechts unten
		self.crossNodes.append(self.potentialNodes[551]); #links oben
		self.crossNodes.append(self.potentialNodes[579]); #rechts oben

		self.crossNodes[0].nodeUp = self.crossNodes[2];
		self.crossNodes[0].nodeRight = self.crossNodes[1];
		self.crossNodes[1].nodeUp = self.crossNodes[3];
		self.crossNodes[1].nodeLeft = self.crossNodes[0];
		self.crossNodes[2].nodeRight = self.crossNodes[3];
		self.crossNodes[2].nodeDown = self.crossNodes[0];
		self.crossNodes[3].nodeLeft = self.crossNodes[2];
		self.crossNodes[3].nodeDown = self.crossNodes[1];


		for i in range(0, 4):
			tempSprite = Sprite('images/node.png');
			tempSprite.x = self.crossNodes[i].x;
			tempSprite.y = self.crossNodes[i].y
			self.add(tempSprite);


		print("INFO labRect.top", self.labRect.top);
		print("INFO labRect.bottom", self.labRect.bottom);
		print("INFO labRect.left", self.labRect.left);
		print("INFO labRect.right", self.labRect.right);


#____________________________________________________________________




class PacmanLayer(Layer):

	#enable pyglet events
	is_event_handler = True

	def __init__(self):
		super(PacmanLayer, self).__init__();

		#create and add Sprites for pacman
		self.pacman1 = Sprite(pyglet.resource.image('images/pacman1.png'));
		self.pacman2 = Sprite(pyglet.resource.image('images/pacman2.png'));

		self.pacmans = [];

		self.pacmans.append(self.pacman1);
		self.pacmans.append(self.pacman2);

		self.pacmanRect = Rect(40, 40, self.pacman1.width * 0.1, self.pacman1.height * 0.1);

		for pacman in self.pacmans:
			self.add(pacman);
			pacman.position = self.pacmanRect.center;
			pacman.scale = 0.1;


		print("INFO pacman.top ", self.pacmanRect.top);
		print("INFO pacman.bottom ", self.pacmanRect.bottom);
		print("INFO pacman.left ", self.pacmanRect.left);
		print("INFO pacman.right ", self.pacmanRect.right);

		print("INFO pacmanRect.x ", self.pacmanRect.x);
		print("INFO pacmanRect.y ", self.pacmanRect.y);


		#Animate pacman
		self.pacman2.do(Repeat(Blink(1, 0.3)));

		#Save pressed key
		self.pressedKey = None;

		#Save direciton
		self.direction = key.RIGHT;

	#_______________________________________________
	#
	# Eventhandler for key presses
	#_______________________________________________
	
	def on_key_press(self, keys, mod):
		print("INFO Key pressed ", keys);
		if keys == key.RIGHT:
			self.pressedKey = key.RIGHT;
		if keys == key.LEFT:
			self.pressedKey = key.LEFT;
		if keys == key.UP:
			self.pressedKey = key.UP;
		if keys == key.DOWN:
			self.pressedKey = key.DOWN;
	#_______________________________________________


	def setDirection(self):
		self.direction = self.pressedKey;


	# Method that is called with schedule() on every new frame
	def update(self, director):

		#_______________________________________________
		#
		# Move and rotate pacman
		#_______________________________________________

		if self.direction == key.RIGHT:
			self.pacman1.rotation = None;
			self.pacmanRect.x += 2;
		elif self.direction == key.LEFT:
			self.pacman1.rotation = 180;
			self.pacmanRect.x -= 2;
		elif self.direction == key.UP:
			self.pacman1.rotation = 270;
			self.pacmanRect.y += 2;
		elif self.direction == key.DOWN:
			self.pacman1.rotation = 90;
			self.pacmanRect.y -= 2;
			
		for pacman in self.pacmans:
			pacman.position = self.pacmanRect.center;

		#________________________________________________


if __name__ == "__main__":
	director.init(resizable=False, caption='HATman')
	#director.window.set_fullscreen(True)
	director.run(GameScene());
