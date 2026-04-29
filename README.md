# ToyboxProject
The ToyboxProject is a work-in-progress UE5 multiplayer game framework designed to gain experience with UE5's broader game systems.
It also includes gameplay gyms for testing ideas and prototyping different games and mechanics while sharing code and functionality between my prototypes.

Current Features:
Game Context System - Used to dynamically add sets of gameplay abilities and in the future UI too, this means we can have varied gameplay abilities added in sets which is particularly convenient.
Interaction System - Implemented via a component that can be attached onto any AActor to add some sort of functionality on interaction or on entering the volume for trigger volumes.
Minigames - A simple state machine style implementation containing various states to transition between while playing the minigame.
GAS Integration - Setup of GAS following the setup of the unoffical GAS documentation from Tranek (https://github.com/tranek/GASDocumentation)