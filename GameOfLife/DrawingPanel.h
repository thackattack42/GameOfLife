#pragma once
#include "wx/wx.h"
#include "Settings.h"
#include <vector>

class DrawingPanel : public wxPanel
{
private:
	wxFrame* _parent = nullptr;
	wxPoint _point;
	Settings* _settings;
	int* _generations;
	int* _livingCells;
	std::vector<std::vector<bool>>& _gameBoard;
	std::vector<std::vector<int>>& _neighborCounts;
	void OnPaint(wxPaintEvent& evt);
	void OnMouseUp(wxMouseEvent& evt);
public:
	DrawingPanel(wxFrame* parent, std::vector<std::vector<bool>>& gameBoard, std::vector<std::vector<int>>& neighborCounts, Settings* settings, int* generations, int* livingCells);
	~DrawingPanel();
	void SetSize(wxSize& size);
	void SetGridSize(int size);

	wxDECLARE_EVENT_TABLE();
};

