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

		self.pacmanRect = Rect(20, 40, self.pacman1.width * 0.1, self.pacman1.height * 0.1);
		self.labRect = Rect(20, 40, 600, 400);
		self.potentialNodes = [];
		
		# write every "point" of the Labyrinth into a list
		for i in range (2, 22):
			for j in range (2, 31): 
				tempNode = LabNode(x = j*20, y = i*20);
				self.potentialNodes.append(tempNode);
				print("INFO", tempNode);


		for pacman in self.pacmans:
			self.add(pacman);
			pacman.position = self.pacmanRect.center;
			pacman.scale = 0.1;

		print("INFO labRect.top", self.labRect.top);
		print("INFO labRect.bottom", self.labRect.bottom);
		print("INFO labRect.left", self.labRect.left);
		print("INFO labRect.right", self.labRect.right);

		print("INFO pacman.top ", self.pacmanRect.top);
		print("INFO pacman.bottom ", self.pacmanRect.bottom);
		print("INFO pacman.left ", self.pacmanRect.left);
		print("INFO pacman.right ", self.pacmanRect.right);



		#Animate pacman
		self.pacman2.do(Repeat(Blink(1, 0.3)));

		#Save pressed key
		self.pressed_key = None;

		#Save direciton
		self.direction = key.RIGHT;

		#add schedule method
		self.schedule(self.update);

	#_______________________________________________
	#
	#Eventhandler for key presses
	#_______________________________________________
	
	def on_key_press(self, keys, mod):
		print("INFO Key pressed ", keys);
		if keys == key.RIGHT:
			self.pressed_key = key.RIGHT;
		if keys == key.LEFT:
			self.pressed_key = key.LEFT;
		if keys == key.UP:
			self.pressed_key = key.UP;
		if keys == key.DOWN:
			self.pressed_key = key.DOWN;
	#_______________________________________________


	#TODO:
	# Überprüfen, ob Pacman sich an einem Node befindet,
	# an dem er in die gewünschte Richtung abbiegen kann
	# Wenn möglich: Direction auf die gewünschte Direction ändern
	# Sonst in die aktuelle Direction weiterfahren bis anstehen

	def setDirection(self):
		if self.pressed_key != self.direction:
			if self.pressed_key == key.RIGHT:
				print("RIGHT");
			elif self.pressed_key == key.LEFT:
				print("LEFT");
			elif self.pressed_key == key.UP:
				print("UP");
			elif self.pressed_key == key.DOWN:
				print("DOWN");


	# Method that is called with schedule() on every new frame
	def update(self, director):
		self.setDirection();

		#_______________________________________________
		#
		# Move and rotate pacman
		#_______________________________________________

		if self.pressed_key == key.RIGHT:
			self.pacman1.rotation = None;
			if self.pacmanRect.right < self.labRect.right:
				self.pacmanRect.x += 2;
		elif self.pressed_key == key.LEFT:
			self.pacman1.rotation = 180;
			if self.pacmanRect.left > self.labRect.left:
				self.pacmanRect.x -= 2;
		elif self.pressed_key == key.UP:
			self.pacman1.rotation = 270;
			if self.pacmanRect.top < self.labRect.top:
				self.pacmanRect.y += 2;
		elif self.pressed_key == key.DOWN:
			self.pacman1.rotation = 90;
			if self.pacmanRect.bottom > self.labRect.bottom:
				self.pacmanRect.y -= 2;
			
		for pacman in self.pacmans:
			pacman.position = self.pacmanRect.center;

		#________________________________________________


if __name__ == "__main__":
	director.init(resizable=False, caption='HATman')
	#director.window.set_fullscreen(True)
	director.run(Scene(PacmanLayer()));
