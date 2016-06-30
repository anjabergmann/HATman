from random import randint
from cocos.layer import Layer
from cocos.rect import Rect
from cocos.sprite import Sprite
from helper.node import LabNode


class LabLayer(Layer):
	def __init__(self):
		super(LabLayer, self).__init__()

		# Size of the labyrinth
		self.labRect = Rect(40, 40, 560, 380)

		self.nodes = self.createAllNodes()

		self.crossNodes = self.chooseCrossNodes(25)
		self.wayNodes = []
		self.wallNodes = []

		
		self.connectCrossNodes()
		self.addWayNodes()
		self.connectWayNodes()
		self.addMissingCrossNodes()
		self.makeWallNodes()

		self.drawSprites()


	# _________________________________________________________________________________________
	#
	# Potential Nodes
	# _________________________________________________________________________________________
	# creates every "point" inside the rect that is either part of a way or a barrier
	def createAllNodes(self):
		print("in createAllNodes")
		nodes = [[0 for x in range(20)] for y in range(29)]

		# write every node of the Labyrinth into the list
		for j in range(0, 20):
			for i in range(0, 29):
				tempNode = LabNode(x=i * 20 + 40, y=j * 20 + 40)
				nodes[i][j] = tempNode

		return nodes

	# __________________________________________________________________________________________
	#
	# CrossNodes
	# __________________________________________________________________________________________
	# creates nodes within the labyrinth where a change of direction is possible (crossroad) 
	# or where a way ends (blind end) -> Needed for navigation
	# this function is not actually needed here because the server now chooses the nodes
	# but we'll leave it in case we need it again
	def chooseCrossNodes(self,num):
		cNodes = []

		# add corners to crossnodes and mark as crossNodes
		self.nodes[0][0].sort = "cross"		#links unten
		self.nodes[0][19].sort = "cross"	#links oben
		self.nodes[28][0].sort = "cross"	#rechts unten
		self.nodes[28][19].sort = "cross"	#rechts oben
		cNodes.append(self.nodes[0][0])
		cNodes.append(self.nodes[0][19])
		cNodes.append(self.nodes[28][0])
		cNodes.append(self.nodes[28][19])

		# list of possible (allowed) coordinates
		xpos = [0, 28];		ypos = [0, 19]
		# list of impossible (because too close) coordinates
		ximpos = [1, 27];	yimpos = [1, 18]
		# currently chosen position
		x = 0;	y = 0

		for i in range(num):
			# alternate between fixed x or y
			# 'fixed' means it is chosen from the list of already existing crossNodes
			if (i % 2 == 0):
				x = xpos[randint(0,len(xpos)-1)]
				# calculate new random int until one fits
				while (True):
					y = randint(0,19)
					#check if random position is too close to another crossNode
					if y not in yimpos:
						if y not in ypos:
							# add if not marked as impossible and not already in possible
							ypos.append(y)
							# mark the two adjoining nodes as impossible
							yimpos.append(y-2)
							yimpos.append(y-1)
							yimpos.append(y+1)
							yimpos.append(y+2)
						break
			else:
				y = ypos[randint(0,len(ypos)-1)]
				# calculate new random int until one fits
				while (True):
					x = randint(0,28)
					# check if random position is too close to another crossNode
					if x not in ximpos:
						if x not in xpos:
							# add to possible if it hasn't been already and is not in impossible
							xpos.append(x)
							# mark the adjoining nodes as impossible
							ximpos.append(x-2)
							ximpos.append(x-1)
							ximpos.append(x+1)
							ximpos.append(x+2)
						break

			# set sort to cross and add to cross nodes
			if self.nodes[x][y] not in cNodes:
				self.nodes[x][y].sort = "cross"
				cNodes.append(self.nodes[x][y])

		return cNodes


	# determine the neighbours of cross nodes (for navigation)
	# TODO: consider passing the crossnode list as parameter
	def connectCrossNodes(self):
		#connecting adjacent crossNodes by reference
		for currNode in self.crossNodes:
			for otherNode in self.crossNodes:
				#same x coordinates -> either above or under
				if (currNode.x == otherNode.x):
					#current under other
					if (currNode.y < otherNode.y):
						currNode.nodeUp = otherNode
						otherNode.nodeDown = currNode
					#curr above other
					elif (currNode.y > otherNode.y):
						currNode.nodeDown = otherNode
						otherNode.nodeUp = currNode

				#same y coordinates -> left or right
				if (currNode.y == otherNode.y):
					#current left of other
					if (currNode.x < otherNode.x):
						currNode.nodeRight = otherNode
						otherNode.nodeLeft = currNode
					#current right of other
					elif (currNode.x > otherNode.x):
						currNode.nodeLeft = otherNode
						otherNode.nodeRight = currNode

				else:
					pass #no connection



	# __________________________________________________________________________________________
	#
	# WayNodes
	# __________________________________________________________________________________________
	# Every node that belongs to a way within the labyrinth
	# Only needed to draw the dots that are eaten by the pacman
	# Irrelevant for navigation

		#Add crossNodes and connections between two neighboured crossNodes to wayNodes[]
	def addWayNodes(self):
		for cNode in self.crossNodes:
			self.wayNodes.append(cNode)

			#connect nodes horizontally
			if cNode.nodeRight != None:
				for nodes in self.nodes:
					for pNode in nodes:
						#all possible nodes on the same height, to the right of cNode, before its next neighbour
						if pNode.y == cNode.y and pNode.x > cNode.x and pNode.x < cNode.nodeRight.x:
							self.wayNodes.append(pNode)
							pNode.sort = "way"

			#connect nodes vertically
			if cNode.nodeDown != None:
				for nodes in self.nodes:
					for pNode in nodes:
						#all possible nodes on the same vertical line, from top to bottom
						if pNode.x == cNode.x and pNode.y < cNode.y and pNode.y > cNode.nodeDown.y:
							#only add if it isn't already a waynode
							if pNode not in self.wayNodes:
								self.wayNodes.append(pNode)
								pNode.sort = "way"


	def connectWayNodes(self):
		#connecting adjacent wayNodes by reference
		for currNode in self.wayNodes:
			for otherNode in self.wayNodes:
				#same x coordinates -> either above or under
				if (currNode.x == otherNode.x):
					#current under other
					if (currNode.y == otherNode.y-20):
						currNode.nodeUp = otherNode
						otherNode.nodeDown = currNode
					#curr above other
					elif (currNode.y == otherNode.y+20):
						currNode.nodeDown = otherNode
						otherNode.nodeUp = currNode

				#same y coordinates -> left or right
				if (currNode.y == otherNode.y):
					#current left of other
					if (currNode.x == otherNode.x-20):
						currNode.nodeRight = otherNode
						otherNode.nodeLeft = currNode
					#current right of other
					elif (currNode.x == otherNode.x+20):
						currNode.nodeLeft = otherNode
						otherNode.nodeRight = currNode



	def addMissingCrossNodes(self):
		# creating wayNodes can accidentally create crossNodes which are not in the crossNode array
		# if a wayNode has 3 or 4 neighbours, it is marked as a crossNode and added to the array
		for wNode in self.wayNodes:
			if ((wNode.nodeLeft != None and wNode.nodeRight != None) and (wNode.nodeUp != None or wNode.nodeDown != None)):
				wNode.sort = "cross"
				self.crossNodes.append(wNode)
			elif ((wNode.nodeUp != None and wNode.nodeDown != None) and (wNode.nodeLeft != None or wNode.nodeRight != None)):
				wNode.sort = "cross"
				self.crossNodes.append(wNode)


	# __________________________________________________________________________________________
	#
	# WallNodes
	# __________________________________________________________________________________________
	# nodes that are not way nodes (includes cross nodes) are wall nodes
	def makeWallNodes(self):
		for nodes in self.nodes:
			for n in nodes:
				if n not in self.wayNodes:
					self.wallNodes.append(n)
					n.sort = "wall"



	# __________________________________________________________________________________________
	#
	# Drawing
	# __________________________________________________________________________________________
	def drawSprites(self):
		# Sprites for the wayNodes = dots that are eaten by pacman
		self.nodeSprites = []

		# add nodeSprites to array, add all to layer
		for nodes in self.nodes:
			for node in nodes:
				if node.sort == "cross":
					tempSprite = Sprite("images/cnode.png")
				if node.sort == "wall":
					tempSprite = Sprite("images/wall.png")
				if node.sort == "way":
					tempSprite = Sprite("images/node.png")

				tempSprite.x = node.x
				tempSprite.y = node.y

				if node.sort == "cross" or node.sort == "way":
					self.nodeSprites.append(tempSprite)

				self.add(tempSprite)





