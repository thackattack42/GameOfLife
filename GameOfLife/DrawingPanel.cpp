#include "DrawingPanel.h"
#include "wx/graphics.h"
#include "wx/dcbuffer.h"

#pragma region Event Table
wxBEGIN_EVENT_TABLE(DrawingPanel, wxPanel)
EVT_PAINT(DrawingPanel::OnPaint)
EVT_LEFT_UP(DrawingPanel::OnMouseUp)
wxEND_EVENT_TABLE()
#pragma endregion Event Table

#pragma region Constructor/Destructor
DrawingPanel::DrawingPanel(wxFrame* parent, std::vector<std::vector<bool>>& gameBoard, std::vector<std::vector<int>>& neighborCounts, Settings* settings, int* generations, int* livingCells)
	: wxPanel(parent, wxID_ANY, wxPoint(0, 0), wxSize(200, 200)), _gameBoard(gameBoard), _neighborCounts(neighborCounts)
{
	this->SetBackgroundStyle(wxBG_STYLE_PAINT);
	SetDoubleBuffered(true);

	_point.x = 100;
	_point.y = 100;

	_parent = parent;
	_gameBoard = gameBoard;
	_neighborCounts = neighborCounts;
	_settings = settings;
	_generations = generations;
	_livingCells = livingCells;

}

DrawingPanel::~DrawingPanel()
{

}
#pragma endregion Constructor/Destructor

#pragma region Event Methods
void DrawingPanel::OnPaint(wxPaintEvent& event)
{
	wxAutoBufferedPaintDC dc(this);
	dc.Clear();

	wxGraphicsContext* _context = wxGraphicsContext::Create(dc);
	_context->SetPen(*wxBLACK);
	_context->SetBrush(*wxWHITE);

	int panelWidth;
	int panelHeight;
	int solidLines = _settings->_gridSize / 10;
	GetSize(&panelWidth, &panelHeight);

	int cellWidth = panelWidth / _settings->_gridSize;
	int cellHeight = panelHeight / _settings->_gridSize;

	for (int i = 0; i < _settings->_gridSize; i++)
	{
		for (int j = 0; j < _settings->_gridSize; j++)
		{
			if (_gameBoard[i][j] == true)
			{
				_context->SetBrush(_settings->GetLivingCellColor());
				if (!_settings->_showGrid)
					_context->SetPen(_settings->GetLivingCellColor());
			}
			else
			{
				_context->SetBrush(_settings->GetDeadCellColor());
				if (!_settings->_showGrid)
					_context->SetPen(_settings->GetDeadCellColor());
			}


			_context->DrawRectangle(i * cellWidth, j * cellHeight, cellWidth, cellHeight);
			
			wxString numOfNeighbors = std::to_string(_neighborCounts[i][j]);
			double textWidth, textHeight;

			_context->SetFont(wxFontInfo(8), *wxBLACK);
			_context->GetTextExtent(numOfNeighbors, &textWidth, &textHeight);
			if (_neighborCounts[i][j] > 0 && !_settings->_showNeighborCount)
				_context->DrawText(numOfNeighbors, (i * cellWidth) + cellWidth / 2 - textWidth / 2, (j * cellHeight) + cellHeight / 2 - textHeight / 2);
		}
	}


	if (_settings->_show10x10Grid)
	{
		wxPen* pen = new wxPen(*wxBLACK, 3);
		dc.SetPen(*pen);

		for (int i = 0; i < solidLines; i++) 
		{
			wxPoint vStart(cellWidth * (i + 1) * 10, 0);
			wxPoint vEnd(cellWidth * (i + 1) * 10, this->GetSize().y);
			dc.DrawLine(vStart, vEnd);
		}

		for (int i = 0; i < solidLines; i++)
		{
			wxPoint vStart(0, cellHeight * (i + 1) * 10);
			wxPoint vEnd(this->GetSize().x, cellHeight * (i + 1) * 10);
			dc.DrawLine(vStart, vEnd);
		}
	}

	if (_settings->_showHUD)
	{
		double xText, yText;
		wxString boundaryType;
		wxString numOfGens = "Generations: " + std::to_string(*_generations);
		wxString numOfLiving = "Cell Count: " + std::to_string(*_livingCells);
		if (_settings->_isFinite)
			boundaryType = "Boundary Type: Finite";
		else
			boundaryType = "Boundary Type: Toroidal";
		wxString universeSize = "Universe Size: " + std::to_string(_settings->_gridSize) + "x" + std::to_string(_settings->_gridSize);

		wxColor* fontColor = new wxColor(255, 0, 0, 200);
		_context->SetFont(wxFontInfo(16), *fontColor);

		_context->GetTextExtent(numOfGens, &xText, &yText);
		_context->DrawText(numOfGens, 5, GetSize().y - yText - 62);

		_context->GetTextExtent(numOfLiving, &xText, &yText);
		_context->DrawText(numOfLiving, 5, GetSize().y - yText - 42);

		_context->GetTextExtent(boundaryType, &xText, &yText);
		_context->DrawText(boundaryType, 5, GetSize().y - yText - 22);

		_context->GetTextExtent(universeSize, &xText, &yText);
		_context->DrawText(universeSize, 5, GetSize().y - yText - 2);
	}
	delete _context;
}

void DrawingPanel::OnMouseUp(wxMouseEvent& event)
{
	int xPos = event.GetX();
	int yPos = event.GetY();

	int panelWidth;
	int panelHeight;
	GetSize(&panelWidth, &panelHeight);

	int cellWidth = panelWidth / _settings->_gridSize;
	int cellHeight = panelHeight / _settings->_gridSize;

	int row = xPos / cellWidth;
	int col = yPos / cellHeight;

	if (_gameBoard[row][col] == true)
		_gameBoard[row][col] = false;
	else
		_gameBoard[row][col] = true;
	
	_parent->Refresh();
}
#pragma endregion Event Methods

#pragma region Methods
void DrawingPanel::SetSize(wxSize& size)
{
	wxPanel* panel = new wxPanel();
	panel->SetSize(size);
	Refresh();
	
	delete panel;
}

void DrawingPanel::SetGridSize(int size)
{
	_settings->_gridSize = size;
}
#pragma endregion Methods