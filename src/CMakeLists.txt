SET(SOURCE
    main.cpp
    GameManager.cpp
    RenderingInstance.cpp
    MenuButton.cpp
    Menu.cpp
    LevelOverview.cpp
    Game.cpp
    Level.cpp
    Player.cpp
    LazyCameraManipulator.cpp
    GameKeyboardHandler.cpp
    PlayerUpdater.cpp
    PlayerState.cpp
	HeadUpDisplay.cpp
	Cuboid.cpp
    KinematicCharacterController.cpp
    ParticleEffectFactory.cpp
    Skybox.cpp
)

#dirty!
SET(HEADER
	../include/Cuboid.h
	../include/Game.h
	../include/GameKeyboardHandler.h
	../include/GameManager.h
	../include/HeadUpDisplay.h
	../include/KinematicCharacterController.h
	../include/LazyCameraManipulator.h
	../include/Level.h
	../include/LevelOverview.h
	../include/Menu.h
	../include/MenuButton.h
	../include/Player.h
	../include/PlayerState.h
	../include/PlayerUpdater.h
	../include/RenderingInstance.h
	../include/types.h
	../include/Skybox.h
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
