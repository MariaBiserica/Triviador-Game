#include "Triviador.h"

Triviador::Triviador(QWidget* parent)
	: QMainWindow(parent)
{

}

Triviador::Triviador(const std::string& ip, const std::string& playerUsername)
{
	ui.setupUi(this);

	this->setWindowTitle("Triviador");

	m_ip = ip;
	m_playerUsername = playerUsername;

	ui.changeUsernameLineEdit->hide();
	ui.changePasswordLineEdit->hide();
	ui.changeEmailLineEdit->hide();

	ui.userErrorLabel->hide();

	ui.updateUserDetailsMessageLabel->hide();

	ui.stackedWidget->setCurrentIndex(0);
}

Triviador::~Triviador()
{
	
}

void Triviador::on_playGamePushButton_released()
{
	triviadorGame = new Game(m_ip, m_playerUsername);

	ui.stackedWidget->insertWidget(3, triviadorGame);
	
	ui.stackedWidget->setCurrentIndex(3);
}

void Triviador::on_profilePushButton_released()
{
	updateUserDetails();
}

void Triviador::on_backToMenuPushButton_released()
{
	ui.stackedWidget->setCurrentIndex(0);
}

void Triviador::on_profileSettingsPushButton_released()
{
	ui.stackedWidget->setCurrentIndex(2);
}

void Triviador::on_backToProfileButton_released()
{
	ui.stackedWidget->setCurrentIndex(1);

	updateUserDetails();
}

void Triviador::on_quitPushButton_released()
{
	std::string link = m_ip + "/logoutuser/?username=" + m_playerUsername;

	cpr::Response responseFromServer = cpr::Get(cpr::Url(link));

	close();
}

void Triviador::on_changeUsernamePushButton_released()
{
	ui.changeUsernameLineEdit->show();

	ui.updateUserDetailsMessageLabel->hide();

	std::string newUsername = ui.changeUsernameLineEdit->text().toLocal8Bit().constData();
}

void Triviador::on_changePasswordPushButton_released()
{
	ui.changePasswordLineEdit->show();

	ui.updateUserDetailsMessageLabel->hide();

	std::string newUsername = ui.changePasswordLineEdit->text().toLocal8Bit().constData();
}

void Triviador::on_changeEmailPushButton_released()
{
	ui.changeEmailLineEdit->show();

	ui.updateUserDetailsMessageLabel->hide();

	std::string newUsername = ui.changeEmailLineEdit->text().toLocal8Bit().constData();
}

void Triviador::on_saveProfileSettingsPushButton_released()
{
	std::string currentUsername = m_playerUsername;
	std::string newUsername = ui.changeUsernameLineEdit->text().toStdString();
	std::string newPassword = ui.changePasswordLineEdit->text().toStdString();
	std::string newEmail = ui.changeEmailLineEdit->text().toStdString();
	
	std::string link = m_ip + "/updateuser/?current_username=" + currentUsername + "&new_username=" + newUsername + "&new_password=" + newPassword + "&new_email=" + newEmail;

	cpr::Response responseFromServer = cpr::Get(cpr::Url(link));

	if (responseFromServer.status_code >= 200 && responseFromServer.status_code < 300)
	{
		if (newUsername.empty() == false)
		{
			m_playerUsername = newUsername;
		}

		ui.changeUsernameLineEdit->setText("");
		ui.changePasswordLineEdit->setText("");
		ui.changeEmailLineEdit->setText("");

		ui.changeUsernameLineEdit->hide();
		ui.changePasswordLineEdit->hide();
		ui.changeEmailLineEdit->hide();
		
		ui.updateUserDetailsMessageLabel->setText(responseFromServer.text.c_str());
		ui.updateUserDetailsMessageLabel->show();
	}
	else
	{
		std::string text = "Error: " + std::to_string(responseFromServer.status_code) + "\n" + responseFromServer.text;
		
		ui.updateUserDetailsMessageLabel->setText(text.c_str()); 
		ui.updateUserDetailsMessageLabel->show();
	}
}

void Triviador::updateUserDetails()
{
	ui.userErrorLabel->hide();

	ui.stackedWidget->setCurrentIndex(1);

	std::string link = m_ip + "/getuserdata/?username=" + m_playerUsername;

	cpr::Response responseFromServer = cpr::Get(cpr::Url(link));

	if (responseFromServer.status_code >= 200 && responseFromServer.status_code < 300)
	{
		auto db_user = crow::json::load(responseFromServer.text);

		ui.userProfileUsernameLabel->setText(QString::fromStdString(db_user["Username"].s()));

		ui.userProfileEmailLabel->setText(QString::fromStdString(db_user["Email"].s()));

		ui.userProfilePlayedGamesLabel->setText(QString::fromStdString(db_user["PlayedGames"].s()));

		ui.userProfileWonGamesLabel->setText(QString::fromStdString(db_user["WonGames"].s()));

		ui.userProfileCreationDateLabel->setText(QString::fromStdString(db_user["AccountCreationDate"].s()));
	}
	else
	{
		ui.userErrorLabel->setText("Error: " + QString::fromStdString(std::to_string(responseFromServer.status_code)) + "\n" + QString::fromStdString(responseFromServer.text));
		ui.userErrorLabel->show();
	}
}
