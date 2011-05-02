#include "displaySettings.h"


int DisplaySettings::isHidePlayerAngle()
{
	return gtk_toggle_button_get_active((GtkToggleButton*)this->checkPlayerAngle);
}

int DisplaySettings::isHidePlayerIndex()
{
	return gtk_toggle_button_get_active((GtkToggleButton*)this->checkPlayerIndex);
}

int DisplaySettings::isHidePlayerBody()
{
	return gtk_toggle_button_get_active((GtkToggleButton*)this->checkPlayerBody);
}

int DisplaySettings::isHideBall()
{
	return gtk_toggle_button_get_active((GtkToggleButton*)this->checkBall);
}

int DisplaySettings::isHidePlayerFuture()
{
	return gtk_toggle_button_get_active((GtkToggleButton*)this->checkPlayerFuture);
}

int DisplaySettings::isHidePlayerPath()
{
	return gtk_toggle_button_get_active((GtkToggleButton*)this->checkPlayerPath);
}
