from websocket_server import WebsocketServer
import SimpleHTTPServer
import SocketServer
import signal
import sys
from threading import Thread, Timer
from time import sleep, time
import json
from nodes import *
from audio import *
import datetime

HTTP_PORT = 8000
WS_PORT = 9001
PATCH_IMPL = {}
PATCH = {"nodes":{},"edges":{}}
RUN_PATCH = True


Handler = SimpleHTTPServer.SimpleHTTPRequestHandler
httpd = SocketServer.TCPServer(("", HTTP_PORT), Handler)
server = WebsocketServer(WS_PORT)



def signal_handler(signal, frame):
	global httpd
	global server
	global RUN_PATCH

	RUN_PATCH = False;
	print('shutdown http and WS servers')
	httpd.shutdown()
	server.server_close()
	print("ok")
	sys.exit(0)

signal.signal(signal.SIGINT, signal_handler)
#signal.pause()



def updatePatch():

	global FRAME_CNT
	global PATCH_IMPL
	global PATCH
	dt = 0

	lastTime = datetime.datetime.now()#

	while RUN_PATCH :
		sleep(0.0001)
		currentTime = datetime.datetime.now()
		dt += (currentTime-lastTime).microseconds / 1000.
		lastTime = currentTime

		if (dt >= 1):
			dt -= 1
			tic(dt)

def tic (dt):

	for key_edge in PATCH["edges"]:
		e = PATCH["edges"][key_edge]
		PATCH["nodes"][e["endNodeId"]]["inputs"][int(e["endPortId"])]["value"] = float(PATCH["nodes"][e["startNodeId"]]["outputs"][int(e["startPortId"])]["value"])

	for key_node in PATCH_IMPL:

		PATCH_IMPL[key_node].setInputs (PATCH["nodes"][key_node]["inputs"])
		PATCH_IMPL[key_node].setOutputs (PATCH["nodes"][key_node]["outputs"])
		PATCH_IMPL[key_node].update(dt)

	#if RUN_PATCH:
	#	Timer(0.1, updatePatch).start()



def start_http_server():
	print ("serving at port", HTTP_PORT)
	httpd.serve_forever()

thread = Thread (target = start_http_server)
thread.start()

#thread1 = Thread (target = update_patch)
Timer(0.1, updatePatch).start()

# Called for every client connecting (after handshake)
def new_client(client, server):
	print("New client connected and was given id %d" % client['id'])
	server.send_message_to_all("Hey all, a new client has joined us")


# Called for every client disconnecting
def client_left(client, server):
	print("Client(%d) disconnected" % client['id'])


# Called when a client sends a message
def message_received(client, server, message):
	global PATCH
	#if len(message) > 200:
	#	message = message[:200]+'..'
	#print("Client(%d) said: %s" % (client['id'], message))

	MSG = json.loads(message)

	if MSG["cmd"] == "SAVE" :
		file = open (MSG["data"], "w")
		file.write (json.dumps(PATCH))
		file.close()

	if MSG["cmd"] == "PATCH" :
		PATCH = MSG["data"]
		for key in PATCH["nodes"]:
			if key not in PATCH_IMPL:
				PATCH_IMPL[key] = classes[PATCH["nodes"][key]["name"]]()



server.set_fn_new_client(new_client)
server.set_fn_client_left(client_left)
server.set_fn_message_received(message_received)
server.run_forever()
