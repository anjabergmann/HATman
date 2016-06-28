from cocos.layer import Layer
from cocos.sprite import Sprite
from cocos.rect import Rect
from cocos.actions import *
from pyglet.gl import *
from pyglet.window import key

class GhostLayer(Layer):
	# enable pyglet events
	is_event_handler = True

	#TODO