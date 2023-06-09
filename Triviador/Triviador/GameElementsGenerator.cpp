#include "GameElementsGenerator.h"

GameElementsGenerator::GameElementsGenerator(const std::string& ip, const std::string& playerUsername, int roomID)
{
	ui.setupUi(this);

	m_ip = ip;
	m_playerUsername = playerUsername;
	m_roomID = roomID;
	
	ui.titleLabel->setText(" ");

	ui.getRandomQuestionButton->hide();

	ui.errorLabel->hide();
	ui.displayProximityCorrectAnswerLabel->hide();
	ui.displayAnswerVerdictSingleChoiceQuestionLabel->hide();
	ui.submitAnswerButton->hide();
	ui.inputAnswerLineEdit->hide();

	ui.multipleChoiceAnswer1Button->hide();
	ui.multipleChoiceAnswer2Button->hide();
	ui.multipleChoiceAnswer3Button->hide();
	ui.multipleChoiceAnswer4Button->hide();
	ui.elapsedTimeLabel->hide();

	ui.fifty_fiftyAdvantageButton->hide();
	ui.suggestAnswerAdvantageButton->hide();
	ui.suggestAnswerAdvantageLabel->hide();
	ui.offerAnswersAdvantageButton->hide();
	HideOfferedAnswers(true);

	ui.chooseTerritoryLabel->hide();
	ui.checkAnswerSelection->hide();
	ui.displayAnswerVerdictMultipleChoiceQuestionLabel->hide();
	ui.multipleChoiceAnswer1Button->setChecked(false);

	gotQuestion = false;
	gotSingleQuestion = false;
	m_gotFirstQuestion = false;
	
	m_answerHasBeenSelected = false;
	m_canChooseTerritory = false;
	m_baseSelectionStageActive = false;
	m_regionSelectionStageActive = false;
	m_duelStageActive = false;

	m_singleChoiceQuestion = QuestionSingleChoice();
	m_multipleChoiceQuestion = QuestionMultipleChoice();

	m_inputPlayerAnswer = -1;

	GetSingleChoiceQuestion();
}

GameElementsGenerator::~GameElementsGenerator()
{
}

void GameElementsGenerator::OnTimerTick()
{
	ui.progressBar->setValue(ui.progressBar->value() + 1);

	if (ui.progressBar->value() >= 100)
	{
		ui.getRandomQuestionButton->setDisabled(false);

		ToggleAnswers(true);

		timer->disconnect();
		ui.offerAnswersAdvantageButton->setDisabled(true);
		ui.suggestAnswerAdvantageButton->setDisabled(true);

		// generate just a new question with a route to server
		// get the question from the server

		gotQuestion = false;

		gotSingleQuestion = false;
		gotMultipleQuestion = false;

		if (m_inputPlayerAnswer == -1)
			SubmitSingleChoiceAnswer();

		DisplayAnswerResult();
	}
}

void GameElementsGenerator::TimerMethodToRequestDataFromServer(int time)
{
	timerToCheckServer->setInterval(time);

	timerToCheckServer->setTimerType(Qt::PreciseTimer);

	timerToCheckServer->start();

	connect(timerToCheckServer, SIGNAL(timeout()), this, SLOT(TickMethodToRequestDataFromServer()));
}

void GameElementsGenerator::GetSingleChoiceQuestion()
{
	std::string link = m_ip + "/getASingleChoiceQuestion";

	cpr::Response responseFromServer = cpr::Get(cpr::Url(link));
	
	if (responseFromServer.status_code == 200)
	{
		auto questionDetails = crow::json::load(responseFromServer.text);
		
		m_singleChoiceQuestion.SetQuestionText(questionDetails["question"].s());
		m_singleChoiceQuestion.SetAnswer(questionDetails["correctAnswer"].i());

		gotSingleQuestion = true;
		gotQuestion = true;
		
		if (m_gotFirstQuestion == false)
		{
			m_gotFirstQuestion = true;
			m_baseSelectionStageActive = true;
		}
	}
}

