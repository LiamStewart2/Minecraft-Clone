## Minecraft recreation : C++ & OpenGL
A project of mine, where i will attempt to recreate the popular voxel game "Minecraft" in C++ and OpenGL.<br>
The project currently uses the "Faithful" texture pack, however i will be moving to a custom texture pack eventually.
### Project setup
To clone the project, use visual studio's "Clone a repository" option. Copy and paste "https://github.com/LiamStewart2/Minecraft-Clone" into the repository location, choose the repository path and press clone. Once the repo is cloned, open "main.cpp", build and run.

If this didnt succeed (to be finished)...

### Controls
WASD           -move, ghost movement currently<br>
Esc            -close the project<br>
#### --- TO BE IMPLEMENTED
Left Click:    -break blocks<br>
Right Click:   -place blocks<br>
P              -restart world with new seed

### Functionality
Trying to stay accurate to the game, i adopted the same chunk system as minecraft, where every 16x16 grid of blocks is self contained. Each chunk is responsible for its own terrain generation, currently using maths functions however will use some form of noise to create realistic smooth terrain. Each chunk has a buffer dedicated for its blocks content where the blocks are put on the initialization of the chunk.<br>
Blocks has 6 faces. Each face has a correlating state. The state determines weather the face should be drawn or not to reduce the number of draw calls that OpenGL has to make. The block also has a reference to a "BlockInfo" class, which stores the texture and colour of the block amongst other data needed for optimisation. One of which being the a variable which measures weather all of the faces textures are the same. If this is the case then the draw function on the block doesnt have to change the texture unneccessarily and the same is done for colours.<br>
A blocks faces state is determined by weather the adjacent blocks are air or solid. This leaves edge cases at the sides of chunks however, so functions are ran to check the edges of chunks to update those specific cases. These face states significantly boost performace and allow the entire landscape to act as one mesh instead of hundreds of seperate boxes.<br>
