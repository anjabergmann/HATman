import cocos
from cocos.director import director
from cocos.scene import Scene
from cocos.layer import Layer
from cocos.menu import *
from pyglet.gl import *
from pyglet.window import key

from hatman import GameScene


class Startmenu(Menu):
	def __init__(self):
		
		super(Startmenu, self).__init__()

		#sets the vertical and horizontal alignment
		self.menu_valign = BOTTOM
		self.menu_halign = LEFT

			# Each MenuItem has text to display, and a callback method
			# The callback method we pass in, gets called when a user clicks on the MenuItem
		nameitem = EntryMenuItem("Name: ", self.enter_name, "Lucy", max_length=10)
		hostitem = EntryMenuItem("Host: ", self.enter_host, "127.0.0.1", max_length=15)
		portitem = EntryMenuItem("Port: ", self.enter_port, "8080", max_length=7)
		startitem = MenuItem("Start Game", self.start_game)
		exititem = MenuItem("Exit", self.on_quit)

		#create a list filled with MenuItem objects
		menu_items = [nameitem, hostitem, portitem, startitem, exititem]

		#create menu with that list
		self.create_menu(menu_items)
		

#CALLBACK-METHODS-----------
	def enter_name(self, name):
		print("enter a name: ", name)
		#TODO pass name to game

	def enter_host(self, host):
		print("enter a host: ", host)
		#TODO pass host to game

	def enter_port(self, port):
		print("enter a port: ", port)
		#TODO pass port to game


	def start_game(self):
		print("start game")

		#SOMETHING LIKE THIS AFTER INTEGRATING INTO HATMAN.PY:
		#main_scene = GameScene()
		#director.push(main_scene)

		#NOT TO FORGET: deleting 'director.init(resizable=False, caption="HATman")' in hatman.py 
		#because we want to initialize the program here
		
		#TODO converting parseargs in hatman to passed variables from menu


	# IMPORTANT!
	# All your menus must have this on_quit function or they will not exit even if you press escape
	# (That means some fun Control+Alt+Tabing for you Windows users)
	def on_quit(self):
		print("exit the game")
		exit()

# class for displaying/handling the character images and not formatting them like the main menu
class ImageMenu(Menu):
	def __init__(self):
		
		super(ImageMenu, self).__init__()

		#sets the vertical and horizontal alignment for image menu
		self.menu_valign = BOTTOM
		self.menu_halign = RIGHT


		self.choose_char_item = ['Pacman', 'Pink Ghost', 'Blue Ghost', 'Orange Ghost', 'Red Ghost']

		pacman = pyglet.resource.image("images/pacman1.png")
		ghostpink = pyglet.resource.image("images/pink1.png")
		ghostblue = pyglet.resource.image("images/blue1.png")
		ghostorange = pyglet.resource.image("images/orange1.png")
		ghostred = pyglet.resource.image("images/red1.png")

		#create menu items with character images
		charpac = ImageMenuItem(pacman, self.choose_char, self.choose_char_item)
		chargpink = ImageMenuItem(ghostpink, self.choose_char, self.choose_char_item)
		chargblue =	ImageMenuItem(ghostblue, self.choose_char, self.choose_char_item)
		chargorange = ImageMenuItem(ghostorange, self.choose_char, self.choose_char_item)
		chargred = ImageMenuItem(ghostred, self.choose_char, self.choose_char_item)

		img_items = [charpac, chargpink, chargblue, chargorange, chargred]
		self.create_menu(img_items)


	def choose_char(self, choose_char_item):
		print("choose a character: ", choose_char_item)
		#TODO set and pass chosen characters to hatman



class StartscreenLayer(Layer):
	def __init__ (self):
		super( StartscreenLayer, self).__init__()

		titlelabel = cocos.text.Label('Let\'s play HATman!', 
			font_name='Times New Roman', 
			font_size=32, anchor_x='center', 
			anchor_y='center')
		# set the title-label at the top center of the screen
		titlelabel.position = 320,440
		self.add(titlelabel)

		startmenu = Startmenu()
		imgmenu = ImageMenu()
		self.add(imgmenu) #add image menu to layer
		self.add(startmenu) #add startmenu to layer





director.init(resizable=False, caption="HATman")


startscreen_layer = StartscreenLayer()

menu_scene = Scene(startscreen_layer)

director.run(menu_scene)

