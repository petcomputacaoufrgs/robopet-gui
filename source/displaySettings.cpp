#include "displaySettings.h"


int DisplaySettings::isCheckPlayerAngle()
{
	return gtk_toggle_button_get_active((GtkToggleButton*)this->checkPlayerAngle);
}

int DisplaySettings::isCheckPlayerIndex()
{
	return gtk_toggle_button_get_active((GtkToggleButton*)this->checkPlayerIndex);
}

int DisplaySettings::isCheckPlayerBody()
{
	return gtk_toggle_button_get_active((GtkToggleButton*)this->checkPlayerBody);
}

int DisplaySettings::isCheckBallShow()
{
	return gtk_toggle_button_get_active((GtkToggleButton*)this->checkBallShow);
}

int DisplaySettings::isCheckPlayerFuture()
{
	return gtk_toggle_button_get_active((GtkToggleButton*)this->checkPlayerFuture);
}
