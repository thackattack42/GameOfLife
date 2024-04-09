#pragma once
#include "wx/wx.h"
#include <fstream>

struct Settings
{
	int _gridSize = 15;
	int _interval = 50;
	bool _isFinite = true;
	bool _showGrid = true;
	bool _show10x10Grid = true;
	bool _showHUD = true;
	bool _showNeighborCount = true;

	unsigned int _livingRed = 128;
	unsigned int _livingBlue = 128;
	unsigned int _livingGreen = 128;
	unsigned int _livingAlpha = 255;

	unsigned int _deadRed = 255;
	unsigned int _deadBlue = 255;
	unsigned int _deadGreen = 255;
	unsigned int _deadAlpha = 255;

	wxColor GetLivingCellColor()
	{
		wxColor color(_livingRed, _livingGreen, _livingBlue, _livingAlpha);
		return color;
	}

	wxColor GetDeadCellColor()
	{
		wxColor color(_deadRed, _deadGreen, _deadBlue, _deadAlpha);
		return color;
	}

	void SetLivingCellColor(wxColor color)
	{
		_livingRed = color.GetRed();
		_livingBlue = color.GetBlue();
		_livingGreen = color.GetGreen();
		_livingAlpha = color.GetAlpha();
	}

	void SetDeadCellColor(wxColor color)
	{
		_deadRed = color.GetRed();
		_deadBlue = color.GetBlue();
		_deadGreen = color.GetGreen();
		_deadAlpha = color.GetAlpha();
	}

	void LoadData()
	{
		std::ifstream file("settings.bin", std::ios::binary | std::ios::in);
		file.read((char*)this, sizeof(Settings));
		file.close();
	}

	void SaveData()
	{
		std::ofstream file("settings.bin", std::ios::out | std::ios::binary);
		file.write((char*)this, sizeof(Settings));
		file.close();
	}
};