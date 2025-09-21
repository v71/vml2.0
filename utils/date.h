#pragma once

////////////////////////////////////////////////////////////////////////////////////
//	This source file is part of v71's engine
//
//	Copyright (c) 2011-2020 v71 
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in
//	all copies or substantial portions of the Software.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//	THE SOFTWARE.

namespace vml
{
	// -------------------------------------------------------------------

	class DateTime
	{

		private:

			// -------------------------------------------------------------
			// time span class for handling date computations

			class TimeSpan
			{

				private:

					int  Years;
					int  Months;
					int  Days;
					int  Hours;
					int  Minutes;
					int  Seconds;
					int  Milliseconds;

				public:

					static const int MAX_MILLISECONDS = 1000;
					static const int MAX_SECONDS = 60;
					static const int MAX_MINUTES = 60;
					static const int MAX_HOURS = 24;
					static const int MAX_DAYS_IN_MONTH = 31;
					static const int MONTHS_IN_YEAR = 12;
					static const int DAYS_IN_A_YEAR = 365;

					int GetMilliseconds() const { return Milliseconds; }
					int GetSeconds() const { return Seconds; }
					int GetMinutes() const { return Minutes; }
					int GetHours() const { return Hours; }
					int GetDays() const { return Days; }
					int GetMonths() const { return Months; }
					int GetYears() const { return Years; }
					void AddMilliseconds(const int& milliSeconds) { Milliseconds += milliSeconds; }
					void AddSeconds(const int& seconds) { Seconds += seconds; }
					void AddMinutes(const int& minutes) { Minutes += minutes; }
					void AddHours(const int& hours) { Hours += hours; }
					void AddDays(const int& days) { Days += days; }
					void AddMonths(const int& months) { Months += months; }
					void AddYears(const int& years)	{ Years += years; }

					bool operator<(const TimeSpan &ts) const
					{
						return	Years        < ts.GetYears() ||
								Months       < ts.GetMonths() ||
								Days         < ts.GetDays() ||
								Minutes      < ts.GetMinutes() ||
								Seconds      < ts.GetSeconds() ||
								Milliseconds < ts.GetMilliseconds();
					}

					bool operator>(const TimeSpan &ts) const
					{
						return	Years        > ts.GetYears() ||
								Months       > ts.GetMonths() ||
								Days         > ts.GetDays() ||
								Minutes      > ts.GetMinutes() ||
								Seconds      > ts.GetSeconds() ||
								Milliseconds > ts.GetMilliseconds();
					}

					bool operator<=(const TimeSpan &ts) const
					{
						return	Years <= ts.GetYears() ||
								Months <= ts.GetMonths() ||
								Days <= ts.GetDays() ||
								Minutes <= ts.GetMinutes() ||
								Seconds <= ts.GetSeconds() ||
								Milliseconds <= ts.GetMilliseconds();
					}

					bool operator>=(const TimeSpan &ts) const
					{
						return	Years >= ts.GetYears() ||
								Months >= ts.GetMonths() ||
								Days >= ts.GetDays() ||
								Minutes >= ts.GetMinutes() ||
								Seconds >= ts.GetSeconds() ||
								Milliseconds >= ts.GetMilliseconds();
					}

					bool operator==(const TimeSpan &ts) const
					{
						return	Years == ts.GetYears() ||
								Months == ts.GetMonths() ||
								Days == ts.GetDays() ||
								Minutes == ts.GetMinutes() ||
								Seconds == ts.GetSeconds() ||
								Milliseconds == ts.GetMilliseconds();
					}

					bool operator!=(const TimeSpan& ts) const
					{
						return	Years != ts.GetYears() ||
								Months != ts.GetMonths() ||
								Days != ts.GetDays() ||
								Minutes != ts.GetMinutes() ||
								Seconds != ts.GetSeconds() ||
								Milliseconds != ts.GetMilliseconds();
					}

					// -------------------------------------------------------------
					// ctor / dtor

					TimeSpan(const int& years,
							const int& months,
							const int& days,
							const int& hours,
							const int& minutes,
							const int& seconds,
							const int& milliseconds)
					{
						Years = years;
						Months = months;
						Days = days;
						Hours = hours;
						Minutes = minutes;
						Seconds = seconds;
						Milliseconds = milliseconds;
					}

					~TimeSpan()
					{

					}
			};

			// -----------------------------------------------------------------------

