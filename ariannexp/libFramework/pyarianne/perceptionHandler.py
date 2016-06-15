

import arianne

class PerceptionListener:
    def __init__(self):
        pass
    def onAdded(self, object):
        pass
    def onBeforeModifiedAdded(self, object, previous):
        pass
    def onModifiedAdded(self, object):
        pass
    def onBeforeModifiedDeleted(self, object, previous):
        pass
    def onModifiedDeleted(self, object):
        pass
    def onDeleted(self, object):
        pass
    def onMyRPObject(self, changed, object):
        pass
    def onBeginPerception(self):
        pass
    def onEndPerception(self):
        pass
    def onClear(self):
        pass
    def onUnsync(self):
        pass
    def onSync(self):
        pass
        

class PerceptionHandler:
    __callback=None
    __previous=0
    __synced=False

    def __init__(self, function=None):
        self.__setCallback(function)
    
    def __setCallback(self, function):
        if function==None:
            self.__callback=PerceptionListener()
        else:
            self.__callback=function
        
    def applyPerception(self, perception, world):
        self.__callback.onBeginPerception()
        if perception.type==perception.SYNC:
            self.__previous=perception.timestamp
            self.__applyPerceptionAddedRPObjects(perception,world)
            self.__applyPerceptionMyRPObject(perception,world)

            if not self.__synced:
                self.__callback.onSync()
                self.__synced=True
        elif perception.type==perception.DELTA and self.__previous+1==perception.timestamp:
            self.__previous=perception.timestamp

            self.__applyPerceptionDeletedRPObjects(perception,world)
            self.__applyPerceptionModifiedRPObjects(perception,world)
            self.__applyPerceptionAddedRPObjects(perception,world)
            self.__applyPerceptionMyRPObject(perception,world)
        else:
            self.__callback.onUnsync()
            self.__synced=False
        self.__callback.onEndPerception()
                

    def __applyPerceptionAddedRPObjects(self, perception, world):
        if perception.type==perception.SYNC:
            self.__callback.onClear()
            world.objects.clear()

        for obj in perception.added:
            self.__callback.onAdded(obj)
            world.objects[obj.getInt("id")]=obj
            
    def __applyPerceptionDeletedRPObjects(self, perception, world):
        for obj in perception.deleted:
            self.__callback.onDeleted(obj)
            del world.objects[obj.getInt("id")]

    def __applyPerceptionModifiedRPObjects(self, perception, world):
        for obj in perception.modifiedDeleted:
            if world.objects.has_key(obj.getInt("id")):
                wobj=world.objects[obj.getInt("id")]
                self.__callback.onBeforeModifiedDeleted(obj,wobj)
                wobj.applyDeletedDifferences(obj)
                self.__callback.onModifiedDeleted(wobj)
            else:
                # Program error. It must never happen. It is a bug
                raise exception
            
        for obj in perception.modifiedAdded:
            if world.objects.has_key(obj.getInt("id")):
                wobj=world.objects[obj.getInt("id")]
                self.__callback.onBeforeModifiedAdded(obj,wobj)
                wobj.applyAddedDifferences(obj)
                self.__callback.onModifiedAdded(wobj)
            else:
                # Program error. It must never happen. It is a bug
                raise exception

    def __applyPerceptionMyRPObject(self, perception, world):
        if perception.myRPObjectChanged:
            id=perception.myRPObject.getInt("id")
            world.objects[id]=perception.myRPObject
            self.__callback.onMyRPObject(True,world.objects[id])
        else:
            self.__callback.onMyRPObject(False,None)
            
            


        