void GameElementsGenerator::TickMethodToRequestDataFromServer()
{		
	if (gotQuestion == true)
	{
		if (gotSingleQuestion == true)
		{
			ui.inputAnswerLineEdit->clear();
			ui.displayProximityCorrectAnswerLabel->clear();

			ui.inputAnswerLineEdit->setDisabled(false);
			ui.submitAnswerButton->setDisabled(false);

			ui.submitAnswerButton->show();
			ui.inputAnswerLineEdit->show();

			ui.multipleChoiceAnswer1Button->hide();
			ui.multipleChoiceAnswer2Button->hide();
			ui.multipleChoiceAnswer3Button->hide();
			ui.multipleChoiceAnswer4Button->hide();

			ui.fifty_fiftyAdvantageButton->hide();
			ui.suggestAnswerAdvantageButton->show();
			ui.suggestAnswerAdvantageLabel->hide();
			ui.offerAnswersAdvantageButton->show();
			HideOfferedAnswers(true);

			ui.displayAnswerVerdictMultipleChoiceQuestionLabel->hide();
			ui.displayAnswerVerdictSingleChoiceQuestionLabel->hide();

			ui.checkAnswerSelection->hide();
			ui.chooseTerritoryLabel->hide();

			ui.offerAnswersAdvantageButton->setDisabled(false);
			ui.suggestAnswerAdvantageButton->setDisabled(false);

			ui.titleLabel->setText(QString::fromStdString(m_singleChoiceQuestion.GetQuestionText()));

			StartTimer();

			update();
		}
		else if (gotMultipleQuestion == true)
		{
			ui.submitAnswerButton->hide();
			ui.inputAnswerLineEdit->hide();
			ui.errorLabel->hide();
			ui.displayProximityCorrectAnswerLabel->hide();
			ui.displayAnswerVerdictMultipleChoiceQuestionLabel->hide();
			ui.displayAnswerVerdictSingleChoiceQuestionLabel->hide();
			ui.chooseTerritoryLabel->hide();

			ui.multipleChoiceAnswer1Button->show();
			ui.multipleChoiceAnswer2Button->show();
			ui.multipleChoiceAnswer3Button->show();
			ui.multipleChoiceAnswer4Button->show();

			ui.fifty_fiftyAdvantageButton->show();
			ui.suggestAnswerAdvantageButton->hide();
			ui.suggestAnswerAdvantageLabel->hide();
			ui.offerAnswersAdvantageButton->hide();
			HideOfferedAnswers(true);

			ui.checkAnswerSelection->setText("");
			ui.checkAnswerSelection->show();
			m_answerHasBeenSelected = false;
			
			ui.multipleChoiceAnswer1Button->setStyleSheet("background-color:light;");
			ui.multipleChoiceAnswer2Button->setStyleSheet("background-color:light;");
			ui.multipleChoiceAnswer3Button->setStyleSheet("background-color:light;");
			ui.multipleChoiceAnswer4Button->setStyleSheet("background-color:light;");

			ui.titleLabel->setText(QString::fromStdString(m_multipleChoiceQuestion.GetQuestionText()));
			
			ui.multipleChoiceAnswer1Button->setText(QString::fromStdString(m_multipleChoiceQuestion.GetAnswer1()));
			ui.multipleChoiceAnswer2Button->setText(QString::fromStdString(m_multipleChoiceQuestion.GetAnswer2()));
			ui.multipleChoiceAnswer3Button->setText(QString::fromStdString(m_multipleChoiceQuestion.GetAnswer3()));
			ui.multipleChoiceAnswer4Button->setText(QString::fromStdString(m_multipleChoiceQuestion.GetAnswer4()));

			StartTimer();

			update();
		}

		ToggleAnswers(false);
		DisableAdvantageOfferedAnswers(false);

		elapsedTime.start();

		gotSingleQuestion = false;
		gotQuestion = false;
	}
}

