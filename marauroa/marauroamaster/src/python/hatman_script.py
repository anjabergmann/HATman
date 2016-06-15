from marauroa.game.python import *

class hatmanRP(PythonRP):
  def __init__(self, zone):
    self._zone=zone

  def execute(self, id, action):
    return 0

  def nextTurn(self):
      pass

  def onInit(self, object):
      return 0

  def onExit(self, objectid):
      return 0

  def onTimeout(self, objectid):
      return 0

class hatmanZone(PythonZoneRP):
    def __init__(self, zone):
        self._zone=zone

    def onInit(self):
        pass

    def onFinish(self):
        pass

    def serializeMap(self, objectid):
        return java.util.LinkedList()

class hatmanAI(PythonAIRP):
    def __init__(self, zone, sched):
        self._zone=zone
        self._sched=sched

    def onCompute(self, timelimit):
        pass
