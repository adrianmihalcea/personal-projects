# BowAndArrow
 Bow and Arrow OpenGL Game

# Intro
 This is a variation to the classic Bow and Arrow game, written for a homework
 for my Introduction to Computer Graphics course (November 2020).

 I have included the source code (a Visual Studio project) and a demo video
 (demo.mp4).

# Gameplay
 The game features a bow that shoots an arrow at two types of targets: balloons
 and shurikens.

 The bow shoots towards the current mouse position proportional to the amount
 of time you press the left mouse button (with a power bar indicator beneath
 the bow).

 There are two types of balloons that spawn at random locations and go towards
 the top of the window. The red balloons give +1 score when you shoot them,
 while the yellow ones give -1 score. The shurikens give +1 score if you shoot
 them, but subtract a life if they collide with the bow.

 Collisions are done by intersecting the tip of the arrow and the circular
 hitboxes of the bow, balloons and shurikens.

 Under the player, the current number of lives is displayed as red hearts.
 The game features two score indicators at the top left of the screen, one for
 the current score and one for the high score. The file scores.txt records the
 log of all scores obtained (used for establishing the highscore).

 The difficulty increases every 10 points, increasing the number of bad balloons
 and shurikens.

# Libraries & implementation
 The game is written in C++ using OpenGL and a series of openGL libraries
 (GLFW, GLEW, GLM), along with a framework containing a simple scene and camera
 provided by my university for this course (EGC-framework).

 The Game is implemented in the Game folder, with Game.cpp being the main
 source file. Constants.h has all the constants required for the program, all
 the meshes are defined in Meshes.h, the classes for the target objects are
 defined in Targets.h, while the score and lives are updated by an object of
 class Score defined in Score.h.

 The rendering is done in the Update function in Game.cpp, while the game logic
 is done in GameEngine.cpp.

 The code features extensive comments, highlighting the use of all classes and
 functions.
 