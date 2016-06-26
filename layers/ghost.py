from cocos.layer import Layer
from cocos.sprite import Sprite
from cocos.rect import Rect
from cocos.actions import *
from pyglet.gl import *
from pyglet.window import key

class GhostLayer(Layer):
    # enable pyglet events
    is_event_handler = True

    def __init__(self, color="blue"):
        super(GhostLayer, self).__init__()

        # create and add Sprites for ghost
        self.ghost1 = Sprite(pyglet.resource.image("images/" + color + "1.png"))
        self.ghost2 = Sprite(pyglet.resource.image("images/" + color + "2.png"))
        self.eyes = Sprite(pyglet.resource.image("images/eyesDown.png"))

        self.ghosts = []

        self.ghosts.append(self.ghost1)
        self.ghosts.append(self.ghost2)
        self.ghosts.append(self.eyes)

        self.ghostRect = Rect(40, 40, self.ghost1.width * 0.12, self.ghost1.height * 0.12)
        self.ghostRect.center = (40, 40);


        for ghost in self.ghosts:
            self.add(ghost)
            ghost.position = self.ghostRect.center
            ghost.scale = 0.12

        # Animate ghost
        self.ghost1.do(Repeat(Blink(1, 0.3)))

        print("INFO ghost.top ", self.ghostRect.top)
        print("INFO ghost.bottom ", self.ghostRect.bottom)
        print("INFO ghost.left ", self.ghostRect.left)
        print("INFO ghost.right ", self.ghostRect.right)
        print("INFO ghostRect.x ", self.ghostRect.x)
        print("INFO ghostRect.y ", self.ghostRect.y)


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

    # _______________________________________________
    #
    # Eventhandler for key presses
    # _______________________________________________

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

    # _______________________________________________
    #
    # Move ghost
    # _______________________________________________

    # Method is called with schedule() on every new frame
    def update(self, director):
        # TODO: "Rotate" Eyes
        if self.direction == key.RIGHT:
            self.ghostRect.x += 2
        elif self.direction == key.LEFT:
            self.ghostRect.x -= 2
        elif self.direction == key.UP:
            self.ghostRect.y += 2
        elif self.direction == key.DOWN:
            self.ghostRect.y -= 2

        for ghost in self.ghosts:
            ghost.position = self.ghostRect.center