			static const int SINCE_YEAR = 1900;
			static const int MIN_MONTH = 1;
			static const int MAX_MONTH = 12;
			static const int MIN_DAY = 1;
			static const int MAX_DAY = 30;
			static const int MIN_WEEKDAY = 0;
			static const int MAX_WEEKDAY = 7;
			static const int MIN_HOUR = 0;
			static const int MAX_HOUR = 24;
			static const int MIN_MINUTE = 0;
			static const int MAX_MINUTE = 60;
			static const int MIN_SECOND = 0;
			static const int MAX_SECOND = 60;
			static const int MIN_MILLISECOND = 0;
			static const int MAX_MILLISECOND = 1000;

			// -----------------------------------------------------------------------

			std::string monthNames[12];
			std::string dayNames[7];
			std::string abbMonthNames[12];
			std::string abbDayNames[7];
			int DaysOfMonth[12];
			int Year;
			int Month;
			int Day;
			int Hour;
			int Minute;
			int Second;
			int Millisecond;
			bool Abbreviations;

			// -------------------------------------------------------------------------------------

			void Init(const int& day, int month, int year, int hour, int minute, int second, int millisecond)
			{
				InitMonthNames();
				InitDayNames();

				if (!IsValidMonth(month)) vml::CMessage::Error(L"Month %d is not in range", month);
				if (!IsValidDayOfMonth(day, month, year)) vml::CMessage::Error(L"Day %d is not in month %d's range", day, month);
				if (!IsValidHour(hour)) vml::CMessage::Error(L"Hour %d is not in valid range ( %d - &d )", hour, MIN_HOUR, MAX_HOUR);
				if (!IsValidMinute(minute)) vml::CMessage::Error(L"Minute %d is not in valid range ( %d - %d )", minute, MIN_MINUTE, MAX_MINUTE);
				if (!IsValidSecond(second)) vml::CMessage::Error(L"Second %d is not in valid range ( %d - %d )", second, MIN_SECOND, MAX_SECOND);
				if (!IsValidMillisecond(millisecond)) vml::CMessage::Error(L"Millisecond %d is not in valid range ( %d - %d )", millisecond, MIN_MILLISECOND, MAX_MILLISECOND);

				Day = day;
				Month = month;
				Year = year;
				Hour = hour;
				Minute = minute;
				Second = second;
				Millisecond = millisecond;
			}

			// -------------------------------------------------------------------------------------

			void InitMonthNames()
			{
				monthNames[0] = "January";
				monthNames[1] = "February";
				monthNames[2] = "March";
				monthNames[3] = "April";
				monthNames[4] = "May";
				monthNames[5] = "June";
				monthNames[6] = "July";
				monthNames[7] = "August";
				monthNames[8] = "September";
				monthNames[9] = "October";
				monthNames[10] = "November";
				monthNames[11] = "December";

				abbMonthNames[0] = "Jan";
				abbMonthNames[1] = "Feb";
				abbMonthNames[2] = "Mar";
				abbMonthNames[3] = "Apr";
				abbMonthNames[4] = "May";
				abbMonthNames[5] = "June";
				abbMonthNames[6] = "July";
				abbMonthNames[7] = "Aug";
				abbMonthNames[8] = "Sept";
				abbMonthNames[9] = "Oct";
				abbMonthNames[10] = "Nov";
				abbMonthNames[11] = "Dec";
			}

			// -------------------------------------------------------------------------------------

			void InitDayNames()
			{
				dayNames[0] = "Sunday";
				dayNames[1] = "Monday";
				dayNames[2] = "Tuesday";
				dayNames[3] = "Wednesday";
				dayNames[4] = "Thursday";
				dayNames[5] = "Friday";
				dayNames[6] = "Saturday";

				abbDayNames[0] = "Sun";
				abbDayNames[1] = "Mon";
				abbDayNames[2] = "Tue";
				abbDayNames[3] = "Wed";
				abbDayNames[4] = "Thu";
				abbDayNames[5] = "Fri";
				abbDayNames[6] = "Sat";
			}

			// -------------------------------------------------------------------------------------

			bool IsValidMillisecond(const int & millisecond)
			{
				return millisecond >= MIN_MILLISECOND && millisecond <= MAX_MILLISECOND;
			}

			// -------------------------------------------------------------------------------------

			bool IsValidSecond(const int& second)
			{
				return second >= MIN_SECOND && second <= MAX_SECOND;
			}

			// -------------------------------------------------------------------------------------

