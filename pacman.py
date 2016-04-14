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

		for pacman in self.pacmans:
			self.add(pacman);
			pacman.position = 320, 300;
			pacman.scale = 0.1;

		# self.add(self.pacman1);
		# self.add(self.pacman2);

		# self.pacman1.position = 320, 300;
		# self.pacman2.position = 320, 300;

		# self.pacman1.scale = 0.1;
		# self.pacman2.scale = 0.1;

		self.moveUp = MoveBy((0, 10), 0.1);
		self.moveDown = MoveBy((0, -10), 0.1);
		self.moveLeft = MoveBy((-10, 0), 0.1);
		self.moveRight = MoveBy((10, 0), 0.1);
		self.direction = self.moveRight;

		self.pacman2.do(Repeat(Blink(1, 0.3)));

		#Save pressed key
		self.pressed_key = None;

		#add schedule method
		self.schedule(self.update);


	#Eventhandler for key presses
	def on_key_press(self, keys, mod):
		print("Key pressed ", keys);
		if keys == key.RIGHT:
			print("KEY RIGHT");
			self.pressed_key = key.RIGHT;
		if keys == key.LEFT:
			print("KEY LEFT");
			self.pressed_key = key.LEFT;
		if keys == key.UP:
			print("KEY UP");
			self.pressed_key = key.UP;
		if keys == key.DOWN:
			print("KEY DOWN");
			self.pressed_key = key.DOWN;



	# Method that is called with schedule() on every new frame
	def update(self, director):
		if self.pressed_key == key.RIGHT:
			self.direction = self.moveRight;
			self.pacman1.rotation = None;
		elif self.pressed_key == key.LEFT:
			self.direction = self.moveLeft;
			self.pacman1.rotation = 180;
		elif self.pressed_key == key.UP:
			self.direction = self.moveUp;
			self.pacman1.rotation = 270;
		elif self.pressed_key == key.DOWN:
			self.direction = self.moveDown;
			self.pacman1.rotation = 90;
		for pacman in self.pacmans:
			pacman.do(self.direction);



if __name__ == "__main__":
	director.init(resizable=True, caption='HATman')
	#director.window.set_fullscreen(True)
	director.run(Scene(PacmanLayer()));