void GameElementsGenerator::ToggleAnswers(bool toggleAnswer)
{
	ui.multipleChoiceAnswer1Button->setDisabled(toggleAnswer);
	ui.multipleChoiceAnswer2Button->setDisabled(toggleAnswer);
	ui.multipleChoiceAnswer3Button->setDisabled(toggleAnswer);
	ui.multipleChoiceAnswer4Button->setDisabled(toggleAnswer);

	ui.inputAnswerLineEdit->setDisabled(toggleAnswer);
	ui.submitAnswerButton->setDisabled(toggleAnswer);
}

void GameElementsGenerator::DisableAdvantageOfferedAnswers(bool toggleAnswer)
{
	ui.offeredAnswer1Button->setDisabled(toggleAnswer);
	ui.offeredAnswer2Button->setDisabled(toggleAnswer);
	ui.offeredAnswer3Button->setDisabled(toggleAnswer);
	ui.offeredAnswer4Button->setDisabled(toggleAnswer);
}

bool GameElementsGenerator::GetIsBaseSelectionStageActive()
{
	return m_baseSelectionStageActive;
}

void GameElementsGenerator::SetIsBaseSelectionStageActive(bool isBaseSelectionStageActive)
{
	m_baseSelectionStageActive = isBaseSelectionStageActive;
}

bool GameElementsGenerator::GetIsRegionSelectionStageActive()
{
	return m_regionSelectionStageActive;
}

void GameElementsGenerator::SetIsRegionSelectionStageActive(bool isRegionSelectionStageActive)
{
	m_regionSelectionStageActive = isRegionSelectionStageActive;
}

bool GameElementsGenerator::GetIsDuelStageActive()
{
	return m_duelStageActive;
}

void GameElementsGenerator::SetIsDuelStageActive(bool isDuelStageActive)
{
	m_duelStageActive = isDuelStageActive;
}

bool GameElementsGenerator::GetCanChooseTerritory()
{
	return m_canChooseTerritory;
}

void GameElementsGenerator::SetCanChooseTerritory(bool canChooseTerritory)
{
	m_canChooseTerritory = canChooseTerritory;
}

void GameElementsGenerator::SetNumberOfPlayers(uint16_t numberOfPlayers)
{
	m_numberOfPlayers = numberOfPlayers;
}

void GameElementsGenerator::StartTimer()
{
	ui.progressBar->setValue(0);

	timer = new QTimer(this);

	timer->setInterval(100);
	timer->setTimerType(Qt::PreciseTimer);

	connect(timer, SIGNAL(timeout()), this, SLOT(OnTimerTick()));

	timer->start();
}

void GameElementsGenerator::CheckMultipleChoiceAnswer(const QString chosenAnswer, bool& isCorrectAnswer)
{
	std::string correctAnswer = chosenAnswer.toStdString();
	QString text = QString::fromStdString(m_multipleChoiceQuestion.GetCorrectAnswer());
	if (correctAnswer == m_multipleChoiceQuestion.GetCorrectAnswer())
	{
		if (!m_answerHasBeenSelected)
		{
			ui.displayAnswerVerdictMultipleChoiceQuestionLabel->setText("The answer is correct!");
			ui.chooseTerritoryLabel->setText("Now you can choose a territory!");
			ui.chooseTerritoryLabel->show();
		}

		else
			ui.displayAnswerVerdictMultipleChoiceQuestionLabel->setText("");
		
		isCorrectAnswer = 1;
	}
	else
	{
		if (!m_answerHasBeenSelected)
			ui.displayAnswerVerdictMultipleChoiceQuestionLabel->setText("The correct answer is: " + text);
		else
			ui.displayAnswerVerdictMultipleChoiceQuestionLabel->setText("");
		
		isCorrectAnswer = 0;
	}
	
	ui.displayAnswerVerdictMultipleChoiceQuestionLabel->show();
}

