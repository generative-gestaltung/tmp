import pyaudio
import wave
import sys
from random import random
from math import sin
from threading import Thread, Timer
from datetime import datetime


p = pyaudio.PyAudio()



class AudioGenerator:
    def __init__(self) :

        self.f = 1
        self.CHUNK = 64
        self.phase = 0
        self.stream = p.open(format=8, channels=2, rate=48000, output=True)

        self.currentTime = 0
        self.lastTime = 0

    def calcNextChunk (self) :
        ddd = []
        for i in range(0, self.CHUNK) :
            self.phase += (self.f) / self.CHUNK
            ddd.append(int(sin(self.phase)*60+60))

        return bytes(ddd)

    def _start(self):

        self.currentTime = datetime.now()
        self.lastTime = datetime.now()

        while True:
            x = self.calcNextChunk ()
            self.stream.write(x)
            self.currentTime = datetime.now()
            print ((self.currentTime - self.lastTime).microseconds)
            self.lastTime = self.currentTime

    def start (self):

        Timer(0.00001, self._start).start()




audio = AudioGenerator()
audio.start()

#stream.stop_stream()
#stream.close()
#p.terminate()
