from cocos.rect import Rect

# LabNode class: defines a node of the labyrinth
# x, y... coordinates in the window
# neighbours... for crossNodes -> the next crossNode in that direction
#               for wayNodes -> the next wayNode in that direction (only relevant for detection of accidental crossNodes)
class LabNode:
	def __init__(self, x, y, sort=""):
		# position of the node
		self.x = x
		self.y = y

		# type of the node: cross/way/wall
		self.sort = sort

		# neighbours
		self.nodeRight = None
		self.nodeLeft = None
		self.nodeUp = None
		self.nodeDown = None

		self.nodeRect = Rect(self.x, self.y, 10, 10)

	def __str__(self):
		return str(self.x) + ";" + str(self.y)

	def __eq__(self, other): 
		if (other is not None):
			return self.x == other.x and self.y == other.y
		else:
			return False

	def __ne__(self, other):
		if (other is not None):
			return self.x == other.x and self.y == other.y
		else:
			return False
