#include "QuestionSingleChoice.h"

QuestionSingleChoice::QuestionSingleChoice()
{
	m_questionText = "None";
	m_answer = 0;
}

QuestionSingleChoice::QuestionSingleChoice(const std::string& question, uint16_t answer)
	: m_questionText(question),
	m_answer(answer)
{

}

QuestionSingleChoice::QuestionSingleChoice(const QuestionSingleChoice& object)
{
	*this = object;
}

QuestionSingleChoice::QuestionSingleChoice(QuestionSingleChoice&& object)
{
	*this = std::move(object);
}

const std::string& QuestionSingleChoice::GetQuestionText() const
{
	return m_questionText;
}

const uint16_t& QuestionSingleChoice::GetAnswer() const
{
	return m_answer;
}

bool QuestionSingleChoice::operator==(const QuestionSingleChoice& object)
{
	if (this->m_answer == object.m_answer && this->m_questionText == object.m_questionText)
		return true;

	return false;
}

QuestionSingleChoice& QuestionSingleChoice::operator=(const QuestionSingleChoice& object)
{
	m_questionText = object.m_questionText;
	m_answer = object.m_answer;

	return *this;
}

QuestionSingleChoice& QuestionSingleChoice::operator=(QuestionSingleChoice&& object)
{
	m_questionText = object.m_questionText;
	m_answer = object.m_answer;

	new(&object)QuestionSingleChoice;

	return *this;
}

std::ostream& operator<<(std::ostream& out, const QuestionSingleChoice& qsc)
{
	out << qsc.m_questionText << std::endl;

	return out;
}