void GameElementsGenerator::HideOfferedAnswers(bool hide)
{
	if (hide == true)
	{
		ui.offeredAnswer1Button->hide();
		ui.offeredAnswer2Button->hide();
		ui.offeredAnswer3Button->hide();
		ui.offeredAnswer4Button->hide();
	}
	else
	{
		ui.offeredAnswer1Button->show();
		ui.offeredAnswer2Button->show();
		ui.offeredAnswer3Button->show();
		ui.offeredAnswer4Button->show();
	}
}

void GameElementsGenerator::on_getRandomQuestionButton_released()
{
	if (m_numberOfPlayers >= 2 && m_numberOfPlayers <= 4)
		if (ui.getRandomQuestionButton->isEnabled())
		{
			srand(time(0));

			uint8_t randomValue = rand() % 2;

			ui.elapsedTimeLabel->hide();

			if (randomValue == 1)
			{
				ui.inputAnswerLineEdit->clear();
				ui.displayProximityCorrectAnswerLabel->clear();

				ui.inputAnswerLineEdit->setDisabled(false);
				ui.submitAnswerButton->setDisabled(false);

				ui.submitAnswerButton->show();
				ui.inputAnswerLineEdit->show();

				ui.multipleChoiceAnswer1Button->hide();
				ui.multipleChoiceAnswer2Button->hide();
				ui.multipleChoiceAnswer3Button->hide();
				ui.multipleChoiceAnswer4Button->hide();

				ui.fifty_fiftyAdvantageButton->hide();
				ui.suggestAnswerAdvantageButton->show();
				ui.suggestAnswerAdvantageLabel->hide();
				ui.offerAnswersAdvantageButton->show();
				HideOfferedAnswers(true);

				ui.displayAnswerVerdictMultipleChoiceQuestionLabel->hide();
				ui.displayAnswerVerdictSingleChoiceQuestionLabel->hide();

				ui.checkAnswerSelection->hide();
				ui.chooseTerritoryLabel->hide();

				ui.offerAnswersAdvantageButton->setDisabled(false);
				ui.suggestAnswerAdvantageButton->setDisabled(false);

				std::string link = m_ip + "/getRandomSingleQuestion";

				cpr::Response responseFromServer = cpr::Get(cpr::Url(link));

				if (responseFromServer.status_code == 200)
				{
					auto questionDetails = crow::json::load(responseFromServer.text);

					std::string quetionText = questionDetails["question"].s();
					ui.titleLabel->setText(QString::fromStdString(quetionText));
					m_currentPlayerAnswer = std::to_string(questionDetails["correctAnswer"].i());
				}

				StartTimer();

				update();
			}
			else
			{
				ui.submitAnswerButton->hide();
				ui.inputAnswerLineEdit->hide();
				ui.errorLabel->hide();
				ui.displayProximityCorrectAnswerLabel->hide();
				ui.displayAnswerVerdictMultipleChoiceQuestionLabel->hide();
				ui.displayAnswerVerdictSingleChoiceQuestionLabel->hide();
				ui.chooseTerritoryLabel->hide();

				ui.multipleChoiceAnswer1Button->show();
				ui.multipleChoiceAnswer2Button->show();
				ui.multipleChoiceAnswer3Button->show();
				ui.multipleChoiceAnswer4Button->show();

				ui.fifty_fiftyAdvantageButton->show();
				ui.suggestAnswerAdvantageButton->hide();
				ui.suggestAnswerAdvantageLabel->hide();
				ui.offerAnswersAdvantageButton->hide();
				HideOfferedAnswers(true);

				ui.checkAnswerSelection->setText("");
				ui.checkAnswerSelection->show();
				m_answerHasBeenSelected = false;
				
				std::string link = m_ip + "/getRandomMultipleQuestion";

				cpr::Response responseFromServer = cpr::Get(cpr::Url(link));

				if (responseFromServer.status_code == 200)
				{
					auto questionDetails = crow::json::load(responseFromServer.text);

					std::string quetionText = questionDetails["question"].s();
					ui.titleLabel->setText(QString::fromStdString(quetionText));
					
					m_currentPlayerAnswer = questionDetails["correctAnswer"].s();

					std::stringstream ss;
					ui.multipleChoiceAnswer1Button->setStyleSheet("background-color:light;");
					ui.multipleChoiceAnswer2Button->setStyleSheet("background-color:light;");
					ui.multipleChoiceAnswer3Button->setStyleSheet("background-color:light;");
					ui.multipleChoiceAnswer4Button->setStyleSheet("background-color:light;");
					ui.multipleChoiceAnswer1Button->setText(QString::fromStdString(questionDetails["answer1"].s()));
					ui.multipleChoiceAnswer2Button->setText(QString::fromStdString(questionDetails["answer2"].s()));
					ui.multipleChoiceAnswer3Button->setText(QString::fromStdString(questionDetails["answer3"].s()));
					ui.multipleChoiceAnswer4Button->setText(QString::fromStdString(questionDetails["answer4"].s()));
					QString answers = QString::fromStdString(ss.str());
				}
				
				StartTimer();

				update();
			}

			ui.getRandomQuestionButton->setDisabled(true);
			ToggleAnswers(false);
			DisableAdvantageOfferedAnswers(false);

			elapsedTime.start();
		}
}

