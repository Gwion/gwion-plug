const URL = "ws://localhost:1111"
var ws;
var scriptNode;

function gwionWsFillBuffer(audioProcessingEvent) {
  ws.send("next");
  var outputBuffer = audioProcessingEvent.outputBuffer;
  for (var channel = 0; channel < NCHAN; channel++) {
    var outputData = outputBuffer.getChannelData(channel);
    for (var sample = 0; sample < BUFLEN; sample++)
      outputData[sample] = outsamples[sample + channel*BUFLEN] * volume;
  }
}

function gwionWsOnOpen()  {
  ws.send("next");
}

function gwionWsOnMessage(e)  {
/*
  if (e.data instanceof Blob)
    reader = new FileReader()
    reader.onload = function() {
    console.log(reader.result)
  }
  reader.readAsText(e.data)
  ws.send("next");
*/
}

function gwionWsOnClose()  {
  console.log("closing");
}

function gwionWsInit() {
  ws = new WebSocket(URL);
  if(!ws)
    alert("Web socket server not found at url: " + URL);
  ws.onopen = gwionWsOnOpen;
  ws.onmessage = gwionWsOnMessage;
  ws.onclose = gwionWsOnClose;
  ws.binaryType = "binary";
//  ws.binaryType = "blob"; // the default
}

function gwionWsOnLoad() {
  console.log("on load");
  if ("WebSocket" in window)
    gwionWsInit();
  else
    alert("WebSocket NOT supported by your Browser!");
}
