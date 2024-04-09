#pragma once

#include "wx/wx.h"
#include "MainWindow.h"

class App : public wxApp
{
private:
	MainWindow* _mainWindow = nullptr;
public:
	App();
	~App();
	virtual bool OnInit();
};

