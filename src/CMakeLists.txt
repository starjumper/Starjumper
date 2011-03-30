SET(SOURCE
    main.cpp
	Level.cpp
	Player.cpp
	KinematicCharacterController.cpp
	LazyCameraManipulator.cpp
	CollisionObject.cpp
	Cuboid.cpp
)

#dirty!
SET(HEADER
	../include/Level.h
	../include/Player.h
	../include/KinematicCharacterController.h
	../include/LazyCameraManipulator.h
	../include/CollisionObject.h
	../include/Cuboid.h
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
