# ToyboxProject
The Toybox is a work-in-progress UE5 multiplayer game framework designed to build up further experience with UE5's broader game systems.

It includes a gameplay gym for testing ideas and prototyping different games and mechanics, allowing for the sharing code and functionality between my prototypes rather than individual projects.

## Feature List
| Name  | Description |
| ------------- | ------------- |
| Game Context System  | Dynamically adds and removes sets of gameplay abilities (and UI is planned) which allows for varied gameplay abilities for various pieces of content. |
| Interactable Component |  Implemented as a component which can be attached onto any AActor to add functionality on interaction or on entering the interaction volume to implement trigger volumes. |
| Minigames | Tied into the interaction system, this is a structured way to implement unique gameplay in a organised state machine implementation.  |
| GAS Integration |  Setup of GAS was implemented by following the setup of the unoffical GAS documentation from Tranek (https://github.com/tranek/GASDocumentation) and UE's offical documentation. |

## Planned Features
Blacksmithing Minigame - Utilises existing art assets I have previously made for a game jam and it is a good way to test my minigame and game context ability logic.

General Game Loop - Consisting of a main menu, loading screens during transition, in game and an options menu.

Scene Manager - Create a hub/overworld area to be able to transition between levels.

Dialogue System - Implement a barebones dialogue system which can be used to do a simple dialogue with an actor.