void GameElementsGenerator::on_submitAnswerButton_released()
{
	ui.errorLabel->hide();

	if (ui.submitAnswerButton->isEnabled())
	{
		if (ui.inputAnswerLineEdit->text().isEmpty())
		{
			ui.errorLabel->setText("Please enter an answer!");
			ui.errorLabel->show();
		}
		else
		{
			QString text = ui.inputAnswerLineEdit->text();

			bool isValidAnswer = true;

			for (int c = 0; c < text.length() && isValidAnswer == true; c++)
				if (!text[c].isDigit())
					isValidAnswer = false;

			if (isValidAnswer == false)
			{
				ui.errorLabel->setText("Please enter a valid answer! (only digits)");
				ui.errorLabel->show();

				ui.displayProximityCorrectAnswerLabel->hide();
			}
			else
			{
				m_inputPlayerAnswer = text.split(" ")[0].toInt();

				ui.suggestAnswerAdvantageButton->setDisabled(true);
				ui.offerAnswersAdvantageButton->setDisabled(true);

				SubmitSingleChoiceAnswer();
			}
		}
	}
}

void GameElementsGenerator::on_multipleChoiceAnswer1Button_released()
{
	bool isCorrectAnswer = false;

	if (ui.multipleChoiceAnswer1Button->isEnabled())
	{
		CheckMultipleChoiceAnswer(ui.multipleChoiceAnswer1Button->text(), isCorrectAnswer);

		if (m_answerHasBeenSelected == false)
		{
			if (isCorrectAnswer)
			{
				ui.multipleChoiceAnswer1Button->setStyleSheet("background-color:green;");
				m_canChooseTerritory = true;

				ui.elapsedTimeLabel->show();
				ui.elapsedTimeLabel->setText(QString::number(elapsedTime.elapsed() / 1000) + "," + QString::number(elapsedTime.elapsed() % 1000) + " s");
			
				std::string link = m_ip + "/setResponseTime/?roomID=" + std::to_string(m_roomID) + "&username=" + m_playerUsername + "&responseTime=" + std::to_string(elapsedTime.elapsed());
				
				cpr::Response responseFromServer = cpr::Get(cpr::Url(link));
				
				if (responseFromServer.status_code != 200)
				{
					ui.errorLabel->setText("Player's response time could not be updated!");
					ui.errorLabel->show();
				}
			}
			else
			{
				ui.multipleChoiceAnswer1Button->setStyleSheet("background-color:red;");
			}
			m_answerHasBeenSelected = true;
		}
		else
			ui.checkAnswerSelection->setText("You have already chosen an answer!");
	}
}

