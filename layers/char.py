from cocos.layer import Layer
from cocos.rect import Rect
from pyglet.gl import *
from pyglet.window import key



class CharLayer(Layer):

	# enable pyglet events
	is_event_handler = True


	def __init__(self):
		super().__init__()


		self.sprites = [];	# sprites
		self.charRect = None; 		# rectangle around sprites for collision detection

		# Save pressed key
		self.pressedKey = None

		# Save direciton
		self.direction = key.RIGHT

		self._score = 0



	# Getter and setter for score
	def getScore(self):
		return self._score

	def setScore(self, score):
		self._sore = score

	def updateScore(self, score):
		self._score = self._score + score




	# Eventhandler for key presses
	def on_key_press(self, keys, mod):
		#print("INFO Key pressed ", keys)
		if keys == key.RIGHT:
			self.pressedKey = key.RIGHT
		if keys == key.LEFT:
			self.pressedKey = key.LEFT
		if keys == key.UP:
			self.pressedKey = key.UP
		if keys == key.DOWN:
			self.pressedKey = key.DOWN




	# Method is called with schedule() on every new frame
	def update(self, director):
		if self.direction == key.RIGHT:
			self.charRect.x += 2
		elif self.direction == key.LEFT:
			self.charRect.x -= 2
		elif self.direction == key.UP:
			self.charRect.y += 2
		elif self.direction == key.DOWN:
			self.charRect.y -= 2

		for sprite in self.sprites:
			sprite.position = self.charRect.center

