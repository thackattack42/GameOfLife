#include "MainWindow.h"
#include "play.xpm"
#include "pause.xpm"
#include "next.xpm"
#include "trash.xpm"
#include "SettingsDialog.h"
#include "wx/numdlg.h"
#include "wx/filedlg.h"
#include <fstream>

#pragma region Event Table
wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
	EVT_SIZE(MainWindow::OnSizeChange)
	EVT_MENU(10001, MainWindow::OnPlayClick)
	EVT_MENU(10002, MainWindow::OnPauseClick)
	EVT_MENU(10003, MainWindow::OnNextClick)
	EVT_MENU(10004, MainWindow::OnTrashClick)
	EVT_TIMER(10005, MainWindow::OnTimerFire)
	EVT_MENU(10006, MainWindow::OnOptionsClick)
	EVT_MENU(10007, MainWindow::OnRandomClick)
	EVT_MENU(10008, MainWindow::OnRandomSeedClick)
	EVT_MENU(wxID_NEW, MainWindow::OnNewClick)
	EVT_MENU(wxID_OPEN, MainWindow::OnOpenClick)
	EVT_MENU(wxID_SAVE, MainWindow::OnSaveClick)
	EVT_MENU(wxID_SAVEAS, MainWindow::OnSaveAsClick)
	EVT_MENU(10017, MainWindow::OnImportClick)
	EVT_MENU(wxID_EXIT, MainWindow::OnExitClick)
	EVT_MENU(10014, MainWindow::OnFiniteClick)
	EVT_MENU(10015, MainWindow::OnToroidalClick)
	EVT_MENU(10016, MainWindow::OnResetSettingsClick)
	EVT_MENU(10018, MainWindow::OnGridClick)
	EVT_MENU(10019, MainWindow::On10x10GridClick)
	EVT_MENU(10020, MainWindow::OnHUDClick)
	EVT_MENU(10021, MainWindow::OnNeighborCountClick)
wxEND_EVENT_TABLE()
#pragma endregion Event Table

#pragma region Constructor/Destructor
MainWindow::MainWindow()
	: wxFrame(nullptr, wxID_ANY, "Game of Life", wxPoint(100, 100), wxSize(500, 500))
{
	_drawingPanel = new DrawingPanel(this, _gameBoard, _neighborCounts, &_settings, &_generations, &_livingCells);
	_boxSizer = new wxBoxSizer(wxVERTICAL);
	_statusBar = CreateStatusBar();
	_toolBar = CreateToolBar();
	_timer = new wxTimer(this, 10005);
	_menuBar = new wxMenuBar();

	_settingsMenu = new wxMenu();

	_fileMenu = new wxMenu();

	_viewMenu = new wxMenu();
	_finite = new wxMenuItem(_viewMenu, 10014, "Finite", "", wxITEM_CHECK);
	_finite->SetCheckable(true);
	_toroidal = new wxMenuItem(_viewMenu, 10015, "Toroidal", "", wxITEM_CHECK);
	_toroidal->SetCheckable(true);
	_grid = new wxMenuItem(_viewMenu, 10018, "Grid", "", wxITEM_CHECK);
	_grid->SetCheckable(true);
	_10x10Grid = new wxMenuItem(_viewMenu, 10019, "10x10 Grid", "", wxITEM_CHECK);
	_10x10Grid->SetCheckable(true);
	_HUD = new wxMenuItem(_viewMenu, 10020, "HUD", "", wxITEM_CHECK);
	_HUD->SetCheckable(true);
	_neighborCount = new wxMenuItem(_viewMenu, 10021, "Neighbor Count", "", wxITEM_CHECK);
	_neighborCount->SetCheckable(true);

	_boxSizer->Add(_drawingPanel, 1, wxEXPAND | wxALL);
	this->SetSizer(_boxSizer);

	this->Bind(wxEVT_SIZE, &MainWindow::OnSizeChange, this);

	_settings.LoadData();
	InitializeGrid();
	UpdateStatusBar();

	wxBitmap playIcon(play_xpm);
	wxBitmap pauseIcon(pause_xpm);
	wxBitmap nextIcon(next_xpm);
	wxBitmap trashIcon(trash_xpm);

	_toolBar->AddTool(10001, "Play", playIcon);
	_toolBar->AddTool(10002, "Pause", pauseIcon);
	_toolBar->AddTool(10003, "Next", nextIcon);
	_toolBar->AddTool(10004, "Trash", trashIcon);

	_toolBar->Realize();

	this->Layout();

	SetMenuBar(_menuBar);
	_menuBar->Append(_fileMenu, "File");
	_menuBar->Append(_viewMenu, "View");
	_menuBar->Append(_settingsMenu, "Settings");

	_settingsMenu->Append(10006, "Options");
	_settingsMenu->Append(10007, "Randomize");
	_settingsMenu->Append(10008, "Randomize With Seed");
	_settingsMenu->Append(10016, "Reset Settings");

	_fileMenu->Append(wxID_NEW);
	_fileMenu->Append(wxID_OPEN);
	_fileMenu->Append(wxID_SAVE);
	_fileMenu->Append(wxID_SAVEAS);
	_fileMenu->Append(10017, "Import");
	_fileMenu->Append(wxID_EXIT);

	_viewMenu->Append(_finite);
	_viewMenu->Append(_toroidal);
	_viewMenu->Append(_grid);
	_viewMenu->Append(_10x10Grid);
	_viewMenu->Append(_HUD);
	_viewMenu->Append(_neighborCount);

	RefreshMenuItems();
}

