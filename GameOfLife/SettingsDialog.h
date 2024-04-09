#pragma once
#include "wx/wx.h"
#include "Settings.h"
#include "wx/spinctrl.h"
#include "wx/clrpicker.h"

class SettingsDialog : public wxDialog
{
private:
	Settings* _settings;
	wxSpinCtrl* _gridSizeSpinner = nullptr;
	wxSpinCtrl* _intervalSpinner = nullptr;
	wxColourPickerCtrl* _livingColorPicker = nullptr;
	wxColourPickerCtrl* _deadColorPicker = nullptr;
public:
	SettingsDialog(wxWindow* parent, Settings* settings);
	~SettingsDialog();
	
	void OnGridSpin(wxSpinEvent& event);
	void OnIntervalSpin(wxSpinEvent& event);
	void OnLivingColorPick(wxColourPickerEvent& event);
	void OnDeadColorPick(wxColourPickerEvent& event);

	wxDECLARE_EVENT_TABLE();
};