from math import *
from audio import AudioGenerator

def tri (phase) :
    PI = 3.14159
    phase = phase % (2*PI)
    if phase <= PI:
        return phase / PI
    return ((2*PI)-phase) / PI


class Const:
    def __init__(self):
        self.out = 0

    def setInputs (self, inputs):
        self.inputs = inputs

    def setOutputs (self, outputs):
        self.outputs = outputs

    def update(self, dt):
        self.outputs[0]["value"] = self.inputs[0]["value"]


class Lfo:
    def __init__(self):
        self.phase = 0

    def setInputs (self, inputs):
        self.inputs = inputs

    def setOutputs (self, outputs):
        self.outputs = outputs

    def update (self, dt):
        self.outputs[0]["value"] = sin (self.phase)
        self.outputs[1]["value"] = tri (self.phase)
        self.outputs[2]["value"] = 0
        self.phase += dt * self.inputs[0]["value"]


class CV_Out:
    def __init__(self):
        return

    def setInputs (self, inputs):
        self.inputs = inputs

    def setOutputs (self, outputs):
        self.outputs = outputs

    def update(self, dt):
        f = open("cv_scope", "w")
        f.write ("xx")
        f.close()


class Trigger_Out:
    def __init__(self):
        self.time = 0

    def setInputs (self, inputs):
        self.inputs = inputs

    def setOutputs (self, outputs):
        self.outputs = outputs

    def update(self, dt):
        self.time += dt
        f = open("trigger_scope", "a")
        f.write (str(self.time)+" "+str(dt)+" "+str(self.inputs[0]["value"])+"\n")
        f.close()


class AudioGen:
    def __init__(self):
        self.time = 0
        self.ag = AudioGenerator()
        self.ag.start()

    def setInputs (self, inputs):
        self.inputs = inputs

    def setOutputs (self, outputs):
        self.outputs = outputs

    def update(self, dt):
        self.ag.f = float(self.inputs[0]["value"])
        self.time += dt


classes = {
    "const" : Const,
    "lfo" : Lfo,
    "cv_out" : CV_Out,
    "trigger_out" : Trigger_Out,
    "audio_test_gen" : AudioGen
}