MainWindow::~MainWindow()
{
	delete _drawingPanel;
	delete _timer;
}
#pragma endregion Constructor/Destructor

#pragma region Methods
void MainWindow::InitializeGrid()
{
	_gameBoard.resize(_settings._gridSize);
	_neighborCounts.resize(_settings._gridSize);

	for (int i = 0; i < _settings._gridSize; i++)
	{
		_gameBoard[i].resize(_settings._gridSize);
		_neighborCounts[i].resize(_settings._gridSize);
	}

	_drawingPanel->SetGridSize(_settings._gridSize);
}

void MainWindow::UpdateStatusBar()
{
	_livingCells = CalculateLivingCells();
	_statusBar->SetStatusText("Generations: " + std::to_string(_generations) + " Interval:  " + std::to_string(_settings._interval) + " Alive: " + std::to_string(_livingCells) + " Seed:  ");
}

int MainWindow::CalculateNeighborCount(int row, int col)
{
	InitializeGrid();
	int neighbors = 0;

	if (!_settings._isFinite)
	{
		for (int i = -1; i < 2; i++)
		{
			for (int j = -1; j < 2; j++)
			{
				int temp1 = row + i;
				int temp2 = col + j;

				if (i == 0 && j == 0)
					continue;

				if (temp1 < 0)
					temp1 = _settings._gridSize - 1;

				if (temp2 < 0)
					temp2 = _settings._gridSize - 1;

				if (temp1 > _settings._gridSize - 1)
					temp1 = 0;

				if (temp2 > _settings._gridSize - 1)
					temp2 = 0;

				if (_gameBoard[temp1][temp2] == true)
					neighbors++;
			}
		}
	}
	else
	{
		for (int i = -1; i < 2; i++)
		{
			for (int j = -1; j < 2; j++)
			{
				if (i == 0 && j == 0)
					continue;

				if (row + i < 0 || col + j < 0 || row + i > _settings._gridSize - 1 || col + j > _settings._gridSize - 1)
					continue;

				if (_gameBoard[row + i][col + j] == true)
					neighbors++;
			}
		}
	}
	return neighbors;
}

void MainWindow::UpdateNeighborCount()
{
	for (int i = 0; i < _gameBoard.size(); i++)
	{
		for (int j = 0; j < _gameBoard.size(); j++)
		{
			int count = CalculateNeighborCount(i, j);
			_neighborCounts[i][j] = count;
		}
	}
}

int MainWindow::CalculateLivingCells()
{
	_livingCells = 0;
	for (int i = 0; i < _gameBoard.size(); i++)
	{
		for (int j = 0; j < _gameBoard.size(); j++)
		{
			if (_gameBoard[i][j] == true)
				_livingCells++;
		}
	}
	return _livingCells;
}

void MainWindow::CreateNextGeneration()
{
	std::vector<std::vector<bool>> sandbox;
	int sandboxAlive = 0;

	sandbox.resize(_settings._gridSize);

	for (int i = 0; i < _settings._gridSize; i++)
		sandbox[i].resize(_settings._gridSize);

	for (int i = 0; i < _gameBoard.size(); i++)
	{
		for (int j = 0; j < _gameBoard.size(); j++)
		{
			int count = CalculateNeighborCount(i, j);
			if (count < 2 && _gameBoard[i][j] == true)
			{
				sandbox[i][j] = false;
			}
			else if (count > 3 && _gameBoard[i][j] == true)
			{
				sandbox[i][j] = false;
			}
			else if (count >= 2 && count <= 3 && _gameBoard[i][j] == true)
			{
				sandbox[i][j] = true;
				sandboxAlive++;
			}
			else if (count == 3 && _gameBoard[i][j] == false)
			{
				sandbox[i][j] = true;
				sandboxAlive++;
			}
		}
	}

	_gameBoard.swap(sandbox);
	UpdateNeighborCount();
	_livingCells = sandboxAlive;
	_generations++;
	Refresh();
}

