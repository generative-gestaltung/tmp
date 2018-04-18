var NODE_ID = 0;
var EDGE_ID = 0;

var DOM_ELEMS_GUI = [];
var DOM_ELEMS = {
  "nodes":{},
  "inputs":{},
  "outputs":{}
};

var state = {
  dragTmp : {

  },

  edgeTmp : {
    startNode : -1,
    startPort : -1,
    endNode : -1,
    endPort : -1,
    started : false
  }
}


var Patch = function() {
  this.nodes = {};
  this.edges = {};
}


Patch.prototype.addNode = function (data) {

  let newNode = new Node (NODE_ID, data);
  this.nodes[NODE_ID] = newNode;

  return newNode;
}


Patch.prototype.addEdge = function (startNodeId, startPortId, endNodeId, endPortId) {

  let newEdge = new Edge (EDGE_ID, startNodeId, startPortId, endNodeId, endPortId);
  this.edges[EDGE_ID] = newEdge;
  EDGE_ID += 1;
}


var Node = function (id, data) {
  this.id = id;
  this.name = data.name;
  this.inputs = [];
  this.outputs = [];

  let idTmp = 0;
  for (let inp of data.inputs) {
    this.inputs.push(new Input(idTmp, this, inp));
    idTmp += 1;
  }

  idTmp = 0;
  for (let out of data.outputs) {
    this.outputs.push(new Output(idTmp, this, out));
    idTmp += 1;
  }
}



// TODO UNPACK
var Input = function (id, node, data) {
  this.id = id;
  this.nodeId = node.id;

  this.name = data.name;
  this.type = data.type;

  this.value = 0;
}


var Output = function (id, node, data) {
  this.id = id;
  this.nodeId = node.id;

  this.name = data.name;
  this.type = data.type;

  this.value = 0;
}


var Edge = function (id, startNodeId, startPortId, endNodeId, endPortId) {

  this.id = id;
  this.startPortId = startPortId;
  this.endPortId = endPortId;

  this.startNodeId = startNodeId;
  this.endNodeId = endNodeId;
}

startEdge = function (div, type) {

  if (type=="output") {
    if (state.edgeTmp.startNode != -1) {
      this.cancelEdge();
      return;
    }

    state.edgeTmp.startDom = div;
    state.edgeTmp.startNode = div.parentNode.parentNode.id;
    state.edgeTmp.startPort = div.id;

    state.edgeTmp.started = true;
  }

  if (type=="input") {
    if (state.edgeTmp.endNode != -1 || state.edgeTmp.startNode == -1) {
      return;
    }

    state.edgeTmp.endNode = div.parentNode.parentNode.id;
    state.edgeTmp.endPort = div.id;


    console.log(this.patch.addEdge)
    this.patch.addEdge (state.edgeTmp.startNode, state.edgeTmp.startPort, state.edgeTmp.endNode, state.edgeTmp.endPort);


    this.cancelEdge();
  }
}


function cancelEdge () {

  state.edgeTmp.started = false;
  state.edgeTmp.startNode = -1;
  state.edgeTmp.startPort = -1;

  state.edgeTmp.endNode = -1;
  state.edgeTmp.endPort = -1;
}


function updateCtx () {

  context.clearRect(0,0,800,800);
  context.strokeStyle = "#00f";
  context.lineWidth = 2;

  if (state.edgeTmp.started) {
    let p0 = state.edgeTmp.startDom.getBoundingClientRect();
    let p1 = state.edgeTmp.mousePos;
    context.beginPath();
    context.moveTo (p0.x+5, p0.y+10);
    context.lineTo (p1.x+5, p1.y+10);
    context.stroke();
  }

  for (let ee in this.patch.edges) {
    let e = this.patch.edges[ee];


    let p0 = DOM_ELEMS.outputs[e.startNodeId][e.startPortId].getBoundingClientRect();
    let p1 = DOM_ELEMS.inputs[e.endNodeId][e.endPortId].getBoundingClientRect();
    context.beginPath();
    context.moveTo (p0.x+5, p0.y+10);
    context.lineTo (p1.x+5, p1.y+10);
    context.stroke();

  }
}

function initEvents() {

  document.getElementById("body").onclick = (e)=> {
    if (e.target.className == "input" || e.target.className == "output") {
      startEdge (e.target, e.target.className);
    }
    else {
      cancelEdge();
    }
  }

  document.getElementById("body").onmousemove = (e)=> {

    state.edgeTmp.mousePos = {
      x:e.x,
      y:e.y
    }
    updateCtx();
  }
}


var PatchRenderer = function (patch) {
  this.patch = patch;
}


