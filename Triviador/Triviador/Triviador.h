#pragma once

#include <QtWidgets/QMainWindow>
#include<array>
#include "ui_Triviador.h"
#include "Questions.h"

class Triviador : public QMainWindow
{
    Q_OBJECT

public:
    Triviador(QWidget *parent = nullptr);
    ~Triviador();

private:
    void SaveSingleChoiceQuestionsToFile(const QString fileName);
    void SaveMultipleChoiceQuestionsToFile(const QString fileName);

    void CheckMultipleChoiceAnswer(const QString chosenAnswer, bool& answer);

private slots:
    void on_saveQuestionsInFileButton_released();
    void on_getRandomQuestionButton_released();

    void on_submitAnswerButton_released();

    void on_multipleChoiceAnswer1Button_released();
    void on_multipleChoiceAnswer2Button_released();
    void on_multipleChoiceAnswer3Button_released();
    void on_multipleChoiceAnswer4Button_released();

private:
    bool m_answerHasBeenSelected;

    Questions m_questions;
    std::string m_currentAnswer;

    Ui::TriviadorClass ui;
};
