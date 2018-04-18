var id = 0;

var state = {
  dragTmp : {

  },
  edgeTmp : {
    input : {},
    output : {},
    started : false
  }
}

var Patch = function() {
  this.nodes = {};
  this.edges = {};
  this.inputs = {};
  this.outputs = {};
}

var DOM_ELEMS = [];

Patch.prototype.addNode = function (id, data) {
  this.nodes[id] = new Node(data);
}

Patch.prototype.addEdge = function (id, startId, startName, endId, endName) {
  this.edges[id] = new Edge (startId, startName, endId, endName);
}

var Node = function (data) {
  this.name = data.name;
  this.inputs = data.inputs;
  this.outputs = data.outputs;
}

var Edge = function (startId, startName, endId, endName) {
  this.startId = startId;
  this.endId = endId;
  this.startName = startName;
  this.endName = endName;
}

var Input = function (parent, id) {
  this.parent = parent;
  this.id = id;
}

var Output = function (parent, id) {
  this.parent = parent;
  this.id = id;
}


var PatchRenderer = function () {

  this.patch1 = new Patch();

  this.dom = {
    patch : document.getElementById("patch")
  };

  this.patch = {
    nodes:{},
    edges:{}
  };

  this.patchDom = {
    nodes:{},
    edges:{}
  };


  document.getElementById("body").onclick = (e)=> {
    console.log(e.x+" "+e.y);
    if (e.target.className == "input" || e.target.className == "output") {

      this.startEdge (e.target, e.target.className);
    }
    else {
      this.cancelEdge();
    }
  }

  document.getElementById("body").onmousemove = (e)=> {

    state.edgeTmp.mousePos = {
      x:e.x,
      y:e.y
    }
    this.updateCtx();
  }
}


PatchRenderer.prototype.createInputDom = function(inp) {
  let i = document.createElement("div");
  i.className = "input";
  i.innerHTML = inp
  i.id = id;

  return i;
}


PatchRenderer.prototype.createOutputDom = function(out) {
  let o = document.createElement("div");
  o.className = "output";
  o.innerHTML = out;
  o.id = id;

  return o;
}

PatchRenderer.prototype.createNodeDom = function (node) {

  let div = document.createElement("div");
  div.className = "node";
  div.id = id;
  id += 1;
  div.setAttribute("draggable", true);
  div.innerHTML = node.name;

  div.style.left = Math.random()*400+"px";
  div.style.top = Math.random()*400+"px";

  let inputs = document.createElement("div");
  inputs.className = "inputs";
  inputs.style.width="100px";

  let outputs = document.createElement("div");
  outputs.className = "outputs";

  div.appendChild(inputs);
  div.appendChild(outputs);

  for (let i of node.inputs) {
    let inp = this.createInputDom(i);

    this.patch1.inputs[id] = "bla";
    this.patch1.inputs[id] = {
      id : id,
      parent: div.id
    };
    id += 1;
    inputs.appendChild(inp);
  }

  for (let o of node.outputs) {
      let out = this.createOutputDom(o);
      this.patch1.outputs[id] = {
        id : id,
        parent: div.id
      };
      id += 1;
      outputs.appendChild(out);
  }


  div.ondragstart = (e) => {
    state.dragTmp.dx = div.offsetLeft - e.x;
    state.dragTmp.dy = div.offsetTop - e.y;
  }

  div.ondrag = (e) => {
    div.style.left = (e.x + state.dragTmp.dx)+"px";
    div.style.top  = (e.y + state.dragTmp.dy)+"px";
    patchRenderer.updateCtx();
  }

  div.ondragend = (e) => {
  div.style.left = (e.x + state.dragTmp.dx)+"px";
  div.style.top  = (e.y + state.dragTmp.dy)+"px";
  }


  return div;
}


PatchRenderer.prototype.addNode = function (type, node) {
  let n = NODE_TYPES[type][node];

  this.patch1.addNode (id, n);
  console.log(this.patch1);

  let div = this.createNodeDom (n);
  this.patch.nodes[id] = n;
  id += 1;
  this.dom.patch.appendChild (div);
}

PatchRenderer.prototype.updateCtx = function() {

  context.clearRect(0,0,800,800);
  context.strokeStyle = "#00f";
  context.lineWidth = 2;

  if (state.edgeTmp.started) {
    let p0 = state.edgeTmp.input.getBoundingClientRect();
    let p1 = state.edgeTmp.mousePos;
    context.beginPath();
    context.moveTo (p0.x+5, p0.y+10);
    context.lineTo (p1.x+5, p1.y+10);
    context.stroke();
  }

  for (let ee in this.patchDom.edges) {
    let e = this.patchDom.edges[ee];
    let p0 = e.input.getBoundingClientRect();
    let p1 = e.output.getBoundingClientRect();
    context.beginPath();
    context.moveTo (p0.x+5, p0.y+10);
    context.lineTo (p1.x+5, p1.y+10);
    context.stroke();
  }
}


PatchRenderer.prototype.startEdge = function (div, type) {

  if (type=="input") {
    if (state.edgeTmp.input.id !== undefined) {
      this.cancelEdge();
      return;
    }
    state.edgeTmp.input = div;
    state.edgeTmp.started = true;
  }
  if (type=="output") {
    if (state.edgeTmp.output.id !== undefined || state.edgeTmp.input.id === undefined) {
      return;
    }
    state.edgeTmp.output = div;

    //this.patch1.addEdge (id,)
    //this.createEdge (state.edgeTmp.input, state.edgeTmp.output);

    this.patch1.addEdge (id, this.patch1.inputs[state.edgeTmp.input.id].id, this.patch1.inputs[state.edgeTmp.input.id].id,
                             this.patch1.outputs[state.edgeTmp.output.id].id, this.patch1.outputs[state.edgeTmp.output.id].id);
    console.log (this.patch1.outputs[state.edgeTmp.output.id].parent);
  }
}



/*
PatchRenderer.prototype.createEdge = function (input, output) {

  this.cancelEdge();

  this.patch.edges[id] = {
    input:input.id,
    output:output.id
  };

  this.patchDom.edges[id] = {
    input:input,
    output:output
  };

  id += 1;
}
*/

PatchRenderer.prototype.cancelEdge = function () {

  state.edgeTmp.started = false;
  state.edgeTmp.input = {};
  state.edgeTmp.output = {};
}
