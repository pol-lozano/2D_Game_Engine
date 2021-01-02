# 2D_Game_Engine
2D Game Engine C++ SDL2
By Pol Lozano Llorens

## Controls
WASD - Move
Left MB - Place block
Right MB - Remove block
Middle MB - Copy block
1-4 - Select block type to use

# Features
## Hybrid ECS (similar to unity's) with components:
- BoxCollider2D
- CharacterController2D
- InputHandler (raw or smoothed input calculated with lerp function)
- Rigidbody2D (with basic physics simulation)
- Sprite (with color, alpha and blend mode modulation)
- Text 
- TileMap (with bitmasking and basic lighting)
- Transform

## AssetManger 
- Handles loading in textures and fonts, keeping track of them as well as cleaning up after the program ends.
## Physics and math classes 
- Generic Vector2 class with vector math functions used for a basic physics engine.
- Timer class with accurate delta time calculation with framecapping and V-Sync. 
- Collision class with AABB collision detection
