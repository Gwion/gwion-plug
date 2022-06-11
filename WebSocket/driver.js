const URL = "ws://localhost:1111"
const BUFLEN = 1024;
const NCHAN = 2;
var ws;
var scriptNode;
var audioCtx;
var outsamples = new Float32Array(BUFLEN*NCHAN);
var volume = 0.5;

function gwionWsSetVolume(value) {
  volume = value;
}

function gwionWsFillBuffer(audioProcessingEvent) {
  ws.send("next");
  const outputBuffer = audioProcessingEvent.outputBuffer;
  for (var channel = 0; channel < NCHAN; channel++) {
    var outputData = outputBuffer.getChannelData(channel);
    for (var sample = 0; sample < BUFLEN; sample++)
{
//console.log(sample + channel*BUFLEN);
      outputData[sample] = outsamples[sample + channel*BUFLEN] * volume;
}
  }
}

function gwionWsOnOpen()  {
  audioCtx = new (window.AudioContext || window.webkitAudioContext)();
  scriptNode = audioCtx.createScriptProcessor(BUFLEN, 0, NCHAN);
  scriptNode.onaudioprocess = gwionWsFillBuffer;
  scriptNode.connect(audioCtx.destination);
}

function gwionWsOnMessage(e)  {
  if(e.data instanceof ArrayBuffer)  {
    outsamples = new Float32Array(e.data);
  }
}

function gwionWsOnClose()  {
  if(audioCtx) {
    scriptNode.disconnect();
    audioCtx.close();
    alert("Connection closed");
  }
}

function gwionWsInit() {
  ws = new WebSocket(URL);
  if(!ws)
    alert("Web socket server not found at url: " + URL);
  ws.onopen = gwionWsOnOpen;
  ws.onmessage = gwionWsOnMessage;
  ws.onclose = gwionWsOnClose;
//  ws.binaryType = "blob"
  ws.binaryType = "arraybuffer";
//  ws.binaryType = "binary";
}

function gwionWsOnLoad() {
  if ("WebSocket" in window)
    gwionWsInit();
  else
    alert("WebSocket NOT supported by your Browser!");
}