			bool IsValidHour(const int& hour)
			{
				return hour >= MIN_HOUR && hour <= MAX_HOUR;
			}

			// -------------------------------------------------------------------------------------

			bool IsValidMinute(const int& minute)
			{
				return minute >= MIN_MINUTE && minute <= MAX_MINUTE;
			}

			// -------------------------------------------------------------------------------------

			bool IsValidMonth(const int& month)
			{
				return month >= MIN_MONTH && month <= MAX_MONTH;
			}

			// -------------------------------------------------------------------------------------

			bool IsValidYear(const int& year)
			{
				return year >= 0;
			}

			// -------------------------------------------------------------------------------------

			bool IsValidDayOfMonth(const int& day, int month, const int& year)
			{
				if (IsValidMonth(month))
					return day >= 1 && day <= GetDaysOfMonth(month, year);
				return false;
			}

			// -------------------------------------------------------------------------------------

			bool IsValidWeekday(const int& day)
			{
				return day >= MIN_WEEKDAY && day <= MAX_WEEKDAY;
			}

			// -------------------------------------------------------------------------------------

			time_t GetTimestamp() const
			{
				time_t rawtime;
				tm tim;
				rawtime = time(nullptr);
				localtime_s(&tim, &rawtime);
				tim.tm_year = Year - SINCE_YEAR;
				tim.tm_mon = Month - 1;
				tim.tm_mday = Day;
				tim.tm_hour = Hour;
				tim.tm_min = Minute;
				tim.tm_sec = Second;
				return mktime(&tim);
			}

		public:

			// -------------------------------------------------------------------------------------
			// getters

			int GetYear() const { return Year; }
			int GetMonth() const { return Month; }
			int GetDay() const { return Day; }
			int GetHour() const { return Hour; }
			int GetMinute() const { return Minute; }
			int GetSecond() const { return Second; }
			int GetMillisecond() const { return Millisecond; }

			// -------------------------------------------------------------------------------------

			void Add(const TimeSpan& ts)
			{
				AddMilliseconds(ts.GetMilliseconds());
				AddSeconds(ts.GetSeconds());
				AddMinutes(ts.GetMinutes());
				AddHours(ts.GetHours());
				AddDays(ts.GetDays());
				AddMonths(ts.GetMonths());
				AddYears(ts.GetYears());
			}

			// -------------------------------------------------------------------------------------

			void Subtract(const TimeSpan& timeSpan)
			{
				SubtractMilliseconds(timeSpan.GetMilliseconds());
				SubtractSeconds(timeSpan.GetSeconds());
				SubtractMinutes(timeSpan.GetMinutes());
				SubtractHours(timeSpan.GetHours());
				SubtractDays(timeSpan.GetDays());
				SubtractMonths(timeSpan.GetMonths());
				SubtractYears(timeSpan.GetYears());
			}

			// -------------------------------------------------------------------------------------

			void AddYears(const int& years)
			{
				years > 0 ? Year += years : Year -=years;
			}

			// -------------------------------------------------------------------------------------

			void SubtractYears(const int& years)
			{
				Year -= years;
			}

			// -------------------------------------------------------------------------------------

			void AddMonths(const int& months)
			{
				auto totalMonths = Month + months;

				if (totalMonths > MAX_MONTH)
				{
					auto yearsToIncrease = totalMonths / MAX_MONTH;
					auto monthsToSubstract = yearsToIncrease * MAX_MONTH;
					AddYears(yearsToIncrease);
					Month = totalMonths - monthsToSubstract;
				}
				else Month += months;
			}

			// -------------------------------------------------------------------------------------

			void SubtractMonths(const int& months)
			{
				for (auto&& i = 0; i < months; i++)
				{
					Month--;

					if (Month < 1)
					{
						SubtractYears(1);
						Month = 12;
					}
				}
			}

			// -------------------------------------------------------------------------------------

			void AddDays(const int& days)
			{

				if (days < 0)
					SubtractDays(days);

				if (GetDaysOfMonth(Month, Year) >= Day + days)
				{
					Day += days;
					return;
				}

				auto totalDays = Day + days;
				auto month = Month;
				auto year = Year;

				for (;;)
				{
					auto daysInCurrentMonth = GetDaysOfMonth(month, year);

					if (totalDays > daysInCurrentMonth)
					{
						AddMonths(1);
						totalDays -= daysInCurrentMonth;
						if (month < 12)
						{
							month++;
						}
						else
						{
							month = 1;
							year++;
						}
					}
					else
					{
						break;
					}
				}
				Day = totalDays;
			}

