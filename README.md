# Dungeon Maze Generator for Unreal Engine

This Generator create a dungeon maze using the following rules : 
- Generate a maze of a given size.
- Generate a given number of chests at dead end locations (3 walls).
- Choose a random outer wall as the entrance and the exit.

To exit the dungeon, the player needs keys that are given by the chests.

![image](https://github.com/noahmazard/TestJumboMana/assets/93341159/5be1ff14-8a94-473f-8e0f-30676347e852)
Exemple of a generated dungeon.

## How to test it :

Open the `FirstPerson\Maps\FirstPersonMap` level.

In the world hierarchy, you can see a `BP_Labyrinth`, that's our dungeon.

In the details panel, you'll see the followings options :

![image](https://github.com/noahmazard/TestJumboMana/assets/93341159/1a8746c6-5954-4049-bf11-e8173d29f07d)

1. The Mesh Category group all the parameters used for the visuals of the dungeon
  - **Wall Mesh Class** : The mesh class for the walls
  - **Move Half Tile** : Should the walls be move half of the tile size ? Set this if the mesh's pivot is not centered
  - **Floor Mesh Class** : The mesh class for the floor
  - **Tile Size** : The world size of a single tile
  - **Exit Actor Class** : The Actor Class for the exit
  - **Entrance Actor Class** : The Actor Class for the entrance
  - **Chest Actor Class** : The Actor Class for the chest
2. The Generation Catergory group all the parameters used in the generation process
  - **Seed** : The seed used to generate the maze
  - **Lab Size** : The size in tiles of the maze
  - **Nb Chests** : The number of chest to generate
  - **Random Generation at Start** : Should the maze be regenerated with a random seed at each game start
3. Two buttons used to rebuild the dungeon
  - **Clear Labyrinth** : clear all the labyrinth
  - **Generate Labyrith** : generate all the labyrinth and the visual
