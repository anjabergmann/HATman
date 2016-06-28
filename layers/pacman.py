from cocos.layer import Layer
from cocos.sprite import Sprite
from cocos.rect import Rect
from cocos.actions import *
from pyglet.gl import *
from pyglet.window import key
from char import CharLayer

class PacmanLayer(CharLayer):
    # enable pyglet events
    is_event_handler = True

    def __init__(self):
        super().__init__()

        # create and add Sprites for pacman
        self.pacman1 = Sprite(pyglet.resource.image("images/pacman1.png"))
        self.pacman2 = Sprite(pyglet.resource.image("images/pacman2.png"))

        self.pacmans = []

        self.pacmans.append(self.pacman1)
        self.pacmans.append(self.pacman2)

        self.charRect = Rect(40, 40, self.pacman1.width * 0.05, self.pacman1.height * 0.05)
        self.charRect.center = (40, 40);

        for pacman in self.pacmans:
            self.add(pacman)
            pacman.position = self.charRect.center
            pacman.scale = 0.05

        # Animate pacman
        self.pacman2.do(Repeat(Blink(1, 0.3)))

        # print("INFO pacman.top ", self.charRect.top)
        # print("INFO pacman.bottom ", self.charRect.bottom)
        # print("INFO pacman.left ", self.charRect.left)
        # print("INFO pacman.right ", self.charRect.right)
        # print("INFO pacmanRect.x ", self.charRect.x)
        # print("INFO pacmanRect.y ", self.charRect.y)


    # _______________________________________________
    #
    # Move and rotate pacman
    # _______________________________________________

    # Method is called with schedule() on every new frame
    def update(self, director):

        if self.direction == key.RIGHT:
            self.pacman1.rotation = None
            self.charRect.x += 2
        elif self.direction == key.LEFT:
            self.pacman1.rotation = 180
            self.charRect.x -= 2
        elif self.direction == key.UP:
            self.pacman1.rotation = 270
            self.charRect.y += 2
        elif self.direction == key.DOWN:
            self.pacman1.rotation = 90
            self.charRect.y -= 2

        for pacman in self.pacmans:
            pacman.position = self.charRect.center


# ___________________________________________________________________________________________________________