void MainWindow::RandomizeGrid(int seed)
{
	srand(seed);
	int randomPercent = 1 + (rand() % 100);

	for (int i = 0; i < _gameBoard.size(); i++)
	{
		for (int j = 0; j < _gameBoard.size(); j++)
		{
			int randomNum = rand();

			if (randomNum % 100 < randomPercent)
				_gameBoard[i][j] = true;
		}
	}
}

void MainWindow::Refresh(bool eraseBackground, const wxRect* rect)
{
	wxFrame::Refresh();
	UpdateStatusBar();
	UpdateNeighborCount();
}

void MainWindow::RefreshMenuItems()
{
	_finite->Check(_settings._isFinite);
	_toroidal->Check(!_settings._isFinite);
	_grid->Check(_settings._showGrid);
	_10x10Grid->Check(_settings._show10x10Grid);
	_HUD->Check(_settings._showHUD);
}
#pragma endregion Methods

#pragma region Event Methods
void MainWindow::OnSizeChange(wxSizeEvent& event)
{
	wxSize windowSize = event.GetSize();
	_drawingPanel->SetSize(windowSize);
	event.Skip();
}

void MainWindow::OnPlayClick(wxCommandEvent& evt)
{
	_timer->Start(_settings._interval);
}

void MainWindow::OnPauseClick(wxCommandEvent& evt)
{
	_timer->Stop();
}

void MainWindow::OnNextClick(wxCommandEvent& evt)
{
	CreateNextGeneration();
}

void MainWindow::OnTrashClick(wxCommandEvent& evt)
{
	InitializeGrid();
	for (int i = 0; i < _settings._gridSize; i++)
		for (int j = 0; j < _settings._gridSize; j++)
			_gameBoard[i][j] = false;

	UpdateStatusBar();
	UpdateNeighborCount();
	_drawingPanel->Refresh();
}

void MainWindow::OnTimerFire(wxTimerEvent& evt)
{
	CreateNextGeneration();
}

void MainWindow::OnOptionsClick(wxCommandEvent& evt)
{
	SettingsDialog* dialog = new SettingsDialog(this, &_settings);
	dialog->CenterOnParent();

	if (dialog->ShowModal() == wxID_OK)
	{
		OnTrashClick(evt);
		_settings.SaveData();
		InitializeGrid();
		Refresh();
	}
	else
		_settings.LoadData();

	delete dialog;
}

void MainWindow::OnRandomClick(wxCommandEvent& evt)
{
	OnTrashClick(evt);

	RandomizeGrid(time(NULL));
	Refresh();
}

void MainWindow::OnRandomSeedClick(wxCommandEvent& evt)
{
	int seed = wxGetNumberFromUser("Randomize", "Seed", "Seed Dialog", 0, 0, LONG_MAX);
	if (seed != -1)
	{
		RandomizeGrid(seed);
		Refresh();
	}
}

void MainWindow::OnResetSettingsClick(wxCommandEvent& evt)
{
	_settings._gridSize = 15;
	_settings._interval = 50;
	_settings._isFinite = true;
	_filePath = "";
	_settings.SetLivingCellColor(*wxLIGHT_GREY);
	_settings.SetDeadCellColor(*wxWHITE);
	_settings._isFinite = true;
	_settings._showGrid = true;
	_settings.SaveData();
	InitializeGrid();
	UpdateStatusBar();
	RefreshMenuItems();
	_drawingPanel->Refresh();
}

void MainWindow::OnNewClick(wxCommandEvent& evt)
{
	_generations = 0;
	OnTrashClick(evt);
	UpdateStatusBar();
	_drawingPanel->Refresh();
}

void MainWindow::OnOpenClick(wxCommandEvent& evt)
{
	wxFileDialog fileDialog(this, "Open", wxEmptyString, wxEmptyString, "Cells Files (*.cells) | *.cells", wxFD_OPEN);

	if (fileDialog.ShowModal() == wxID_CANCEL)
		return;

	for (int i = 0; i < _gameBoard.size(); i++)
	{
		_gameBoard[i].clear();
		_gameBoard.resize(0);
	}
	_gameBoard.clear();
	_gameBoard.resize(0);

	std::string buffer;
	std::ifstream fileStream;
	int index = 0;
	_filePath = (std::string)fileDialog.GetPath();
	fileStream.open(_filePath);

	if (fileStream.is_open())
	{
		while (!fileStream.eof())
		{
			std::getline(fileStream, buffer);

			if (buffer.size() == 0)
				break;
			if (_gameBoard.size() == 0)
				_gameBoard.resize(buffer.size());
			_gameBoard[index].resize(buffer.size());

			for (int i = 0; i < buffer.size(); i++)
			{
				if (buffer[i] == '*')
					_gameBoard[index][i] = true;
				else
					_gameBoard[index][i] = false;
			}
			index++;
		}
		fileStream.close();
	}
	Refresh();
}