			// -------------------------------------------------------------------------------------

			void SubtractDays(const int& days)
			{
				for (auto&& i = 0; i < days; i++)
				{
					Day--;

					if (Day < 1)
					{
						SubtractMonths(1);
						Day = GetDaysOfMonth(Month, Year);
					}
				}
			}

			// -------------------------------------------------------------------------------------

			void AddHours(const int& hours)
			{
				auto totalHours = Hour + hours;

				if (totalHours >= MAX_HOUR)
				{
					auto daysToIncrease = totalHours / MAX_HOUR;
					auto hoursToSubstract = daysToIncrease * MAX_HOUR;

					AddDays(daysToIncrease);
					Hour = totalHours - hoursToSubstract;
				}
				else
					Hour += hours;
			}

			// -------------------------------------------------------------------------------------

			void SubtractHours(const int& hours)
			{
				for (auto&& i = 0; i < hours; i++)
				{
					Hour--;

					if (Hour < 1)
					{
						SubtractDays(1);
						Hour = MAX_HOUR;
					}
				}
			}

			// -------------------------------------------------------------------------------------

			void AddMinutes(const int& minutes)
			{
				auto totalMinutes = Minute + minutes;

				if (totalMinutes >= MAX_MINUTE)
				{
					auto hoursToIncrease = totalMinutes / MAX_MINUTE;
					auto minutesToSubstract = hoursToIncrease * MAX_MINUTE;

					AddHours(hoursToIncrease);
					Minute = totalMinutes - minutesToSubstract;
				}
				else
					Minute += minutes;
			}

			// -------------------------------------------------------------------------------------

			void SubtractMinutes(const int& minutes)
			{
				for (auto&& i = 0; i < minutes; i++)
				{
					Minute--;

					if (Minute < 1)
					{
						SubtractHours(1);
						Minute = MAX_MINUTE;
					}
				}
			}

			// -------------------------------------------------------------------------------------

			void AddSeconds(const int& seconds)
			{
				auto totalSeconds = Second + seconds;

				if (totalSeconds >= MAX_SECOND)
				{
					auto minutesToIncrease = totalSeconds / MAX_SECOND;
					auto secondsToSubtract = minutesToIncrease * MAX_SECOND;
					AddMinutes(minutesToIncrease);
					Second = totalSeconds - secondsToSubtract;
				}
				else
					Second += seconds;
			}

			// -------------------------------------------------------------------------------------

			void SubtractSeconds(const int& seconds)
			{
				for (auto&& i = 0; i < seconds; i++)
				{
					Second--;

					if (Second < 1)
					{
						SubtractMinutes(1);
						Second = MAX_SECOND;
					}
				}
			}

			// -------------------------------------------------------------------------------------

			void AddMilliseconds(const int& milliseconds)
			{
				auto totalMilliseconds = Millisecond + milliseconds;

				if (totalMilliseconds >= MAX_MILLISECOND)
				{
					auto secondsToIncrease = totalMilliseconds / MAX_MILLISECOND;
					auto milliSecondsToSubstract = secondsToIncrease * MAX_MILLISECOND;
					AddSeconds(secondsToIncrease);
					Millisecond = totalMilliseconds - milliSecondsToSubstract;
				}
				else Millisecond += milliseconds;
			}

			// -------------------------------------------------------------------------------------

			void SubtractMilliseconds(const int& milliseconds)
			{
				for (auto&& i = 0; i < milliseconds; i++)
				{
					Millisecond--;

					if (Millisecond < 1)
					{
						SubtractSeconds(1);
						Millisecond = MAX_MILLISECOND;
					}
				}
			}

			// -------------------------------------------------------------------------------------

			TimeSpan CalculateTimeSpan(DateTime& from, DateTime& to)
			{
				return to - from;
			}

			// -------------------------------------------------------------------------------------

			bool IsLeapYear()
			{
				return IsLeapYear(Year);
			}

			// -------------------------------------------------------------------------------------

