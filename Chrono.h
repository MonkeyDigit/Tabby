#pragma once
#include <iostream>

namespace Chrono {
	enum class Month { jan = 1, feb, mar, apr, may, jun, jul, aug, sep, oct, nov, dec };
	enum class WeekDay { monday = 1, tuesday, wednesday, thursday, friday, saturday, sunday };

	class Date {
	public:
		Date(int yy, int mm, int dd); // check for valid date and initialize
		Date(); // default constructor

		// Modifying operations
		void AddDay(int n);
		void AddMonth(int n);
		void AddYear(int n);
		// Getter
		int GetDay() const { return m_day; };
		Month GetMonth() const { return m_month; };
		int GetYear() const { return m_year; };
		WeekDay GetWeekDay() const { return m_weekDay; };
		std::string GetMonthStr() const;
		std::string GetWeekDayStr() const;
		int MonthDays() const;

	private:
		int m_year;
		Month m_month;
		int m_day;
		WeekDay m_weekDay;
	};

	// HELPER FUNCTIONS
	bool isDate(int y, Month m, int d);
	bool leapYear(int y);
	WeekDay dayOfWeek(const Date& d);
	//Operator overloading
	bool operator>(const Date& a, const Date& b);
	bool operator<(const Date& a, const Date& b);
	bool operator==(const Date& a, const Date& b);
	bool operator!=(const Date& a, const Date& b);
}