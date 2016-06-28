from random import randint
from cocos.layer import Layer
from cocos.rect import Rect
from cocos.sprite import Sprite

class LabNode:
    def __init__(self, x, y):
        # position of the node
        self.x = x
        self.y = y

        # type of the node: cross/way/wall
        self.sort = ""

        # neighbours
        self.nodeRight = None
        self.nodeLeft = None
        self.nodeUp = None
        self.nodeDown = None

        self.nodeRect = Rect(self.x, self.y, 10, 10)

    def __str__(self):
        return "labNode [x: " + str(self.x) + ", y: " + str(self.y) + "]"


class LabLayer(Layer):
    def __init__(self):
        super(LabLayer, self).__init__()

        # Size of the labyrinth
        self.labRect = Rect(40, 40, 560, 380)


        # _________________________________________________________________________________________
        #
        # Potential Nodes
        # _________________________________________________________________________________________

        # Every "point" inside the rect that is either part of a way or a barrier
        self.nodes = [[0 for x in range(20)] for y in range(29)]

        # write every node of the Labyrinth into the list
        for j in range(0, 20):
            for i in range(0, 29):
                tempNode = LabNode(x=i * 20 + 40, y=j * 20 + 40)
                self.nodes[i][j] = tempNode


        # __________________________________________________________________________________________
        #
        # CrossNodes
        # __________________________________________________________________________________________

        # Nodes within the labyrinth where a change of direction (crossroad) is possible or a way ends (blind end)
        # Needed for navigation
        self.crossNodes = []

        #add corners to crossnodes
        self.crossNodes.append(self.nodes[0][0])
        self.crossNodes.append(self.nodes[0][19])
        self.crossNodes.append(self.nodes[28][0])
        self.crossNodes.append(self.nodes[28][19])

        self.nodes[0][0].sort = "cross"		#links unten
        self.nodes[0][19].sort = "cross"	#links oben
        self.nodes[28][0].sort = "cross"	#rechts unten
        self.nodes[28][19].sort = "cross"	#rechts oben



        def chooseNodes(num):
	        #choose more or less random crossnodes

	        # list of possible (allowed) coordinates
	        xpos = [0, 28]
	        ypos = [0, 19]
	        # list of impossible (because too close) coordinates
	        ximpos = [1, 27]
	        yimpos = [1, 18]

	        x = 0
	        y = 0

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

	        	# set sort to cross and add to crossNodes
        		self.nodes[x][y].sort = "cross"
        		self.crossNodes.append(self.nodes[x][y])


        chooseNodes(25)


        def connectNodes():
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
	        		elif (currNode.y == otherNode.y):
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

        connectNodes()

        # __________________________________________________________________________________________
        #
        # WayNodes
        # __________________________________________________________________________________________

        # Every node that belongs to a way within the labyrinth
        # Only needed to draw the dots that are eaten by the pacman
        # Irrelevant for navigation
        self.wayNodes = []
        for cnode in self.crossNodes:
        	self.wayNodes.append(cnode)

        #Add crossNodes and connections between two neighboured crossNodes to wayNodes[]
        for cNode in self.crossNodes:
        	self.wayNodes.append(cNode)

            #connect nodes horizontally
        	if cNode.nodeRight != None:
        		for nodes in self.nodes:
        			for pNode in nodes:
        				#all possible nodes on the same height, to the right of cNode, before its next neighbour
        				if pNode.y == cNode.y and pNode.x > cNode.x and pNode.x < cNode.nodeRight.x:
        					self.wayNodes.append(pNode)
        					#very complicated way of getting the node object
        					self.nodes[self.nodes.index(nodes)][self.nodes[self.nodes.index(nodes)].index(pNode)].sort = "way"

            #connect nodes vertically
        	if cNode.nodeDown != None:
        		for nodes in self.nodes:
        			for pNode in nodes:
        				#all possible nodes on the same vertical line, from top to bottom
        				if pNode.x == cNode.x and pNode.y < cNode.y and pNode.y > cNode.nodeDown.y:
        					self.wayNodes.append(pNode)
        					#very complicated way of getting the node object
        					self.nodes[self.nodes.index(nodes)][self.nodes[self.nodes.index(nodes)].index(pNode)].sort = "way"



        # __________________________________________________________________________________________
        #
        # WallNodes
        # __________________________________________________________________________________________


        self.wallNodes = []

        for nodes in self.nodes:
        	for n in nodes:
        		if n not in self.wayNodes:
        			self.wallNodes.append(n)


        # __________________________________________________________________________________________
        #
        # Drawing
        # __________________________________________________________________________________________

        # Sprites for the wayNodes = dots that are eaten by pacman
        self.nodeSprites = []
        self.wallSprites = []

        #TODO: non-way-nodes zb blau markieren
        for wallNode in self.wallNodes:
        	tempSprite = Sprite("images/lab.png")
        	tempSprite.x = wallNode.x
        	tempSprite.y = wallNode.y
        	self.wallSprites.append(tempSprite)


        for wayNode in self.wayNodes:
        	if wayNode in self.crossNodes:
        		tempSprite = Sprite("images/cnode.png")
        	else:
        		tempSprite = Sprite("images/node.png")

        	tempSprite.x = wayNode.x
        	tempSprite.y = wayNode.y
        	self.nodeSprites.append(tempSprite)

        # Add sprites to the layer
        for nodeSprite in self.nodeSprites:
            self.add(nodeSprite)

        for wallSprite in self.wallSprites:
        	self.add(wallSprite)
