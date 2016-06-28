from cocos.sprite import Sprite
from cocos.rect import Rect
from cocos.actions import *
from pyglet.gl import *
from pyglet.window import key
from layers.char import CharLayer

class PacmanLayer(CharLayer):

	def __init__(self):
		super().__init__()

		# create and add Sprites for pacman
		self.pacman1 = Sprite(pyglet.resource.image("images/pacman1.png"))
		self.pacman2 = Sprite(pyglet.resource.image("images/pacman2.png"))

		self.sprites.append(self.pacman1)
		self.sprites.append(self.pacman2)

		self.charRect = Rect(40, 40, self.pacman1.width * 0.05, self.pacman1.height * 0.05)
		self.charRect.center = (40, 40);

		for pacman in self.sprites:
			self.add(pacman)
			pacman.position = self.charRect.center
			pacman.scale = 0.05

		# Animate pacman
		self.pacman2.do(Repeat(Blink(1, 0.3)))

		self.rotations = [90, 180, 270, 0]

		# print("INFO pacman.top ", self.charRect.top)
		# print("INFO pacman.bottom ", self.charRect.bottom)
		# print("INFO pacman.left ", self.charRect.left)
		# print("INFO pacman.right ", self.charRect.right)
		# print("INFO pacmanRect.x ", self.charRect.x)
		# print("INFO pacmanRect.y ", self.charRect.y)



	# Method is called with schedule() on every new frame
	def update(self, director):

		# Update position of pacman in the same way as for the ghosts
		super().update(director);

		# Rotate sprite appropriate to the direction
		self.pacman1.rotation = self.rotations[(self.direction % 4)];