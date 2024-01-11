#include "RenderObject.h"

#include <iostream>

void RenderObject::render()
{
	update();
	draw();
}
