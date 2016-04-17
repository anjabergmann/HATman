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

		self.nodeRect = Rect(self.x, self.y, 10, 10);


	def __str__(self):
		return "labNode [x: " + str(self.x) + ", y: " + str(self.y) + "]";

#___________________________________________________________________________________________________________





#Game Scene --> Contains all needed Layers [not yet, but ...]
class GameScene(Scene):


	def __init__(self):
		super(GameScene, self).__init__();
		
		#add Layers to scene
		self.labLayer = LabLayer();
		self.pacmanLayer = PacmanLayer();
		self.pacmanLayer.pacmanRect.center = (self.labLayer.crossNodes[0].x, self.labLayer.crossNodes[0].y);

		self.add(self.labLayer);
		self.add(self.pacmanLayer);

		#add schedule method
		self.schedule(self.update);

		self.pressedKey = None;
		self.direction = key.RIGHT;

		self.crossNodes = self.labLayer.crossNodes;


	#_________________________________________________________________________________________
	#
	# Change direction 
	#_________________________________________________________________________________________

	# Change the direction to pressedKey if it is possible 
	#		(= if pacman reaches a node with a neighbor-node in the pressedKey-direction)
	def setDirection(self):
		self.pressedKey = self.pacmanLayer.pressedKey;
		if self.pressedKey != self.direction:
			# invert direction --> always possible
			if (self.pressedKey == key.RIGHT and self.direction == key.LEFT) or (self.pressedKey == key.LEFT and self.direction == key.RIGHT) or (self.pressedKey == key.DOWN and self.direction == key.UP) or (self.pressedKey == key.UP and self.direction == key.DOWN):
				self.direction = self.pressedKey;
				self.pacmanLayer.direction = self.direction;
			else:
				for cn in self.crossNodes:
					if self.pacmanLayer.pacmanRect.center == (cn.x, cn.y):
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


	#_________________________________________________________________________________________
	#
	# Stand still
	#_________________________________________________________________________________________

	#Check if pacman reaches border or blind end
	# = if pacman reaches a node where neither "direction" nor "pressedKey" is a possible option
	def checkBorders(self):
		for cNode in self.labLayer.crossNodes:
			#only check if pacman reaches a crossNode (blind ends HAVE to be crossNodes)
			if self.pacmanLayer.pacmanRect.center == (cNode.x, cNode.y):
				if self.pacmanLayer.direction == key.RIGHT:
					if cNode.nodeRight == None:
						self.pacmanLayer.direction = None;
				elif self.pacmanLayer.direction == key.LEFT:
					if cNode.nodeLeft == None:
						self.pacmanLayer.direction = None;
				elif self.pacmanLayer.direction == key.UP:
					if cNode.nodeUp == None:
						self.pacmanLayer.direction = None;
				elif self.pacmanLayer.direction == key.DOWN:
					if cNode.nodeDown == None:
						self.pacmanLayer.direction = None;

	#_________________________________________________________________________________________
	#
	# Eat dots
	#_________________________________________________________________________________________

	# Remove wayNodes and wayNodeSprites if pacman reaches them
	def eatDots(self):
		for nodeSprite in self.labLayer.nodeSprites:
			if self.pacmanLayer.pacmanRect.center == (nodeSprite.x, nodeSprite.y):
				self.labLayer.remove(nodeSprite);
				self.labLayer.nodeSprites.remove(nodeSprite);
				self.pacmanLayer.updateScore(1);


	#_________________________________________________________________________________________
	#
	# Update method (called on every new frame)
	#_________________________________________________________________________________________

	def update(self, director):
		self.eatDots();
		self.setDirection();
		self.checkBorders();
		self.pacmanLayer.update(director);



#___________________________________________________________________________________________________________


class LabLayer(Layer):

	def __init__(self):
		super(LabLayer, self).__init__();

		# Size of the labyrinth
		self.labRect = Rect(40, 40, 560, 380);

		print("INFO labRect.top", self.labRect.top);
		print("INFO labRect.bottom", self.labRect.bottom);
		print("INFO labRect.left", self.labRect.left);
		print("INFO labRect.right", self.labRect.right);

		#_________________________________________________________________________________________
		#
		# Potential Nodes
		#_________________________________________________________________________________________

		# Every "point" inside the rect that is either part of a way or a barrier
		self.potentialNodes = [];

		# write every node of the Labyrinth into the list
		for i in range (2, 22):
			for j in range (2, 31): 
				tempNode = LabNode(x = j*20, y = i*20);
				self.potentialNodes.append(tempNode);

		#__________________________________________________________________________________________
		#
		# CrossNodes
		#__________________________________________________________________________________________

		# Nodes within the labyrinth where a change of direction (crossroad) is possible or a way ends (blind end)
		# Needed for navigation
		self.crossNodes = []; 

		#TODO Choose some Nodes that are crossNodes (random?)
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


		#__________________________________________________________________________________________
		#
		# WayNodes
		#__________________________________________________________________________________________

		# Every node that belongs to a way within the labyrinth
		# Only needed to draw the dots that are eaten by the pacman
		# Irrelevant for navigation
		self.wayNodes = [];

		#Add crossNodes and connections between two neighboured crossNodes to wayNodes[]
		for cNode in self.crossNodes:
			self.wayNodes.append(cNode);
			if cNode.nodeRight != None:
				for pNode in self.potentialNodes:
					if pNode.y == cNode.y and pNode.x > cNode.x and pNode.x < cNode.nodeRight.x:
						self.wayNodes.append(pNode);
			if cNode.nodeDown != None:
				for pNode in self.potentialNodes:
					if pNode.x == cNode.x and pNode.y < cNode.y and pNode.y > cNode.nodeDown.y:
						self.wayNodes.append(pNode);

		#Sprites for the wayNodes = dots that are eaten by pacman
		self.nodeSprites = [];

		for wayNode in self.wayNodes:
			tempSprite = Sprite("images/node.png");
			tempSprite.x = wayNode.x;
			tempSprite.y = wayNode.y;
			self.nodeSprites.append(tempSprite);

		# Add sprites to the layer
		for nodeSprite in self.nodeSprites:
			self.add(nodeSprite);


#___________________________________________________________________________________________________________




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
			pacman.scale = 0.05;

		#Animate pacman
		self.pacman2.do(Repeat(Blink(1, 0.3)));


		print("INFO pacman.top ", self.pacmanRect.top);
		print("INFO pacman.bottom ", self.pacmanRect.bottom);
		print("INFO pacman.left ", self.pacmanRect.left);
		print("INFO pacman.right ", self.pacmanRect.right);
		print("INFO pacmanRect.x ", self.pacmanRect.x);
		print("INFO pacmanRect.y ", self.pacmanRect.y);


		#Save pressed key
		self.pressedKey = None;

		#Save direciton
		self.direction = key.RIGHT;

		self._score = 0;


	# Getter and setter for score
	def getScore(self):
		return self._score;

	def setScore(self, score):
		self._sore = score;

	def updateScore(self, score):
		self._score = self._score + score;


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
	#
	# Move and rotate pacman
	#_______________________________________________

	# Method is called with schedule() on every new frame
	def update(self, director):

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


#___________________________________________________________________________________________________________



if __name__ == "__main__":
	director.init(resizable=False, caption='HATman')
	#director.window.set_fullscreen(True)
	director.run(GameScene());
