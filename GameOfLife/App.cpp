#include "App.h"

wxIMPLEMENT_APP(App);

App::App() 
{

}

App::~App() 
{

}

bool App::OnInit() 
{
	_mainWindow = new MainWindow();
	_mainWindow->Show();

	return true;
}