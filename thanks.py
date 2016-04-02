#
# cocos2d
# http://python.cocos2d.org
#

from __future__ import division, print_function, unicode_literals

# This code is so you can run the samples without installing the package
import sys
import os
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..'))
#

import cocos
from cocos.actions import *


class HelloWorld(cocos.layer.Layer):

    def __init__(self):
        super(HelloWorld, self).__init__()

        # a cocos.text.Label is a wrapper of pyglet.text.Label
        # with the benefit of being a cocosnode
        label = cocos.text.Label('Thank you for your attention!',
                                 font_name='Times New Roman',
                                 font_size=32,
                                 anchor_x='center', anchor_y='center')

        label.position = 320, 240
        self.add(label)

        # similar to cocos.text.Label, a cocos.sprite.Sprite
        # is a subclass of pyglet.sprite.Sprite with the befits of
        # being a CocosNode.
        sprite = cocos.sprite.Sprite('pacman1.png')
        sprite2 = cocos.sprite.Sprite('pacman2.png')

        # sprite in the center of the screen (default is 0,0)
        sprite.position = 1200, 240
        sprite2.position = 1200, 240

        sprite.scale = 0.4
        sprite2.scale = 0.4

        # add the sprite as a child, but with z=1 (default is z=0).
        # this means that the sprite will be drawn on top of the label
        self.add(sprite, z=1)
        self.add(sprite2, z=1)

        move = MoveTo((300, 240), 5)


        # tell the sprite to scaleback and then scale, and repeat these 2 actions forever
        sprite.do(move)
        sprite2.do(move | Repeat(Blink(1, 0.3)))


if __name__ == "__main__":
    # director init takes the same arguments as pyglet.window
    cocos.director.director.init()

    # We create a new layer, an instance of HelloWorld
    hello_layer = HelloWorld()

    # A scene that contains the layer hello_layer
    main_scene = cocos.scene.Scene(hello_layer)

    # And now, start the application, starting with main_scene
    cocos.director.director.run(main_scene)

    # or you could have written, without so many comments:
    #      director.run( cocos.scene.Scene( HelloWorld() ) )
