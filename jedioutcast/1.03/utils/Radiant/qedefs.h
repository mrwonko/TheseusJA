#ifndef __QEDEFS_H__
#define __QEDEFS_H__

#define QE_VERSION  0x0501

#define QE3_STYLE (WS_OVERLAPPED | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_CHILD)
#define QE3_STYLE2 (WS_OVERLAPPED | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_MINIMIZEBOX | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU)
#define QE3_CHILDSTYLE (WS_OVERLAPPED | WS_MINIMIZEBOX | WS_THICKFRAME | WS_CAPTION | WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_MAXIMIZEBOX)

#define QE3_SPLITTER_STYLE (WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS)



#define QE_AUTOSAVE_INTERVAL  5       // number of minutes between autosaves

#define	_3DFXCAMERA_WINDOW_CLASS	"Q3DFXCamera"
#define	CAMERA_WINDOW_CLASS	"QCamera"
#define	XY_WINDOW_CLASS	    "QXY"
#define	Z_WINDOW_CLASS   	"QZ"
#define	ENT_WINDOW_CLASS	"QENT"
#define	TEXTURE_WINDOW_CLASS	"QTEX"

#define	ZWIN_WIDTH	40
#define CWIN_SIZE	(0.4)

#define	MAX_EDGES	256
#define	MAX_POINTS	512

#define	CMD_TEXTUREWAD	60000
#define	CMD_BSPCOMMAND	61000

#define	PITCH	0
#define	YAW		1
#define	ROLL	2

#define QE_TIMER0   1

#define	PLANE_X		0
#define	PLANE_Y		1
#define	PLANE_Z		2
#define	PLANE_ANYX	3
#define	PLANE_ANYY	4
#define	PLANE_ANYZ	5

#define	ON_EPSILON	0.01

#define	KEY_FORWARD		1
#define	KEY_BACK		2
#define	KEY_TURNLEFT	4
#define	KEY_TURNRIGHT	8
#define	KEY_LEFT		16
#define	KEY_RIGHT		32
#define	KEY_LOOKUP		64
#define	KEY_LOOKDOWN	128
#define	KEY_UP			256
#define	KEY_DOWN		512

// xy.c
#define EXCLUDE_LIGHTS				0x01
#define EXCLUDE_ENT					0x02
#define EXCLUDE_PATHS				0x04
#define EXCLUDE_WATER				0x08
#define EXCLUDE_WORLD				0x10
#define EXCLUDE_CLIP				0x20
#define	EXCLUDE_DETAIL				0x40
#define	EXCLUDE_CURVES				0x80
#define	INCLUDE_EASY				0x100
#define	INCLUDE_NORMAL				0x200
#define	INCLUDE_HARD				0x400
#define	INCLUDE_DEATHMATCH			0x800
#define EXCLUDE_HINT				0x1000
#define EXCLUDE_WAYPOINTS			0x2000
#define EXCLUDE_MISCMODELS			0x4000
#define EXCLUDE_MISCMODELBREAKABLES 0x8000
#define EXCLUDE_TARGETSPEAKERS		0x10000
#define	EXCLUDE_REFTAGS				0x20000
#define EXCLUDE_MISCMODELXXXX		0x40000
#define SHOW_GROUPNAMES				0x80000		// a bit misleading, this is used elsewhere than the show/hide menu, but I'm not sure of the side effects of adding to the QE globals structure (invalidate user prefs etc?) and I only need one bit, so...
#define SHOW_WAYPOINTS_ONLY			0x100000	// this IS used in view/show, but it's not a single exclude, nor an include in the normal sense
#define EXCLUDE_TRIGGERXXXX			0x200000
#define EXCLUDE_NONSOLID			0x400000
#define EXCLUDE_EASY				0x800000
#define EXCLUDE_MEDIUM				0x1000000
#define EXCLUDE_HARD				0x2000000
#define EXCLUDE_ALL_EXCEPT_DETAIL	0x4000000
#define EXCLUDE_FUNC_GROUP			0x8000000
#define EXCLUDE_LIGHTS_RADII		0x10000000
#define SHOW_CURVES_ONLY			0x20000000

//
// menu indexes for modifying menus
//
#define	MENU_VIEW		2
#define	MENU_BSP		4
#define	MENU_TEXTURE	6
#define	MENU_PLUGIN	11


// odd things not in windows header...
#define	VK_COMMA		188
#define	VK_PERIOD		190

/*
** window bits
*/
#define	W_CAMERA		  0x0001
#define	W_XY			    0x0002
#define	W_XY_OVERLAY	0x0004
#define	W_Z				    0x0008
#define	W_TEXTURE		  0x0010
#define	W_Z_OVERLAY		0x0020
#define W_CONSOLE		  0x0040
#define W_ENTITY		  0x0080
#define W_CAMERA_IFON 0x0100
#define W_XZ          0x0200  //--| only used for patch vertex manip stuff
#define W_YZ          0x0400  //--|
#define	W_ALL			0xFFFFFFFF

#define	COLOR_TEXTUREBACK	0
#define	COLOR_GRIDBACK		1
#define	COLOR_GRIDMINOR		2
#define	COLOR_GRIDMAJOR		3
#define	COLOR_CAMERABACK	4
#define COLOR_ENTITY      5
#define COLOR_GRIDBLOCK   6
#define COLOR_GRIDTEXT    7
#define COLOR_BRUSHES     8
#define COLOR_SELBRUSHES  9
#define COLOR_CLIPPER     10
#define COLOR_VIEWNAME    11
#define COLOR_LAST        12

// classes
#define ENTITY_WIREFRAME		0x00001
#define ENTITY_SKIN_MODEL		0x00010
#define ENTITY_SELECTED_ONLY	0x00100
#define ENTITY_BOXED			0x01000

// menu settings
#define ENTITY_BOX				0x01000
#define ENTITY_WIRE				0x00001
#define ENTITY_SELECTED			0x00101
#define ENTITY_SKINNED 			0x00010
#define ENTITY_SKINNED_BOXED	0x01010
#define ENTITY_SELECTED_SKIN	0x00110



#endif
