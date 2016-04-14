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

		self.pacmanRect = Rect(0, 0, self.pacman1.width * 0.1, self.pacman1.height * 0.1);
		self.labRect = Rect(0, 0, director.window.width, director.window.height);

		for pacman in self.pacmans:
			self.add(pacman);
			pacman.position = self.pacmanRect.center;
			pacman.scale = 0.1;

		print("labRect.top", self.labRect.top);
		print("labRect.bottom", self.labRect.bottom);
		print("labRect.left", self.labRect.left);
		print("labRect.right", self.labRect.right);

		print("pacman.top ", self.pacmanRect.top);
		print("pacman.bottom ", self.pacmanRect.bottom);
		print("pacman.left ", self.pacmanRect.left);
		print("pacman.right ", self.pacmanRect.right);





		#Animate pacman
		self.pacman2.do(Repeat(Blink(1, 0.3)));

		#Save pressed key
		self.pressed_key = None;

		#add schedule method
		self.schedule(self.update);

	#_______________________________________________
	#
	#Eventhandler for key presses
	#_______________________________________________
	
	def on_key_press(self, keys, mod):
		print("[INFO] Key pressed ", keys);
		if keys == key.RIGHT:
			self.pressed_key = key.RIGHT;
		if keys == key.LEFT:
			self.pressed_key = key.LEFT;
		if keys == key.UP:
			self.pressed_key = key.UP;
		if keys == key.DOWN:
			self.pressed_key = key.DOWN;
	#_______________________________________________



	# Method that is called with schedule() on every new frame
	def update(self, director):
		#_______________________________________________
		#
		# Move and rotate pacman
		#_______________________________________________

		if self.pressed_key == key.RIGHT:
			self.pacman1.rotation = None;
			if self.pacmanRect.right < self.labRect.right:
				self.pacmanRect.x += 1;
		elif self.pressed_key == key.LEFT:
			self.pacman1.rotation = 180;
			if self.pacmanRect.left > self.labRect.left:
				self.pacmanRect.x -= 1;
		elif self.pressed_key == key.UP:
			self.pacman1.rotation = 270;
			if self.pacmanRect.top < self.labRect.top:
				self.pacmanRect.y += 1
		elif self.pressed_key == key.DOWN:
			self.pacman1.rotation = 90;
			if self.pacmanRect.bottom > self.labRect.bottom:
				self.pacmanRect.y -= 1
			
		for pacman in self.pacmans:
			pacman.position = self.pacmanRect.center;

		#________________________________________________


if __name__ == "__main__":
	director.init(resizable=True, caption='HATman')
	#director.window.set_fullscreen(True)
	director.run(Scene(PacmanLayer()));
