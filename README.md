This repository contains the code and all assets required for the game project titled Fantastic strategies 
All assets attached to this project were created by generative AI
AI was used as an assistant to creating some code in this project 

Within this repository is an EXE file which will run the game when opened 

The project uses 3 external libraries, all of which are contained in the library
SDL 3: https://github.com/libsdl-org/SDL/releases/tag/release-3.2.26
SDL image: https://github.com/libsdl-org/SDL_image
SDL ttf:https://github.com/libsdl-org/SDL_ttf/releases

Set up in visual studio: 
1. Open project properties and select VC++ Directories
2. Select VC++ Directories and open include Directories
3. Click the 3 dots at the end of the first include path
4. Navigate to the repository, select SDL3-3.2.22 and then select include then click select folder (File path should be (Repository path)\SDL3-3.2.22\include)
5.Click the 3 dots on the second include path, navigate to the repository, click SDL_image-3.2.4 then include SDL3 then click select folder(File path should be (Repository path)\SDL3_image-3.2.4\include\SDL3)
6.Click ok 
7.Open Library directories 
8.On the first path click the 3 dots and navigate to the repository,then SDL3-2.22 then
lib then x64. Once navigated here click select folder (Path should be : Repository path\SDL3.3.2.22\lib\x64)
9.On the second path click the 3 dots, navigate to the repository, click SDL3_image-3.2.4 then click lib then x64 then click select folder (path should be: Repository path\SDL3_image-3.2.4\lib\x64)
10.Click ok then apply
11.Click C/C++ ,click general then open additional include directories. On the include path click the 3 dots and navigate to the repository, then SDL3_ttf-3.2.2\include then select folder (Path should be : Repository: SDL3_ttf-3.2.2\include) then click ok
12.Click C/C++ all options then open additional include directories. On the include path click the 3 dots and navigate to the repository, then SDL3_ttf-3.2.2\include then select folder (Path should be : Repository: SDL3_ttf-3.2.2\include) then click ok
13.Open linker then click general then open additional library directories. Click the 3 dots on the path and open repository path, then SDL3_ttf-3.2.2 then lib then x64 then click ok (final path should be: Repository path\SDL3_ttf-3.2.2\lib\x64
14.Open linker, then input and open additional dependencies and add:
 SDL3.lib 
SDL3_ttf.lib
SDL3_image.lib
15.Click ok then click system, set subsystem to Console (/SUBSYSTEM:CONSOLE) then click apply

Once setup in visual studio, the EXE file and the game will run
