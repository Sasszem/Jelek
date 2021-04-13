#include "netEditApp.h"
#include "netMain.h"

wxIMPLEMENT_APP(netEditApp);

netEditApp::netEditApp()
{
}

netEditApp::~netEditApp()
{
}

bool netEditApp::OnInit()
{
	netMain* m = new netMain();
	m->Show(true);
	return true;
}
