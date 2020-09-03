#define ENTRYPOINT
#include <Morpheus.h>

//using namespace Morpheus;

class SandboxApp : public Morpheus::Application
{
public:
	SandboxApp()
		: Morpheus::Application()
	{
		//LAYER
	}

	~SandboxApp()
	{
	}

};

Morpheus::Application* CreateApplication()
{
	return new SandboxApp();
}