#!/usr/bin/env python

# Copyright (c) Twisted Matrix Laboratories.
# See LICENSE for details.

from twisted.internet.protocol import Protocol, Factory
from twisted.internet import reactor

### Protocol Implementation

class Echo(Protocol):
    def dataReceived(self, data):
        self.transport.write(data)

class EchoFactory(Factory):
    def buildProtocol(self, addr):
        return Echo()

reactor.run()

if __name__ == '__main__':
    main()
