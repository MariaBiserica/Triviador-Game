#pragma once

#include "QuestionSingleChoice.h"
#include "QuestionMultipleChoice.h"

#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <array>

class Questions
{
public:
	Questions();

	const std::vector<QuestionSingleChoice>& GetSingleChoiceQuestionsVector() const;
	const std::vector<QuestionMultipleChoice>& GetMultipleChoiceQuestionsVector() const;

public:
	void RemoveSingleChoiceQuestionByIndex(const uint8_t& index);
	void RemoveMultipleChoiceQuestionByIndex(const uint8_t& index);

	void RemoveSingleChoiceQuestionByObject(QuestionSingleChoice& object);
	void RemoveMultipleChoiceQuestionByObject(QuestionMultipleChoice& object);

	uint8_t GenerateRandomNumber(const uint8_t& size);

public:
	std::vector<QuestionSingleChoice> GetAFewRandomSingleChoiceQuestions(uint16_t amount);
	std::vector<QuestionMultipleChoice> GetAFewRandomMultipleChoiceQuestions(uint16_t amount);

	const QuestionSingleChoice& GetRandomSingleChoiceQuestion();
	const QuestionMultipleChoice& GetRandomMultipleChoiceQuestion();

public:
	void GetSingleChoiceQuestionsFromFile(const std::string& filePathSingleChoiceQuestions);
	void GetMultipleChoiceQuestionsFromFile(const std::string& filePathMultipleChoiceQuestions);

private:
	std::vector<QuestionSingleChoice> m_singleChoiceQuestions;
	std::vector<QuestionMultipleChoice> m_multipleChoiceQuestions;

	std::vector<QuestionSingleChoice> randomSingleChoiceQuestions;
	std::vector<QuestionMultipleChoice> randomMultipleChoiceQuestions;

};