			bool IsLeapYear(const int& year)
			{
				return ((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0);
			}

			// -------------------------------------------------------------------------------------

			int GetDayOfWeek()
			{
				return GetDayOfWeek(Year, Month, Day);
			}

			// -------------------------------------------------------------------------------------

			int GetDayOfWeek(int year, const int& month, const int& day)
			{
				const int monthTable[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
				year -= month < 3;
				return ((year + year / 4 - year / 100 + year / 400 + monthTable[month - 1] + day) % 7);
			}

			// -------------------------------------------------------------------------------------

			int GetDayOfYear()
			{
				return GetDayOfYear(Year, Month, Day);
			}

			// -------------------------------------------------------------------------------------

			int GetDayOfYear(const int& year, const int& month, const int& day)
			{
				int daysToMonth[2][13] =
				{
					{ 0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 },
					{ 0, 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335 },
				};

				return daysToMonth[IsLeapYear(year) ? 1 : 0][month] + day;
			}

			// -------------------------------------------------------------------------------------

			int GetDaysOfMonth()
			{
				return GetDaysOfMonth(Month, Year);
			}

			// -------------------------------------------------------------------------------------

			int GetDaysOfMonth(const int& month, const int& year)
			{
				const int MonthDays[2][13] = { { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
				{ 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 } };
				if (month < 1 || month > 12) return 0;
				return MonthDays[IsLeapYear(year)][month - 1];
			}

			// -------------------------------------------------------------------------------------

			std::string GetNameOfDay()
			{
				return GetNameOfDay(GetDayOfWeek());
			}

			// -------------------------------------------------------------------------------------

			std::string GetNameOfDay(const int & day)
			{
				if (IsValidWeekday(day))
				{
					if (Abbreviations) return abbDayNames[day];
					else return dayNames[day];
				}
				else vml::CMessage::Error(L"Day %d is not in valid weekday range ( %d - %d )", day, MIN_WEEKDAY, MAX_WEEKDAY);
				return "";
			}

			// -------------------------------------------------------------------------------------

			bool operator<(const DateTime & dateTime) const
			{
				return GetTimestamp() < dateTime.GetTimestamp();
			}

			// -------------------------------------------------------------------------------------

			bool operator>(const DateTime &dateTime) const
			{
				return GetTimestamp() > dateTime.GetTimestamp();
			}

			// -------------------------------------------------------------------------------------

			bool operator<=(const DateTime &dateTime) const
			{
				return GetTimestamp() <= dateTime.GetTimestamp();
			}

			// -------------------------------------------------------------------------------------

			bool operator>=(const DateTime &dateTime) const
			{
				return GetTimestamp() >= dateTime.GetTimestamp();
			}

			// -------------------------------------------------------------------------------------

			bool operator==(const DateTime &dateTime) const
			{
				return GetTimestamp() == dateTime.GetTimestamp();
			}

			// -------------------------------------------------------------------------------------

			bool operator!=(const DateTime& dateTime) const
			{
				return GetTimestamp() != dateTime.GetTimestamp();
			}

			// -------------------------------------------------------------------------------------

			DateTime operator+(const TimeSpan &timeSpan)
			{
				auto timeStmp = this;
				timeStmp->Add(timeSpan);
				return *timeStmp;
			}

			// -------------------------------------------------------------------------------------

			DateTime operator-(const TimeSpan& timeSpan)
			{
				auto timeStmp = this;
				timeStmp->Subtract(timeSpan);
				return *timeStmp;
			}

			// -------------------------------------------------------------------------------------
			// gets current date

			void Now()
			{
				time_t timeSpan;
				tm curTime;
				time(&timeSpan);
				localtime_s(&curTime, &timeSpan);
				Init(curTime.tm_mday,
					curTime.tm_mon + 1,
					curTime.tm_year + SINCE_YEAR,
					curTime.tm_hour,
					curTime.tm_min,
					curTime.tm_sec,
					0);
			}

			// -------------------------------------------------------------------------------------

			TimeSpan operator-(const DateTime& dateTime)
			{
				auto milliSeconds = dateTime.GetMillisecond();
				auto seconds = dateTime.GetSecond();
				auto minutes = dateTime.GetMinute();
				auto hours = dateTime.GetHour();
				auto days = dateTime.GetDay();
				auto months = dateTime.GetMonth();
				auto years = dateTime.GetYear();

				auto totalMilliseconds = 0,
					totalSeconds = 0,
					totalMinutes = 0,
					totalHours = 0,
					totalDays = 0,
					totalMonths = 0,
					totalYears = 0,
					leapYears = 0;

				const int DaysInYear[] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };

				for (auto i = years; i < Year; i++)
				{
					if (IsLeapYear(i))
						leapYears++;
				}

				auto difference = ((Year - 1000) * 365 + DaysInYear[Month - 1] + Day) -
					((years - 1000) * 365 + DaysInYear[months - 1] + days);

				difference += leapYears;

				auto monthCounter = months;
				auto yearCounter = years;

				for (;;)
				{
					auto daysInCurrentMonth = GetDaysOfMonth(monthCounter, yearCounter);

					if (difference > daysInCurrentMonth)
					{
						totalMonths++;

						difference -= daysInCurrentMonth;

						if (monthCounter < 12)
						{
							monthCounter++;
						}
						else
						{
							monthCounter = 1;
							yearCounter++;
						}
					}
					else
					{
						break;
					}
				}

				totalYears = totalMonths / MAX_MONTH;
				totalDays = difference;
				totalMonths = totalMonths - (MAX_MONTH * totalYears);

				totalHours = Hour - hours;
				totalMinutes = Minute - minutes;
				totalSeconds = Second - seconds;
				totalMilliseconds = Millisecond - milliSeconds;

				if (totalMilliseconds < 0)
				{
					totalSeconds--;
					totalMilliseconds += 1000;
				}

				if (totalSeconds < 0)
				{
					totalMinutes--;
					totalSeconds += 60;
				}

				if (totalMinutes < 0 && totalHours > 0 && totalDays > 0)
				{
					totalHours--;
					totalMinutes += 60;
				}

				if (totalHours < 0 && totalDays > 0)
				{
					totalDays--;
					totalHours += 24;
				}

				return TimeSpan(totalYears, totalMonths, totalDays, totalHours, totalMinutes, totalSeconds, totalMilliseconds);
			}

			// -------------------------------------------------------------------------------------

			void operator+=(const TimeSpan &timeSpan)
			{
				Add(timeSpan);
			}

			// -------------------------------------------------------------------------------------

			void operator-=(const TimeSpan &timeSpan)
			{
				Subtract(timeSpan);
			}

			// -------------------------------------------------------------------------------------

			std::string GetNameOfMonth(const int& month)
			{
				if (IsValidMonth(month))
				{
					if (Abbreviations) return abbMonthNames[month - 1];
					else return monthNames[month - 1];
				}
				else vml::CMessage::Error(L"Month %d is not in valid range ( %d - %d )", month, MIN_MONTH, MAX_MONTH);
				return "";
			}

			// -------------------------------------------------------------------------------------

			std::string GetNameOfMonth()
			{
				if (Abbreviations) return abbMonthNames[Month - 1];
				return monthNames[Month - 1];
			}

			// -------------------------------------------------------------------------------------

			std::string GetAsString()
			{
				return vml::strings::StringFormat::Text("%s - %s", GetShortDateString().c_str(), GetShortTimeString().c_str());
			}

			// -------------------------------------------------------------------------------------

			std::string GetShortTimeString()
			{
				return vml::strings::StringFormat::Text("%d:%d:%d", Hour, Minute, Second);
			}

			// -------------------------------------------------------------------------------------

			std::string GetLongTimeString()
			{
				return vml::strings::StringFormat::Text("%d:%d:%d:%d", Hour, Minute, Second, Millisecond);
			}

			// -------------------------------------------------------------------------------------

			std::string GetShortDateString()
			{
				return vml::strings::StringFormat::Text("%d/%d/%d", Month, Day, Year);
			}

			// -------------------------------------------------------------------------------------

			std::string GetLongDateString()
			{
				return vml::strings::StringFormat::Text("%d:%d:%d - %s %s %d",
													  Hour, Minute,Second,
													  GetNameOfDay(GetDayOfWeek()).c_str(),
													  GetNameOfMonth(Month).c_str(),Year);
			}

			// ----------------------------------------------------------------------
			// ctor / dtor

			DateTime()
			{
				Day = 0;
				Month = 0;
				Year = 0;
				Hour = 0;
				Minute = 0;
				Second = 0;
				Millisecond = 0;
				Abbreviations = true;
				InitMonthNames();
				InitDayNames();
			}

			DateTime(int day, int month, int year)
			{
				Init(month, day, year, 0, 0, 0, 0);
			}

			DateTime(int day, int month, int year, int hour, int minute, int second)
			{
				Init(month, day, year, hour, minute, second, 0);
			}

			DateTime(int day, int month, int year, int hour, int minute, int second, int millisecond)
			{
				Init(month, day, year, hour, minute, second, millisecond);
			}

			~DateTime()
			{
			}

	};

} // end of vml namespace