void GameElementsGenerator::on_multipleChoiceAnswer2Button_released()
{
	bool isCorrectAnswer = false;

	if (ui.multipleChoiceAnswer2Button->isEnabled())
	{
		CheckMultipleChoiceAnswer(ui.multipleChoiceAnswer2Button->text(), isCorrectAnswer);
		if (m_answerHasBeenSelected == false)
		{
			if (isCorrectAnswer)
			{
				ui.multipleChoiceAnswer2Button->setStyleSheet("background-color:green;");
				m_canChooseTerritory = true;

				ui.elapsedTimeLabel->show();
				ui.elapsedTimeLabel->setText(QString::number(elapsedTime.elapsed() / 1000) + "," + QString::number(elapsedTime.elapsed() % 1000) + " s");
			
				std::string link = m_ip + "/setResponseTime/?roomID=" + std::to_string(m_roomID) + "&username=" + m_playerUsername + "&responseTime=" + std::to_string(elapsedTime.elapsed());
				
				cpr::Response responseFromServer = cpr::Get(cpr::Url(link));

				if (responseFromServer.status_code != 200)
				{
					ui.errorLabel->setText("Player's response time could not be updated!");
					ui.errorLabel->show();
				}
			}
			else
			{
				ui.multipleChoiceAnswer2Button->setStyleSheet("background-color:red;");
			}
			m_answerHasBeenSelected = true;
		}
		else
			ui.checkAnswerSelection->setText("You have already chosen an answer!");
	}
}

void GameElementsGenerator::on_multipleChoiceAnswer3Button_released()
{
	bool isCorrectAnswer = false;

	if (ui.multipleChoiceAnswer3Button->isEnabled())
	{
		CheckMultipleChoiceAnswer(ui.multipleChoiceAnswer3Button->text(), isCorrectAnswer);

		if (m_answerHasBeenSelected == false)
		{
			if (isCorrectAnswer)
			{
				ui.multipleChoiceAnswer3Button->setStyleSheet("background-color:green;");
				m_canChooseTerritory = true;

				ui.elapsedTimeLabel->show();
				ui.elapsedTimeLabel->setText(QString::number(elapsedTime.elapsed() / 1000) + "," + QString::number(elapsedTime.elapsed() % 1000) + " s");
			
				std::string link = m_ip + "/setResponseTime/?roomID=" + std::to_string(m_roomID) + "&username=" + m_playerUsername + "&responseTime=" + std::to_string(elapsedTime.elapsed());
				
				cpr::Response responseFromServer = cpr::Get(cpr::Url(link));

				if (responseFromServer.status_code != 200)
				{
					ui.errorLabel->setText("Player's response time could not be updated!");
					ui.errorLabel->show();
				}
			}
			else
			{
				ui.multipleChoiceAnswer3Button->setStyleSheet("background-color:red;");
			}
			m_answerHasBeenSelected = true;
		}
		else
			ui.checkAnswerSelection->setText("You have already chosen an answer!");
	}
}

void GameElementsGenerator::on_multipleChoiceAnswer4Button_released()
{
	bool isCorrectAnswer = false;

	if (ui.multipleChoiceAnswer4Button->isEnabled())
	{
		CheckMultipleChoiceAnswer(ui.multipleChoiceAnswer4Button->text(), isCorrectAnswer);

		if (m_answerHasBeenSelected == false)
		{
			if (isCorrectAnswer)
			{
				ui.multipleChoiceAnswer4Button->setStyleSheet("background-color:green;");
				m_canChooseTerritory = true;

				ui.elapsedTimeLabel->show();
				ui.elapsedTimeLabel->setText(QString::number(elapsedTime.elapsed() / 1000) + "," + QString::number(elapsedTime.elapsed() % 1000) + " s");
			
				std::string link = m_ip + "/setResponseTime/?roomID=" + std::to_string(m_roomID) + "&username=" + m_playerUsername + "&responseTime=" + std::to_string(elapsedTime.elapsed());
			
				cpr::Response responseFromServer = cpr::Get(cpr::Url(link));

				if (responseFromServer.status_code != 200)
				{
					ui.errorLabel->setText("Player's response time could not be updated!");
					ui.errorLabel->show();
				}
			}
			else
			{
				ui.multipleChoiceAnswer4Button->setStyleSheet("background-color:red;");
			}
			m_answerHasBeenSelected = true;
		}
		else
			ui.checkAnswerSelection->setText("You have already chosen an answer!");
	}
}

