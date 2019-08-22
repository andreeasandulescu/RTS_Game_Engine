#ifndef _CURSOR_H_
#define _CURSOR_H_

enum ButtonStatus{PRESSED, RELEASED, UNCHANGED_PRESSED, UNCHANGED_RELEASED};

class Cursor {
public:
	ButtonStatus right;
	ButtonStatus left;

	// screen coordinates:
	int xPos;
	int yPos;

	Cursor();
};

#endif