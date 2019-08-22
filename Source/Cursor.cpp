#include <Cursor.h>

Cursor::Cursor() {
	this->left = ButtonStatus::UNCHANGED_RELEASED;
	this->right = ButtonStatus::UNCHANGED_RELEASED;

	this->xPos = 0;
	this->yPos = 0;
}