bool GameElementsGenerator::CheckQStringToAnswer(QString text)
{
	QString answer = QString::fromStdString(m_currentPlayerAnswer);
	
	if (answer == text)
		return true;
	return false;
}

void GameElementsGenerator::on_fifty_fiftyAdvantageButton_released()
{
	if (ui.fifty_fiftyAdvantageButton->isEnabled())
	{
		srand(time(0));
		int disabledAnswersCounter = 0;
		bool isCorrectAnswer;
		bool oneIsDisabled = false, twoIsDisabled = false, threeIsDisabled = false, fourIsDisabled = false;

		while (disabledAnswersCounter != 2)
		{
			int randomAnswerNumber = rand() % 4 + 1;

			switch (randomAnswerNumber)
			{
			case 1:
				isCorrectAnswer = CheckQStringToAnswer(ui.multipleChoiceAnswer1Button->text());
				if (!isCorrectAnswer && !oneIsDisabled)
				{
					ui.multipleChoiceAnswer1Button->setDisabled(true);
					disabledAnswersCounter++;
					oneIsDisabled = true;
				}
				break;
			case 2:
				isCorrectAnswer = CheckQStringToAnswer(ui.multipleChoiceAnswer2Button->text());
				if (!isCorrectAnswer && !twoIsDisabled)
				{
					ui.multipleChoiceAnswer2Button->setDisabled(true);
					disabledAnswersCounter++;
					twoIsDisabled = true;
				}
				break;
			case 3:
				isCorrectAnswer = CheckQStringToAnswer(ui.multipleChoiceAnswer3Button->text());
				if (!isCorrectAnswer && !threeIsDisabled)
				{
					ui.multipleChoiceAnswer3Button->setDisabled(true);
					disabledAnswersCounter++;
					threeIsDisabled = true;
				}
				break;
			case 4:
				isCorrectAnswer = CheckQStringToAnswer(ui.multipleChoiceAnswer4Button->text());
				if (!isCorrectAnswer && !fourIsDisabled)
				{
					ui.multipleChoiceAnswer4Button->setDisabled(true);
					disabledAnswersCounter++;
					fourIsDisabled = true;
				}
				break;
			}
		}

	}
}

void GameElementsGenerator::on_suggestAnswerAdvantageButton_released()
{
	srand(time(0));
	uint16_t randomValue = m_singleChoiceQuestion.GetAnswer() + rand() % 10 - rand() % 10;

	ui.suggestAnswerAdvantageLabel->setText(QString::number(randomValue));
	ui.suggestAnswerAdvantageLabel->show();
	ui.offerAnswersAdvantageButton->setDisabled(true);
}

void GameElementsGenerator::on_offerAnswersAdvantageButton_released()
{
	srand(time(0));
	uint16_t randomValue;
	ui.suggestAnswerAdvantageButton->setDisabled(true);

	randomValue =  m_singleChoiceQuestion.GetAnswer() - rand() % 80;
	ui.offeredAnswer1Button->setText(QString::number(randomValue));

	randomValue =  m_singleChoiceQuestion.GetAnswer() - rand() % 20;
	ui.offeredAnswer2Button->setText(QString::number(randomValue));

	randomValue =  m_singleChoiceQuestion.GetAnswer() + rand() % 20;
	ui.offeredAnswer3Button->setText(QString::number(randomValue));

	randomValue =  m_singleChoiceQuestion.GetAnswer() + rand() % 80;
	ui.offeredAnswer4Button->setText(QString::number(randomValue));

	HideOfferedAnswers(false);
}

