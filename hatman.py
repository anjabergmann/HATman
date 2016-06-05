from marauroamaster.build.classes.marauroa.server.game.python import *;

class HATmanPR(PythonRP):
	def __init__(self, zone):
		self.zone = zone;

	def execute (self, id, action):
		return 0;