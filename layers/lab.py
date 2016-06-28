from random import randint
from cocos.layer import Layer
from cocos.rect import Rect
from cocos.sprite import Sprite

class LabNode:
    def __init__(self, x, y, nodeRight=None, nodeLeft=None, nodeUp=None, nodeDown=None):
        # position of the node
        self.x = x
        self.y = y

        # neighbours
        self.nodeRight = nodeRight
        self.nodeLeft = nodeLeft
        self.nodeUp = nodeUp
        self.nodeDown = nodeDown

        self.nodeRect = Rect(self.x, self.y, 10, 10)

    def __str__(self):
        return "labNode [x: " + str(self.x) + ", y: " + str(self.y) + "]"

class LabLayer(Layer):
    def __init__(self):
        super(LabLayer, self).__init__()

        # Size of the labyrinth
        self.labRect = Rect(40, 40, 560, 380)

        # print("INFO labRect.top", self.labRect.top)
        # print("INFO labRect.bottom", self.labRect.bottom)
        # print("INFO labRect.left", self.labRect.left)
        # print("INFO labRect.right", self.labRect.right)

        # _________________________________________________________________________________________
        #
        # Potential Nodes
        # _________________________________________________________________________________________

        # Every "point" inside the rect that is either part of a way or a barrier
        self.potentialNodes = []

        # write every node of the Labyrinth into the list
        for i in range(2, 22):
            for j in range(2, 31):
                tempNode = LabNode(x=j * 20, y=i * 20)
                self.potentialNodes.append(tempNode)

        # __________________________________________________________________________________________
        #
        # CrossNodes
        # __________________________________________________________________________________________

        # Nodes within the labyrinth where a change of direction (crossroad) is possible or a way ends (blind end)
        # Needed for navigation
        self.crossNodes = []

        #add corners to crossnodes
        self.crossNodes.append(self.potentialNodes[0])  # links unten
        self.crossNodes.append(self.potentialNodes[28])  # rechts unten
        self.crossNodes.append(self.potentialNodes[551])  # links oben
        self.crossNodes.append(self.potentialNodes[579])  # rechts oben

        #choose random crossnodes
        for i in range (0, 30):
        	suitable = False
        	
        	while (suitable == False):
        		x = randint(0,579)
        		tmpNode = self.potentialNodes[x]

       			for node in self.crossNodes:
       				#same x or y as some other node
       				if (tmpNode.x == node.x or tmpNode.y == node.y):
       					#two paths cannot be next to each other
       					if (((40 > (tmpNode.x - node.x) > 0) or
       						(-40 < (tmpNode.x - node.x) < 0)) and
       						((40 > (tmpNode.y - node.y) > 0) or
       						(-40 < (tmpNode.y - node.y) < 0))):
       						suitable = False
       					else:
	       					suitable = True


        	self.crossNodes.append(self.potentialNodes[x])




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



        # __________________________________________________________________________________________
        #
        # WayNodes
        # __________________________________________________________________________________________

        # Every node that belongs to a way within the labyrinth
        # Only needed to draw the dots that are eaten by the pacman
        # Irrelevant for navigation
        self.wayNodes = []

        # Add crossNodes and connections between two neighboured crossNodes to wayNodes[]
        for cNode in self.crossNodes:
            self.wayNodes.append(cNode)

            #connect nodes horizontally
            if cNode.nodeRight != None:
                for pNode in self.potentialNodes:
                	#all possible nodes on the same height, to the right of cNode, before its next neighbour
                    if pNode.y == cNode.y and pNode.x > cNode.x and pNode.x < cNode.nodeRight.x:
                        self.wayNodes.append(pNode)

            #connect nodes vertically
            if cNode.nodeDown != None:
                for pNode in self.potentialNodes:
                	#all possible nodes on the same vertical line, from top to bottom
                    if pNode.x == cNode.x and pNode.y < cNode.y and pNode.y > cNode.nodeDown.y:
                        self.wayNodes.append(pNode)


        # __________________________________________________________________________________________
        #
        # WallNodes
        # __________________________________________________________________________________________


        self.wallNodes = []

        for node in self.potentialNodes:
        	if node not in self.wayNodes:
        		self.wallNodes.append(node)


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
