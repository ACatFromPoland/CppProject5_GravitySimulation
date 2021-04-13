# CppProject5_GravitySimulation

This is my 5th Project, the other 3 between 1 and 5 aren't functional for presentation.

Brief description - 
   Just trying out physics simulation and decided to go with the Gravity and Motion section from my Physics book.
   Has a bit of controls for camera movement and changing the enviroment.

Preview

![ezgif com-gif-maker (2)](https://user-images.githubusercontent.com/67654528/114599787-7a3ae780-9c8b-11eb-93ac-6c832c7a2867.gif)

Youtube link- -- https://youtu.be/nWG7fOTy0w0

This uses the SFML library. -- I think its the 64bit Visual Studio 15 one.

Setup - (This is for myself in the future)

Go into Project Properties- Select all Configurations In C++ -> Additional Include Dicts add "c:\SFMLVS\INCLUDE" In Linker -> Additional Library Dicts "c:\SFMLVS\lib" In Linker, Linker Input -> Additional Dependencies "sfml-window.lib;sfml-system.lib;sfml-graphics.lib;sfml-audio.lib;sfml-network.lib;"

Apply <- Go into Debug Configuration In Linker, Linker Input -> "sfml-window-d.lib;sfml-system-d.lib;sfml-graphics-d.lib;sfml-audio-d.lib;sfml-network-d.lib;"

After that get SFMLVS Bin .dlls and add into solution with CPP files. --> Run
