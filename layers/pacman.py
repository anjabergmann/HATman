from cocos.layer import Layer
from cocos.sprite import Sprite
from cocos.rect import Rect
from cocos.actions import *
from pyglet.gl import *
from pyglet.window import key

class PacmanLayer(Layer):
    # enable pyglet events
    is_event_handler = True

    def __init__(self):
        super(PacmanLayer, self).__init__()

        # create and add Sprites for pacman
        self.pacman1 = Sprite(pyglet.resource.image("images/pacman1.png"))
        self.pacman2 = Sprite(pyglet.resource.image("images/pacman2.png"))

        self.pacmans = []

        self.pacmans.append(self.pacman1)
        self.pacmans.append(self.pacman2)

        self.pacmanRect = Rect(40, 40, self.pacman1.width * 0.05, self.pacman1.height * 0.05)
        self.pacmanRect.center = (40, 40);

        for pacman in self.pacmans:
            self.add(pacman)
            pacman.position = self.pacmanRect.center
            pacman.scale = 0.05

        # Animate pacman
        self.pacman2.do(Repeat(Blink(1, 0.3)))

        print("INFO pacman.top ", self.pacmanRect.top)
        print("INFO pacman.bottom ", self.pacmanRect.bottom)
        print("INFO pacman.left ", self.pacmanRect.left)
        print("INFO pacman.right ", self.pacmanRect.right)
        print("INFO pacmanRect.x ", self.pacmanRect.x)
        print("INFO pacmanRect.y ", self.pacmanRect.y)


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
    # Move and rotate pacman
    # _______________________________________________

    # Method is called with schedule() on every new frame
    def update(self, director):

        if self.direction == key.RIGHT:
            self.pacman1.rotation = None
            self.pacmanRect.x += 2
        elif self.direction == key.LEFT:
            self.pacman1.rotation = 180
            self.pacmanRect.x -= 2
        elif self.direction == key.UP:
            self.pacman1.rotation = 270
            self.pacmanRect.y += 2
        elif self.direction == key.DOWN:
            self.pacman1.rotation = 90
            self.pacmanRect.y -= 2

        for pacman in self.pacmans:
            pacman.position = self.pacmanRect.center


# ___________________________________________________________________________________________________________
