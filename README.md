#Gwion-plug#
plugins for [Gwion](github.com/fennecdjay/Gwion)  
actually a Gwion submodule
##Configure##
If you're not building from within Gwion, please edit config.mk
so that INC points to a valid gwion include directory.

##make##
just *cd* into the diectory containing the plug you want to build, and type 
```
make
```

##install##
while still in a plug directory, typing
```
make install
```

will try to install the plugin (a .so file) in /usr/lib/Gwion/plug, which is the default place for a gwion plug.  
this behavior can be changed by editing include/defs.h in Gwion and rebuilding it.
