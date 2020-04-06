# Tarkov ESP

I have included a small example of how you could use looking-glass to render ESP on a virtual-machine host.

## How to use
- Build the project in /client/
- Read the guide for setting up your virtual machine to support [Looking Glass](https://github.com/gnif/LookingGlass) (my project only includes the modified client, you still need the drivers and software on the guest.)
- Run `./looking-glass-client -s -k -g EGLTarkov` as root once Tarkov is open, you can run without `-g EGLTarkov` to run the normal renderer.

You will probably need to use something like Synergy for m/k input because spice is rubbish. `-s` disables it.

![Picture](https://i.imgur.com/bKtYsOd.png)

## Issues & Improvements
The code is far from perfect and I have only included a really simple example, there are a couple of issues that I have identified that really should be worked on;

### Issues/Flaws/Improvements
* FPS Camera sometimes can't be found in tagged-objects and has to be cached, you must open looking glass before joining a match.
* You will get bad framerate until you enter a game because I am bad and am iterating tagged/active objects every frame until GameWorld is found. You can run without `-g EGLTarkov` until you are ready to join a game.
* The overlay and game are not in perfect sync, although not terrible it could definitely be improved.
* And probably a bunch of other things.

## Third Party

This project is a modification of [Looking Glass](https://github.com/gnif/LookingGlass) and uses [VMRead](https://github.com/Heep042/vmread) to read virtual machine memory.
