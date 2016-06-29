from cocos.sprite import Sprite
from cocos.rect import Rect
from cocos.actions import *
from pyglet.gl import *
from pyglet.window import key
from layers.char import CharLayer

class GhostLayer(CharLayer):

	def __init__(self, color="blue"):
		super().__init__()

		# create and add Sprites for ghost
		self.ghost1 = Sprite(pyglet.resource.image("images/" + color + "1.png"))
		self.ghost2 = Sprite(pyglet.resource.image("images/" + color + "2.png"))
		self.eyes = Sprite(pyglet.resource.image("images/eyesDown.png"))

		self.sprites.append(self.ghost1)
		self.sprites.append(self.ghost2)
		self.sprites.append(self.eyes)

		self.charRect = Rect(40, 40, self.ghost1.width * 0.12, self.ghost1.height * 0.12)
		self.charRect.center = (40, 40);


		for ghost in self.sprites:
			self.add(ghost)
			ghost.position = self.charRect.center
			ghost.scale = 0.12

		# Animate ghost
		self.ghost1.do(Repeat(Blink(1, 0.3)))

		# print("INFO ghost.top ", self.charRect.top)
		# print("INFO ghost.bottom ", self.charRect.bottom)
		# print("INFO ghost.left ", self.charRect.left)
		# print("INFO ghost.right ", self.charRect.right)
		# print("INFO charRect.x ", self.charRect.x)
		# print("INFO charRect.y ", self.charRect.y)

