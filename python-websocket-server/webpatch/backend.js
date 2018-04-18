var Backend = function (patch) {
    this.patch = patch;
    this.cnt = 0;
    this.impl = {};
}

Backend.prototype.updatePatch = function (patch) {
  this.patch = patch;
}


var TriggerIn = function() {
  this.lastValue = 0;
}

TriggerIn.update = function (value) {
  if (value>0 && this.lastValue <= 0) {
    if (this.onRisingEdge !== undefined) {
      this.onRisingEdge();
    }
  }

  if (value<=0 && this.lastValue > 0) {
    if (this.onFallingEdge !== undefined) {
      this.onFallingEdge();
    }
  }
  this.lastValue = value;
}

Backend.prototype.tic = function(dt) {

  // TODO: MOVE to backend
  for (let n in this.patch.nodes) {

    // CREATE CLASS 1st time
    if (this.impl[n] === undefined) {
      this.impl[n] = new NODE_IMPL[this.patch.nodes[n].name]();

      // CREATE VALUES 1st time
      for (let ii in this.patch.nodes[n].inputs) {
        let inp = this.patch.nodes[n].inputs[ii];
        if (true || inp.type=="frt") {
          this.impl[n][inp.name] = 0;
        //}
        //else if (inp.type=="trg") {
        //  this.impl[n]["triggerIns"][inp.name] = new TriggerIn(inp.name);
        }
      }
    }

    // UPDATE values every tic
    for (let ii in this.patch.nodes[n].inputs) {
      let inp = this.patch.nodes[n].inputs[ii];
      if (true || inp.type=="frt") {
        this.impl[n][inp.name] = parseFloat(inp.value);
      //}
      //else if (inp.type=="trg") {
      //  this.impl[n][inp.name] = parseFloat(inp.value);
      }
    }

    for (let oo in this.patch.nodes[n].outputs) {
      this.impl[n][this.patch.nodes[n].outputs[oo].name] = parseFloat(this.patch.nodes[n].outputs[oo].value);
    }

    this.impl[n].update(dt);

    for (let ii in this.patch.nodes[n].inputs) {
      this.patch.nodes[n].inputs[ii].value = this.impl[n][this.patch.nodes[n].inputs[ii].name];
    }

    for (let oo in this.patch.nodes[n].outputs) {
       this.patch.nodes[n].outputs[oo].value = this.impl[n][this.patch.nodes[n].outputs[oo].name];
    }
  }

  for (let ee in this.patch.edges) {
    let e = this.patch.edges[ee];
    this.patch.nodes[e.endNodeId].inputs[e.endId].value = this.patch.nodes[e.startNodeId].outputs[e.startId].value
  }
}
