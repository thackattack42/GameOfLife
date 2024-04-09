#include "SettingsDialog.h"

#pragma region Event Table
wxBEGIN_EVENT_TABLE(SettingsDialog, wxDialog)
EVT_SPINCTRL(10001, SettingsDialog::OnGridSpin)
EVT_SPINCTRL(10002, SettingsDialog::OnIntervalSpin)
EVT_COLOURPICKER_CHANGED(10003, SettingsDialog::OnLivingColorPick)
EVT_COLOURPICKER_CHANGED(10004, SettingsDialog::OnDeadColorPick)
wxEND_EVENT_TABLE()
#pragma endregion Event Table

#pragma region Constructor/Destructor
SettingsDialog::SettingsDialog(wxWindow* parent, Settings* settings)
	: wxDialog(parent, wxID_ANY, "Settings", wxPoint(100, 100), wxSize(300, 300))
{
	_settings = settings;

	wxBoxSizer* _sizer = new wxBoxSizer(wxVERTICAL);
	SetSizer(_sizer);
	wxBoxSizer* gridSpinnerSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* intervalSpinnerSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* livingColorSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* deadColorSizer = new wxBoxSizer(wxHORIZONTAL);
	wxSizer* okCancelButtons = CreateButtonSizer(wxOK | wxCANCEL);

	_gridSizeSpinner = new wxSpinCtrl(this, 10001);
	_intervalSpinner = new wxSpinCtrl(this, 10002);
	_livingColorPicker = new wxColourPickerCtrl(this, 10003);
	_deadColorPicker = new wxColourPickerCtrl(this, 10004);

	wxStaticText* gridSpinnerLabel = new wxStaticText(this, wxID_ANY, "Grid Size");
	gridSpinnerLabel->SetMinSize(wxSize(100, 50));
	wxStaticText* intervalSpinnerLabel = new wxStaticText(this, wxID_ANY, "Interval");
	intervalSpinnerLabel->SetMinSize(wxSize(100, 50));
	wxStaticText* livingColorLabel = new wxStaticText(this, wxID_ANY, "Living Cell Color");
	livingColorLabel->SetMinSize(wxSize(100, 50));
	wxStaticText* deadColorLabel = new wxStaticText(this, wxID_ANY, "Dead Cell Color");
	deadColorLabel->SetMinSize(wxSize(100, 50));

	gridSpinnerSizer->Add(gridSpinnerLabel);
	gridSpinnerSizer->Add(_gridSizeSpinner);
	intervalSpinnerSizer->Add(intervalSpinnerLabel);
	intervalSpinnerSizer->Add(_intervalSpinner);

	livingColorSizer->Add(livingColorLabel);
	livingColorSizer->Add(_livingColorPicker);
	deadColorSizer->Add(deadColorLabel);
	deadColorSizer->Add(_deadColorPicker);

	_sizer->Add(gridSpinnerSizer);
	_sizer->Add(intervalSpinnerSizer);
	_sizer->Add(livingColorSizer);
	_sizer->Add(deadColorSizer);
	_sizer->Add(okCancelButtons);

	_gridSizeSpinner->SetValue(_settings->_gridSize);
	_intervalSpinner->SetValue(_settings->_interval);
	_livingColorPicker->SetColour(_settings->GetLivingCellColor());
	_deadColorPicker->SetColour(_settings->GetDeadCellColor());
}

SettingsDialog::~SettingsDialog()
{
	delete _gridSizeSpinner;
	delete _intervalSpinner;
	delete _livingColorPicker;
	delete _deadColorPicker;
}
#pragma endregion Constructor/Destructor

#pragma region Event Methods
void SettingsDialog::OnGridSpin(wxSpinEvent& event)
{
	_settings->_gridSize = event.GetValue();
}

void SettingsDialog::OnIntervalSpin(wxSpinEvent& event)
{
	_settings->_interval = event.GetValue();
}

void SettingsDialog::OnLivingColorPick(wxColourPickerEvent& event)
{
	_settings->SetLivingCellColor(event.GetColour());
}

void SettingsDialog::OnDeadColorPick(wxColourPickerEvent& event)
{
	_settings->SetDeadCellColor(event.GetColour());
}
#pragma endregion Event Methods