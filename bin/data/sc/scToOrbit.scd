// SC a Orbit

p = ProxySpace.push(s.boot);
s.meter;
p.quant = 4;
p.makeTempoClock((120/120));
m = NetAddr("127.0.0.1", 5612); // para OpenFrameworks

m.sendMsg("/draw", "box");
m.sendMsg("/clear", "box");

s.meter
~sin.play;
~sin = {Pan2.ar(SinOsc.ar(400)) * ~deca};
~deca = {Decay.kr(Demand.kr(Impulse.kr(~tempo*4), 0, Dseq([1, 0], inf)) * Impulse.kr(~tempo*4), 0.25)}

//
OSCdef(\glitchTwist).free;

(
OSCdef(\orbitX, {|msg|
	var data = msg[3];
	m.sendMsg("/camOrbitX", data);
}, '/orbitXDef');
)

~orbitXSend = {SendReply.kr(Impulse.kr(30), '/orbitXDef', ~orbitX.kr)};

~orbitX = {Demand.kr(Impulse.kr(~tempo*2), 0, Dseq([13, -27, 55, 110, -160] * 2,inf))};

//OSCdef(\glitch2).free;


(
OSCdef(\orbitY, {|msg|
	var data = msg[3];
	m.sendMsg("/camOrbitY", data);
}, '/orbitYDef');
)

~orbitYSend = {SendReply.kr(Impulse.kr(30), '/orbitYDef', ~orbitY.kr)};

~orbitY = {Demand.kr(Impulse.kr(~tempo*1), 0, Dseq([13, -27, 55, 110, -160],inf))};

//OSCdef(\glitch2).free;

m.sendMsg("/feedbackEnable");

(
OSCdef(\fbX, {|msg|
	var data = msg[3];
	m.sendMsg("/feedbackSetPositionX", data);
}, '/fbXDef');
)

~fbXSend = {SendReply.kr(Impulse.kr(60), '/fbXDef', ~fbX.kr)};

~fbX = {Demand.kr(Impulse.kr(~tempo*2), 0, Dseq([40, -40, 20, -20, 0, 100, -100]*4,inf))};

~fbX = {Lag.kr(Demand.kr(Impulse.kr(~tempo*2), 0, Drand([40, -40, 20, -20, 0, 100, -100]*4,inf)), 2)};


(
OSCdef(\fbY, {|msg|
	var data = msg[3];
	m.sendMsg("/feedbackSetPositionY", data);
}, '/fbYDef');
)

~fbYSend = {SendReply.kr(Impulse.kr(60), '/fbYDef', ~fbY.kr)};

~fbY = {Demand.kr(Impulse.kr(~tempo*2), 0, Dseq([40, -40, 20, -20, 0, 100, -100]*4,inf))};

~fbY = {Lag.kr(Demand.kr(Impulse.kr(~tempo*2), 0, Dseq([40, -40, 20, -20, 0, 100, -100]*4,inf)), 1)};



(
OSCdef(\glitchCon, {|msg|
	var data = msg[3];
	m.sendMsg("/glitchConvergence", data);
}, '/glitchConDef');
)

~glitchConSend = {SendReply.kr(Impulse.kr(60), '/glitchConDef', ~glitchCon.kr)};

~glitchCon = {Demand.kr(Impulse.kr(~tempo*4), 0, Dseq([1, 0],inf))};

(
OSCdef(\glitchSlitscan, {|msg|
	var data = msg[3];
	m.sendMsg("/glitchSlitscan", data);
}, '/glitchSlitscanDef');
)

~glitchSlitscanSend = {SendReply.kr(Impulse.kr(60), '/glitchSlitscanDef', ~glitchSlitscan.kr)};

~glitchSlitscan = {Demand.kr(Impulse.kr(~tempo*1), 0, Dseq([0, 1],inf))};
