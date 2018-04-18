var NODE_TYPES = {

    "inputs" : {
      "encoders":{
        name: "encoders",
        inputs: [],
        outputs: [
          {name:"v0", type:"xxx"},
          {name:"v1", type:"xxx"},
          {name:"v2", type:"xxx"},
          {name:"v3", type:"xxx"}
        ]
      },
      "midi_in":{
        name: "midi_in",
        inputs: [],
        outputs: []
      }
    },
    "sources": {
      "lfo":{
        name: "lfo",
        inputs: [{name:"freq", type:"frt"}, {name:"reset", type:"trg"}],
        outputs: [{name:"sin", type:"frt"}, {name:"tri", type:"frt"}, {name:"sq", type:"frt"}]
      },
      "envelope":{
        name: "envelope",
        inputs: [{name:"trigger", type:"trg"}],
        outputs: [{name:"value", type:"frt"}]
      },
      "const":{
        name: "const",
        inputs: [{name:"value", type:"usr"}],
        outputs: [{name:"out", type:"frt"}]
      },
      "clock":{
        name: "clock",
        inputs: [{name:"speed", type:"frt"}, {name:"phase", type:"frt"}, {name:"reset", type:"trg"}],
        outputs: [{name:"out", type:"frt"}]
      },
      "noise":{
        name: "noise",
        inputs: [],
        outputs: [{name:"out", type:"frt"}]
      }

    },
    "operators" : {
      "map":{
        name: "map",
        inputs: [{name:"in", type:"frt"}],
        outputs: [{name:"out", type:"frt"}]
      },
      "sample_hold":{
        name: "sample_hold",
        inputs: [{name:"value", type:"frt"}, {name:"trigger", type:"trg"}],
        outputs: [{name:"out", type:"frt"}]
      },
      "scale":{
        name: "scale",
        inputs: [{name:"in", type:"frt"}, {name:"scale", type:"frt"}],
        outputs: [{name:"out", type:"frt"}]
      },
      "add":{
        name: "add",
        inputs: [{name:"in0", type:"frt"}, {name:"in1", type:"frt"}],
        outputs: [{name:"out", type:"frt"}]
      }
    },
    "outputs" : {
      "cv_out":{
        name: "cv_out",
        inputs: [{name:"cv0", type:"frt"}, {name:"cv1", type:"frt"}, {name:"cv2", type:"frt"}, {name:"cv3", type:"frt"}],
        outputs: []
      },
      "trigger_out":{
        name: "trigger_out",
        inputs: [{name:"tr0", type:"frt"}, {name:"tr1", type:"frt"}, {name:"tr2", type:"frt"}, {name:"tr3", type:"frt"}],
        outputs: []
      },
      "scope":{
        name: "scope",
        inputs: [{name:"value", type:"frt"}],
        outputs: [{name:"scope", type:"guiTxt"}]
      },
      "audio_test_gen":{
        name: "audio_test_gen",
        inputs: [{name:"freq", type:"frt"}],
        outputs: []
      }
    }
};



var Const = function() {

}

Const.prototype.init = function () {

}

Const.prototype.update = function (dt) {
  this.out = this.value;
}

var Lfo = function() {
  this.phase = 0;

}

Lfo.prototype.init = function () {

}

Lfo.prototype.update = function (dt) {

  this.sin = Math.sin (this.phase)*0.5+0.5;
  this.tri = this.sin;
  this.sq = this.sin;


  this.phase += dt*this.freq;
}

var Scope = function() {

}

Scope.prototype.init = function () {

}

Scope.prototype.update = function (dt) {
  this.scope = this.value;
}

var Add = function() {

}

Add.prototype.init = function () {

}

Add.prototype.update = function (dt) {
  this.out = this.in0 + this.in1;
}

var Clock = function() {

}

Clock.prototype.init = function () {
  this.phase = 0;
  this._TRIGGER_INPUTS.reset.onRisingEdge = ()=> {
    console.log("reset");
  };
}


Clock.prototype.update = function (dt) {

}


/*
var Clock = function() {

}

Clock.prototype.update = function (dt) {

}
*/
// TODO dynamic class and file management
var NODE_IMPL = {

  "const" : Const,
  "lfo" : Lfo,
  "scope" : Scope,
  "add" : Add,
  "clock" : Clock

};

/*
  "const":{
    file : "simple_nodes.js",
    className : "Const"
  },
  "lfo":{
    file : "lfo.js",
    className : "Lfo"
  },
  "clock":{

  },
  "scale":{

  },
  "map":{

  }
}
*/