void MainWindow::OnSaveClick(wxCommandEvent& evt)
{
	if (_filePath == "")
	{
		wxFileDialog fileDialog(this, "Save", wxEmptyString, wxEmptyString, "Cells Files (*.cells) | *.cells", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

		if (fileDialog.ShowModal() == wxID_CANCEL)
			return;

		_filePath = (std::string)fileDialog.GetPath();
	}

	std::ofstream fileStream;
	fileStream.open(_filePath);

	if (fileStream.is_open())
	{
		for (int i = 0; i < _gameBoard.size(); i++)
		{
			for (int j = 0; j < _gameBoard.size(); j++)
			{
				if (_gameBoard[i][j])
					fileStream << '*';
				else
					fileStream << '.';
			}
			fileStream << "\n";
		}
		fileStream.close();
	}
	evt.Skip();
}

void MainWindow::OnSaveAsClick(wxCommandEvent& evt)
{
	wxFileDialog fileDialog(this, "Save As", wxEmptyString, wxEmptyString, "Cells Files (*.cells) | *.cells", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

	if (fileDialog.ShowModal() == wxID_CANCEL)
		return;

	std::ofstream fileStream;
	_filePath = (std::string)fileDialog.GetPath();
	fileStream.open(_filePath);

	if (fileStream.is_open())
	{
		for (int i = 0; i < _gameBoard.size(); i++)
		{
			for (int j = 0; j < _gameBoard.size(); j++)
			{
				if (_gameBoard[i][j])
					fileStream << '*';
				else
					fileStream << '.';
			}
			fileStream << "\n";
		}
		fileStream.close();
	}
	evt.Skip();
}

void MainWindow::OnImportClick(wxCommandEvent& evt)
{
	wxFileDialog fileDialog(this, "Import", wxEmptyString, wxEmptyString, "Cells Files (*.cells) | *.cells", wxFD_OPEN);

	if (fileDialog.ShowModal() == wxID_CANCEL)
		return;

	std::string buffer;
	std::ifstream fileStream;
	int index = 0;
	_filePath = (std::string)fileDialog.GetPath();
	fileStream.open(_filePath);

	if (fileStream.is_open())
	{
		while (!fileStream.eof() && index < _gameBoard.size())
		{
			std::getline(fileStream, buffer);

			for (int i = 0; i < buffer.size() && i < _gameBoard.size(); i++)
			{
				if (buffer[i] == '*')
					_gameBoard[index][i] = true;
				else
					_gameBoard[index][i] = false;
			}
			index++;
		}
		fileStream.close();
	}
	Refresh();
}

void MainWindow::OnExitClick(wxCommandEvent& evt)
{
	Close();
}

void MainWindow::OnFiniteClick(wxCommandEvent& evt)
{
	_settings._isFinite = true;
	RefreshMenuItems();
	_settings.SaveData();
	Refresh();
	evt.Skip();
}

void MainWindow::OnToroidalClick(wxCommandEvent& evt)
{
	_settings._isFinite = false;
	RefreshMenuItems();
	_settings.SaveData();
	Refresh();
	evt.Skip();
}

void MainWindow::OnGridClick(wxCommandEvent& evt)
{
	if (_settings._showGrid)
		_settings._showGrid = false;
	else
		_settings._showGrid = true;

	RefreshMenuItems();
	_settings.SaveData();
	Refresh();
	evt.Skip();
}

void MainWindow::On10x10GridClick(wxCommandEvent& evt)
{
	if (_settings._show10x10Grid)
		_settings._show10x10Grid = false;
	else
		_settings._show10x10Grid = true;

	RefreshMenuItems();
	_settings.SaveData();
	Refresh();
	evt.Skip();
}

void MainWindow::OnHUDClick(wxCommandEvent& evt)
{
	if (_settings._showHUD)
		_settings._showHUD = false;
	else
		_settings._showHUD = true;

	RefreshMenuItems();
	_settings.SaveData();
	Refresh();
	evt.Skip();
}

void MainWindow::OnNeighborCountClick(wxCommandEvent& evt)
{
	if (_settings._showNeighborCount)
		_settings._showNeighborCount = false;
	else
		_settings._showNeighborCount = true;

	RefreshMenuItems();
	_settings.SaveData();
	Refresh();
	evt.Skip();
}
#pragma endregion Event Methods