Sporth s1 => dac;
s1.parse("0 p 0.01 port mtof 1 p 0.003 port 1 1 2 p 0.01 port fm 0.5 * dup dup 0.94 10000 revsc drop 0.1 * +");
s1.p(1, 0.5);


Sporth s2 => dac;
s2.parse("0 p 0.01 port mtof 1 p 0.003 port 1 1 2 p 0.01 port fm 0.5 * dup dup 0.94 10000 revsc drop 0.1 * +");
s2.p(1, 0.5);

//SinOsc s => dac;
110 => float bpm;
(60.0 / bpm) :: second => dur t;
[0, 1, 5, 7, 8] @=> int scale[];
0 => int stp;
0 => int nbars;
58 => int base;
3 => int block;

function void loop_me(Sporth s, int base)
{
	while(1)
	{
    if(stp == 0) {
        s.p(1, 0.7);
        nbars++;
    } else s.p(1, 0.4);
    if(nbars > block) {
        if(base == 60) 53 => base; else 60 => base;
        1 => nbars;
    }
    s.p(0, base + scale[stp] + 12 * Math.rand2(-1, 1));
    s.p(2, Math.rand2f(0.1, 3));
//	base + scale[stp] + 12 * Math.rand2(-1, 1) => Std.mtof => s.freq;
    t * 0.25 => now;
    (stp + 1) % scale.size() => stp;
//		if(maybe && maybe && maybe)
//			Math.rand2(-4, 4) +=> base;
	}
}

spork ~loop_me(s1, 60);
spork ~loop_me(s2, 36);
//me.yield();
minute => now;
//loop_me(s1, 60);

