#include "Chrono.h"

namespace Chrono {

	std::string monthStr[] = { "Gennaio", "Febbraio", "Marzo", "Aprile", "Maggio", "Giugno", "Luglio", "Agosto", "Settembre", "Ottobre", "Novembre", "Dicembre" };
	std::string weekStr[] = { "Lunedì", "Martedì", "Mercoledì", "Giovedì", "Venerdì", "Sabato", "Domenica" };

	// Constructor
	Date::Date(int yy, int mm, int dd)
		: m_year{yy}, m_month{Month(mm)}, m_day{dd}, m_weekDay{WeekDay(1)}
	{
		if (!isDate(m_year, m_month, m_day))
		{
			std::cerr << "Invalid date\n";
			exit(1);
		}

		m_weekDay = dayOfWeek(*this);
	}

	Date::Date()
		: m_year{2001}, m_month{Month::jan}, m_day{1}, m_weekDay{ WeekDay(1) }
	{ m_weekDay = dayOfWeek(*this); }

	void Date::AddDay(int n)
	{
		if (n < 0)
		{
			std::cerr << "Invalid numbero of days\n";
			exit(1);
		}

		while (n > 0)
		{
			int daysCount = MonthDays();
			if (m_day + n > daysCount)
			{
				n -= daysCount - m_day + 1;
				m_day = 1;

				if (m_month == Month::dec)
				{
					m_month = Month::jan;
					m_year++;
				}
				else
				{
					m_month = Month(int(m_month) + 1);
				}
			}
			else
			{
				m_day += n;
				n = 0;
			}
		}
	}

	void Date::AddMonth(int n)
	{
		for (int i = 0; i < n; i++)
		{
			if (m_month == Month::dec)
			{
				m_month = Month::jan;
				m_year++;
			}
			else
				m_month = Month(int(m_month) + 1);
		}
	}

	void Date::AddYear(int n)
	{
		if (m_month == Month::feb && m_day == 29 && !leapYear(m_year + n)) // beware of leap years!
		{
			m_month = Month::mar; // use March 1 instead of February 29
			m_day = 1;
		}
		m_year += n;
	}

	int Date::MonthDays() const
	{
		int d{};
		switch (m_month)
		{
		case Month::apr: case Month::jun: case Month::sep: case Month::nov:
			d = 30;
			break;

		case Month::jan: case Month::mar: case Month::may: case Month::jul: case Month::aug: case Month::oct: case Month::dec:
			d = 31;
			break;

		case Month::feb:
			d = (leapYear(m_year)) ? 29 : 28;
			break;
		}

		return d;
	}

	std::string Date::GetMonthStr() const { return monthStr[(int)m_month - 1]; };
	std::string Date::GetWeekDayStr() const { return weekStr[(int)m_weekDay - 1]; };

	bool isDate(int y, Month m, int d)
	{
		// assume that y is valid
		if (d <= 0) return false; // d must be positive
		if (m < Month::jan || Month::dec < m) return false;

		int days_in_month = 31; // most months have 31 days
		switch (m)
		{
		case Month::feb: // the length of February varies
			days_in_month = (leapYear(y)) ? 29 : 28;
			break;

		case Month::apr: case Month::jun: case Month::sep: case Month::nov:
			days_in_month = 30; // the rest have 30 days
			break;
		}
		if (days_in_month < d) return false;
		return true;
	}

	bool leapYear(int y)
	{
		if (y % 400 == 0 || (y % 4 == 0 && !(y % 100 == 0)))
			return true;

		return false;
	}

	WeekDay dayOfWeek(const Date& d)
	{
		// Algoritmo di Sakamoto
		// t[] è la tabella degli offset per i mesi
		static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };

		int y = d.GetYear();
		// Convertiamo l'enum Month in int per i calcoli
		int m = int(d.GetMonth());
		int day = d.GetDay();

		// Se il mese è Gennaio o Febbraio, l'algoritmo calcola come se fosse l'anno prima
		if (m < 3)
			y -= 1;

		/* Calcolo indice giorno (Sakamoto):
		   Risultato standard: 0 = Domenica, 1 = Lunedì, ..., 6 = Sabato
		*/
		int result = (y + y / 4 - y / 100 + y / 400 + t[m - 1] + day) % 7;

		// Se il risultato è 0 (Domenica), restituiamo WeekDay::sunday (che è 7 o l'ultimo enum)
		if (result == 0)
			return WeekDay::sunday;

		// Altrimenti 1 corrisponde a Monday, 2 a Tuesday, ecc.
		return WeekDay(result);
	}

	//operator overloading-------------------------------------------------------------------------------
	bool operator==(const Date& a, const Date& b)
	{
		return a.GetYear() == b.GetYear()
			&& a.GetMonth() == b.GetMonth()
			&& a.GetDay() == b.GetDay();
	}

	bool operator!=(const Date& a, const Date& b) { return !(a == b); }
}