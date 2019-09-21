# RTS Game Engine

  Game engines are important software tools in game industry as they relieve the game developers of reimplementing common components 
like the Renderer, Camera, Resource Loader etc. In this thesis we aim to implement a game engine for developing an RTS (Real Time Strategy)
game. We focus on implementing common features found in most RTS games like selecting a unit and giving commands to it, rendering terrain, 
water, vegetation, sky, loading resources, communication bus etc. for an open-source RTS game engine. For the features we implemented 
we present both the theoretical approach and the implementation. In developing the engine, we only used portable open-source libraries 
with the purpose to allow others to continue our work without licensing restrictions.

This engine is written in C++. For rendering it uses OpenGL 4.6 and GLSL for shading.
Other libraries employed:
  - GLM - math library
  - GLFW - for creating the OpenGL context
  - STD - for algorithms, data structures, threading
  - Assimp - model loading and animations
  - GLAD - OpenGL
  - FreeType - for rendering text for the User Interface
  - STB - loading textures/images
