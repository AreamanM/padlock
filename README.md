# Padlock

After trying many hacky autohotkey scripts from 7 year old posts in obscure forums,
I decided to make my own reliable and working implementation of global media keys for Spotify.

Simply dropping the executable in your startup folder(press win+r and type $shell:startup) is enough; Padlock does not have a gui and takes around 2-3 mb of ram at any given moment so it is very lightweight.

> **Note**
> Padlock is a Windows application.

- ctrl-alt-{left,right} for prev/next song
- ctrl-alt-space for toggling play/pause
- ctrl-alt-{up,down} for increasing/decreasing volume by 5 points

The code is far from clean, and there are oppurtunities to generalise padlock to work
for any app, not just Spotify, but it does work.

This is a visual studio project(I might try moving it to cmake in the future), so building is straightforward.
