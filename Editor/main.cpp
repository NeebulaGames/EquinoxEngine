#include "Engine.h"

int main(int argc, char** argv)
{
	ReportMemoryLeaks();

	LOG("Engine Creation ----------------");
	Engine* engine = new Engine;

	int main_return = engine->Loop();

	RELEASE(engine);
	LOG("Bye! :D");

	return main_return;
}