void GameElementsGenerator::on_offeredAnswer1Button_released()
{
	uint16_t inputAnswer = ui.offeredAnswer1Button->text().toInt();
	uint16_t currentAnswer =  m_singleChoiceQuestion.GetAnswer();

	SubmitSingleChoiceAnswer();
	DisableAdvantageOfferedAnswers(true);
}

void GameElementsGenerator::on_offeredAnswer2Button_released()
{
	uint16_t inputAnswer = ui.offeredAnswer2Button->text().toInt();
	uint16_t currentAnswer =  m_singleChoiceQuestion.GetAnswer();

	SubmitSingleChoiceAnswer();
	DisableAdvantageOfferedAnswers(true);
}

void GameElementsGenerator::on_offeredAnswer3Button_released()
{
	uint16_t inputAnswer = ui.offeredAnswer3Button->text().toInt();
	uint16_t currentAnswer =  m_singleChoiceQuestion.GetAnswer();

	SubmitSingleChoiceAnswer();
	DisableAdvantageOfferedAnswers(true);
}

void GameElementsGenerator::on_offeredAnswer4Button_released()
{
	uint16_t inputAnswer = ui.offeredAnswer4Button->text().toInt();
	uint16_t currentAnswer =  m_singleChoiceQuestion.GetAnswer();

	SubmitSingleChoiceAnswer();
	DisableAdvantageOfferedAnswers(true);
}

void GameElementsGenerator::DisplayAnswerResult()
{ 
	if (m_inputPlayerAnswer == -1)
	{
		ui.errorLabel->setText("You have not chosen an answer!");
		ui.errorLabel->show();
	}
	else
	{
		int closedBy = m_singleChoiceQuestion.GetAnswer() - m_inputPlayerAnswer;

		std::stringstream ss;
		ss << "Correct answer is: " << m_singleChoiceQuestion.GetAnswer() << "\n"
			<< "close by: " << abs(closedBy) << "\n";

		if (abs(closedBy) == 0)
		{
			ui.displayAnswerVerdictSingleChoiceQuestionLabel->setText("<b><font color=\"green\">The answer is correct!</font></b>");
			ui.displayAnswerVerdictSingleChoiceQuestionLabel->show();
			m_canChooseTerritory = true;
			ui.chooseTerritoryLabel->setText("Now you can choose a territory!");
			ui.chooseTerritoryLabel->show();
		}
		else
		{
			ui.displayAnswerVerdictSingleChoiceQuestionLabel->setText("<b><font color=\"red\">The answer is wrong!</font></b>");
			ui.displayAnswerVerdictSingleChoiceQuestionLabel->show();
		}

		QString displayAnswer = QString::fromStdString(ss.str());
		ui.displayProximityCorrectAnswerLabel->setText(displayAnswer);
		ui.displayProximityCorrectAnswerLabel->show();
	}
}

void GameElementsGenerator::SubmitSingleChoiceAnswer()
{
	ui.submitAnswerButton->setDisabled(true);
	ui.inputAnswerLineEdit->setDisabled(true);

	if (m_inputPlayerAnswer != -1)
	{
		ui.elapsedTimeLabel->show();
		ui.elapsedTimeLabel->setText("Response time " + QString::number(elapsedTime.elapsed() / 1000) + "," + QString::number(elapsedTime.elapsed() % 1000) + " s");
	}
	
	std::string link = m_ip + "/setAnswerDetails/?roomID=" + std::to_string(m_roomID) + "&username=" + m_playerUsername + "&playerAnswer=" + std::to_string(m_inputPlayerAnswer) + "&responseTime=" + std::to_string(elapsedTime.elapsed());

	cpr::Response responseFromServer = cpr::Get(cpr::Url(link));

	if (responseFromServer.status_code != 200)
	{
		ui.errorLabel->setText("Player's response time could not be updated!");
		ui.errorLabel->show();
	}
}