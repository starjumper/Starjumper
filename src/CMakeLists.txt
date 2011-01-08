SET(SOURCE
    main.cpp
    GameManager.cpp
    RenderingInstance.cpp
    Menu.cpp
    Game.cpp
)

FIND_PACKAGE( OpenGL )
IF( APPLE )
    SET( CMAKE_CXX_LINK_FLAGS "-framework OpenGL -framework GLUT" )
ENDIF( APPLE )

LINK_LIBRARIES(
	BulletDynamics
	BulletCollision
	LinearMath
	${OPENGL_gl_LIBRARY}
	${OPENGL_glu_LIBRARY}
)

ADD_EXECUTABLE( StarJumper ${SOURCE} ${HEADER} )

TARGET_LINK_LIBRARIES(
	StarJumper
	${OSG_LIBRARIES}
	${OSGWORKS_LIBRARIES}
	${BULLET_LIBRARIES}
	${OSGBULLET_LIBRARIES}
)