PatchRenderer.prototype.createUsrInputDom = function (id, input) {
  let i = document.createElement("div");
  i.className = "usrInput";
  innerHTML = input.name;

  let divInput = document.createElement("input");
  divInput.setAttribute("type", "text");
  divInput.onchange = (e)=> {
    let nodeID = e.target.parentNode.parentNode.parentNode.id;
    let portID = e.target.parentNode.id;

    let node = patch.nodes[nodeID];
    node.inputs[portID].value = divInput.value;
  }
  i.appendChild(divInput);
  divInput.value = input.value;
  i.id = id;

  return i;
}

PatchRenderer.prototype.createInputDom = function (id, input) {
  let i = document.createElement("div");
  i.className = "input";

  i.innerHTML = input.name;
  i.id = input.id;

  return i;
}


PatchRenderer.prototype.createOutputDom = function (id, name, nodeId) {
  let o = document.createElement("div");
  o.className = "output";
  o.innerHTML = name;
  o.id = id;

  return o;
}

PatchRenderer.prototype.createOutputDomGuiTxt = function (id, output) {
  let o = document.createElement("div");

  o.port = output;
  o.className = "scope";
  o.innerHTML = "KKKK";
  o.id = output.id;

  return o;
}



PatchRenderer.prototype.createPatch = function (patch) {

  this.patch.nodes = patch.nodes;
  this.patch.edges = patch.edges;

  log (patch);

  NODE_ID = 0;
  EDGE_ID = 0;
  EDGE_ID = Object.keys(this.patch.edges).length;
  for (let n in this.patch.nodes) {
    if (this.patch.nodes[n].id > NODE_ID)
      NODE_ID = this.patch.nodes[n].id;
    this.createNodeDom (this.patch.nodes[n]);
  }

}


PatchRenderer.prototype.createNodeDom = function (node) {

  let div = document.createElement("div");

  DOM_ELEMS.nodes[node.id] = div;
  DOM_ELEMS.outputs[node.id] = {};
  DOM_ELEMS.inputs[node.id] = {};

  div.className = "node";
  div.id = node.id;
  div.setAttribute("draggable", true);
  div.innerHTML = node.name;

  div.style.left = node.gui.x+"px";
  div.style.top = node.gui.y+"px";


  let inputs = document.createElement("div");
  inputs.className = "inputs";
  inputs.style.width="100px";

  let outputs = document.createElement("div");
  outputs.className = "outputs";

  div.appendChild(inputs);
  div.appendChild(outputs);

  for (let ii in node.inputs) {
    let i = node.inputs[ii];

    let inp;

    if (i.type=="usr")
      inp = this.createUsrInputDom (ii, i);
    else
      inp = this.createInputDom (ii, i);

    inputs.appendChild(inp);

    DOM_ELEMS.inputs[div.id][ii] = inp;
  }

  for (let oo in node.outputs) {
    let o = node.outputs[oo];

    let out;
    if (o.type!="guiTxt") {
      out = this.createOutputDom (oo, o.name);
    }
    else {
      out = this.createOutputDomGuiTxt (oo, o);
      DOM_ELEMS_GUI.push(out);
    }

    outputs.appendChild(out);

    DOM_ELEMS.outputs[div.id][oo] = out;
  }

  div.ondragstart = (e) => {
    state.dragTmp.dx = div.offsetLeft - e.x;
    state.dragTmp.dy = div.offsetTop - e.y;
  }

  div.ondrag = (e) => {
    div.style.left = (e.x + state.dragTmp.dx)+"px";
    div.style.top  = (e.y + state.dragTmp.dy)+"px";
    updateCtx();
  }

  div.ondragend = (e) => {
    let x = e.x + state.dragTmp.dx;
    let y = e.y + state.dragTmp.dy;

    div.style.left = x+"px";
    div.style.top  = y+"px";

    this.patch.nodes[div.id]["gui"] = {
      x : x,
      y : y
    };
  }

  document.getElementById("patch").appendChild(div);

  return div;
}


PatchRenderer.prototype.addNode = function (category, type) {

  NODE_ID += 1;
  let newNode = this.patch.addNode (NODE_TYPES[category][type]);
  newNode["gui"] = {
    x: Math.round(Math.random()*400),
    y: Math.round(Math.random()*400)
  };

  let div = this.createNodeDom (newNode);
  DOM_ELEMS.nodes[div.id] = div;
}


PatchRenderer.prototype.updateGui = function () {
  for (let elem of DOM_ELEMS_GUI) {
    elem.innerHTML = elem.port.value;
  }
}
