#pragma once
#include "wx/wx.h"
#include "DrawingPanel.h"
#include "Settings.h"
#include <vector>

class MainWindow : public wxFrame
{
private:
	DrawingPanel* _drawingPanel = nullptr;
	wxBoxSizer* _boxSizer = nullptr;
	wxStatusBar* _statusBar = nullptr;
	wxToolBar* _toolBar = nullptr;
	wxTimer* _timer = nullptr;
	Settings _settings;
	wxMenuBar* _menuBar = nullptr;

	wxMenu* _settingsMenu = nullptr;

	wxMenu* _fileMenu = nullptr;

	wxMenu* _viewMenu = nullptr;
	wxMenuItem* _finite = nullptr;
	wxMenuItem* _toroidal = nullptr;
	wxMenuItem* _grid = nullptr;
	wxMenuItem* _10x10Grid = nullptr;
	wxMenuItem* _HUD = nullptr;
	wxMenuItem* _neighborCount = nullptr;

	std::vector<std::vector<bool>> _gameBoard;
	std::vector<std::vector<int>> _neighborCounts;

	int _generations;
	int _livingCells;
	std::string _filePath;
public:
	MainWindow();
	~MainWindow();

	void InitializeGrid();
	void UpdateStatusBar();
	int CalculateNeighborCount(int row, int col);
	void UpdateNeighborCount();
	int CalculateLivingCells();
	void CreateNextGeneration();
	void RandomizeGrid(int seed);
	void Refresh(bool eraseBackground = true, const wxRect* rect = nullptr);
	void RefreshMenuItems();

	void OnSizeChange(wxSizeEvent& evt);
	void OnPlayClick(wxCommandEvent& evt);
	void OnPauseClick(wxCommandEvent& evt);
	void OnNextClick(wxCommandEvent& evt);
	void OnTrashClick(wxCommandEvent& evt);
	void OnTimerFire(wxTimerEvent& evt);
	void OnOptionsClick(wxCommandEvent& evt);
	void OnRandomClick(wxCommandEvent& evt);
	void OnRandomSeedClick(wxCommandEvent& evt);
	void OnResetSettingsClick(wxCommandEvent& evt);
	void OnNewClick(wxCommandEvent& evt);
	void OnOpenClick(wxCommandEvent& evt);
	void OnSaveClick(wxCommandEvent& evt);
	void OnSaveAsClick(wxCommandEvent& evt);
	void OnImportClick(wxCommandEvent& evt);
	void OnExitClick(wxCommandEvent& evt);
	void OnFiniteClick(wxCommandEvent& evt);
	void OnToroidalClick(wxCommandEvent& evt);
	void OnGridClick(wxCommandEvent& evt);
	void On10x10GridClick(wxCommandEvent& evt);
	void OnHUDClick(wxCommandEvent& evt);
	void OnNeighborCountClick(wxCommandEvent& evt);

	wxDECLARE_EVENT_TABLE();
};