var Menu = function() {
  this.menuDiv = document.getElementById("menu");
  for (let type in NODE_TYPES) {
    let div = document.createElement("div");
    div.className = "menuContainer";
    div.innerHTML = type;
    let sel = document.createElement("select");

    for (let name in NODE_TYPES[type]) {
      let n = NODE_TYPES[type][name];
      let o = new Option();
      o.value = n.name;
      o.text = n.name;
      sel.options.add(o);
    }

    sel.onchange = (e)=> {
      patchRenderer.addNode (type, sel.options[sel.selectedIndex].text);
    }

    div.appendChild(sel);
    this.menuDiv.appendChild(div);
  }

  let button = document.createElement("button");
  button.onclick = () => {
    console.log(patch);
    sendCmd ("SAVE", "xxx.txt");
  };
  button.innerHTML = "SAVE";
  this.menuDiv.appendChild(button);

  button = document.createElement("button");
  button.onclick = () => {
    let req = new XMLHttpRequest();
    req.open("GET", "http://localhost:8000/xxx.txt", true);
    req.send();
    let cnt = 0;
    req.onreadystatechange = ()=> {
      try {

        if (cnt==0) {
          log(req.responseText)
          let p = JSON.parse(req.responseText);
          patchRenderer.createPatch(p);
        }
        cnt += 1;
      }
      catch(e) {

      }
    }
  };
  button.innerHTML = "LOAD";
  this.menuDiv.appendChild(button);
}
