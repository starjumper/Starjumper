# Starjumper

## Requirements

### OpenSceneGraph
get Version [2.8.3](http://www.openscenegraph.org/projects/osg/wiki/Downloads)

**for Mac OS X (32 Bit)**
	$ cmake -DOSG_WINDOWING_SYSTEM=Cocoa -DOSG_DEFAULT_IMAGE_PLUGIN_FOR_OSX=imageio
	$ make
	$ sudo make install

**for Mac OS X (64 Bit)**
	$ cmake -DOSG_WINDOWING_SYSTEM=Cocoa -DOSG_DEFAULT_IMAGE_PLUGIN_FOR_OSX=imageio -DCMAKE_OSX_ARCHITECTURES=x86_64
	$ make
	$ sudo make install

### Bullet Physics Library
get Version [2.75]
install as described in the instructions	

### osgWorks
get [latest Version](http://code.google.com/p/osgworks/)
install as described in the instructions	
	
### osgBullet
get [latest Version](http://code.google.com/p/osgbullet/)
install as described in the instructions	

### OggVorbis
get [latest Version](http://www.vorbis.com/)
install as described in the instructions
you will need libogg library as well as libvorbis library

### OpenAL 
get [latest Version](http://connect.creativelabs.com/openal/)
install as described in the instructions	

### freeALUT
get [latest Version](http://connect.creativelabs.com/openal/)
install as described in the instructions	

### osgAudio
get [latest Version](http://code.google.com/p/osgaudio/)
install as described in the instructions

## Install and run Starjumper
	$ cmake .
	$ make
	$ ./bin/Starjumper

## Coding Conventions

- Tabs: 4 Whitespaces

- Classes and methods:
	- Camel-case
	- Classes start with capital letter (files are named _exactly_ like the class)
	- Methods don't, neither do variables
	- private member variables start with _ (underscore)
	- No egyptian-style brackets
	- #pragma once instead of include guards in header files (#ifndef ... #define ... #endif)
	
	Example:
	--------

	*** FooBar.h ***

	#pragma once

	class FooBar
	{
	private:
		int _member1;
		int _member2;
	protected:
		int fooBarMethod();
	public:
		FooBar(int member1, int member2);
		char suckMyDick();
	}
	
	
	*** FooBar.cpp ***
	
	FooBar::FooBar(int member1, int member2) :
		_member1(member1),
		_member2(member2)
	{
		// More code here
	}
	
- Comments
	- License and commented-out code: /* ... */
	- All other comments:             // ...
	- No capital letters at the beginning

- for-loops
	- unless required otherwise, use size_t as type for run variable
	- preincrement, not postincrement the run variable
	- e.g.: for(size_t i = 0; i < 100; ++i)
	
- Don't use namespaces!
- use const if possible!