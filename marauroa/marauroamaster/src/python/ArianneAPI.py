 import pyarianne

def doStuffonCallback():
	print "Calling from Python!"

world=pyarianne.World()
perceptionHandler=pyarianne.PerceptionHandler()

print "Starting pyclient"

pyarianne.setIdleMethod(doStuffonCallback)
pyarianne.connectToArianne("127.0.0.1",3214)
if pyarianne.login("username","password"):
	chars=pyarianne.availableCharacters()

	if pyarianne.chooseCharacter(chars[0]):
		i=0
		while i<100:
			if pyarianne.hasPerception():
				perception=pyarianne.getPerception()
				perceptionHandler.applyPerception(perception, world)
				i=i+1

		pyarianne.logout()
	else:
		print "CAN'T CHOOSE: "+pyarianne.errorReason()
else:
	print "CAN'T LOGIN: "+pyarianne.errorReason()

print "Finishing pyclient"