SET(SOURCE
    main.cpp
	LevelMenu.cpp
	MenuKeyboardHandler.cpp
	Level.cpp
	LevelKeyboardHandler.cpp
	Player.cpp
	PlayerState.cpp
	PlayerUpdater.cpp
	KinematicCharacterController.cpp
	LazyCameraManipulator.cpp
	CollisionObject.cpp
	CollisionModel.cpp
	Cuboid.cpp
	ParticleEffectFactory.cpp
	HeadUpDisplay.cpp
	Sky.cpp
	Sound.cpp
)

#dirty!
SET(HEADER
	../include/types.h
	../include/LevelMenu.h
	../include/MenuKeyboardHandler.h
	../include/Level.h
	../include/LevelKeyboardHandler.h
	../include/Player.h
	../include/PlayerState.h
	../include/PlayerUpdater.h
	../include/KinematicCharacterController.h
	../include/LazyCameraManipulator.h
	../include/CollisionObject.h
	../include/CollisionModel.h
	../include/Cuboid.h
	../include/ParticleEffectFactory.h
	../include/HeadUpDisplay.h
	../include/Sky.h
	../include/Sound.h
)

FIND_PACKAGE( OpenGL )
IF( APPLE )
    SET( CMAKE_CXX_LINK_FLAGS "-framework OpenGL -framework GLUT" )
ENDIF( APPLE )

LINK_LIBRARIES(
	BulletDynamics
	BulletCollision
	LinearMath
	osgShadow
	osgAudio
	openalpp
	${OPENGL_gl_LIBRARY}
	${OPENGL_glu_LIBRARY}
)

ADD_EXECUTABLE( StarJumper ${SOURCE} ${HEADER} )

TARGET_LINK_LIBRARIES(
	StarJumper
	osgWidget
	${OSG_LIBRARIES}
	${OSGWORKS_LIBRARIES}
	${BULLET_LIBRARIES}
	${OSGBULLET_LIBRARIES}
)
