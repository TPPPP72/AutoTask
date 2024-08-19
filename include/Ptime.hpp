#ifndef P_TIME_HPP
#define P_TIME_HPP

#include <iostream>
#include <algorithm>
#include <chrono>
#include <string>
#include <vector>

namespace Ptime
{

    tm *GetCurTM()
    {
        auto tm = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        return std::localtime(&tm);
    }

    class TimePoint
    {
    public:
        TimePoint() = default;
        ~TimePoint() = default;
        TimePoint(const tm *tm) : Tval(tm->tm_hour * 60 + tm->tm_min) {}
        TimePoint(uint32_t val) : Tval(val) {}
        TimePoint(const std::string &str)
        {
            uint32_t h, m;
            std::sscanf(str.c_str(), "%u:%u", &h, &m);
            Tval = h * 60 + m;
        }
        uint32_t Tval;
        friend bool operator==(const TimePoint &tp1, const TimePoint &tp2)
        {
            if (tp1.Tval == tp2.Tval)
                return true;
            return false;
        }
        friend bool operator!=(const TimePoint &tp1, const TimePoint &tp2)
        {
            if (tp1.Tval == tp2.Tval)
                return false;
            return true;
        }
        friend bool operator>=(const TimePoint &tp1, const TimePoint &tp2)
        {
            if (tp1.Tval >= tp2.Tval)
                return true;
            return false;
        }
        friend bool operator<=(const TimePoint &tp1, const TimePoint &tp2)
        {
            if (tp1.Tval <= tp2.Tval)
                return true;
            return false;
        }
        friend std::ostream &operator<<(std::ostream &output, const TimePoint &tp)
        {
            output << tp.Tval / 60 << ":" << tp.Tval % 60;
            return output;
        }
        std::string ToStr() const
        {
            return std::to_string(Tval / 60) + ":" + std::to_string(Tval % 60);
        }
        bool is_empty() const
        {
            if (Tval == 1441)
                return true;
            return false;
        }
        static TimePoint GetEmpty()
        {
            return 1441u;
        }
        static TimePoint GetCurrent()
        {
            return GetCurTM();
        }
    };

    class TimeSeg
    {
    public:
        TimeSeg() = default;
        ~TimeSeg() = default;
        TimeSeg(TimePoint beginval, TimePoint endval) : begin(beginval), end(endval) {}
        TimeSeg(const std::string &str)
        {
            std::string str1 = str.substr(0, str.find('-')), str2 = str.substr(str.find('-') + 1);
            begin = str1;
            end = str2;
        }
        TimePoint begin;
        TimePoint end;
        friend bool operator==(const TimeSeg &ts1, const TimeSeg &ts2)
        {
            if (ts1.begin == ts2.begin && ts1.end == ts2.end)
                return true;
            return false;
        }
        friend bool operator!=(const TimeSeg &ts1, const TimeSeg &ts2)
        {
            if (ts1.begin == ts2.begin && ts1.end == ts2.end)
                return false;
            return true;
        }
        friend std::ostream &operator<<(std::ostream &output, const TimeSeg &ts)
        {
            output << ts.begin << '-' << ts.end;
            return output;
        }
        std::string ToStr() const
        {
            return begin.ToStr() + '-' + end.ToStr();
        }
        bool is_empty() const
        {
            if (begin == TimePoint::GetEmpty() && end == TimePoint::GetEmpty())
                return true;
            return false;
        }
        bool contain(const TimePoint &tp) const
        {
            if (tp >= begin && tp <= end)
                return true;
            return false;
        }
        static TimeSeg GetEmpty()
        {
            return TimeSeg(TimePoint::GetEmpty(), TimePoint::GetEmpty());
        }
    };

    class Date
    {
    public:
        Date() = default;
        ~Date() = default;
        Date(const tm *tm) : year(tm->tm_year + 1900), month(tm->tm_mon + 1), day(tm->tm_mday) {}
        Date(uint32_t _year, uint32_t _month, uint32_t _day) : year(_year), month(_month), day(_day) {}
        Date(const std::string &str)
        {
            uint32_t y, m, d;
            std::sscanf(str.c_str(), "%u.%u.%u", &y, &m, &d);
            year = y;
            month = m;
            day = d;
        }
        uint32_t year;
        uint32_t month;
        uint32_t day;
        friend bool operator==(const Date &d1, const Date &d2)
        {
            if (d1.year == d2.year && d1.month == d2.month && d1.day == d2.day)
                return true;
            return false;
        }
        friend bool operator!=(const Date &d1, const Date &d2)
        {
            if (d1.year == d2.year && d1.month == d2.month && d1.day == d2.day)
                return false;
            return true;
        }
        friend std::ostream &operator<<(std::ostream &output, const Date &d)
        {
            output << d.year << '.' << d.month << '.' << d.day;
            return output;
        }
        bool is_empty() const
        {
            if (year == 0 && month == 0 && year == 0)
                return true;
            return false;
        }
        std::string ToStr() const
        {
            return std::to_string(year) + '.' + std::to_string(month) + '.' + std::to_string(day);
        }
        static Date GetEmpty()
        {
            return Date(0, 0, 0);
        }
        static Date GetCurrent()
        {
            return GetCurTM();
        }
    };

    class Wday
    {
    public:
        Wday() = default;
        ~Wday() = default;
        Wday(const tm *tm) : wday(tm->tm_wday) {}
        Wday(uint32_t _wday) : wday(_wday) {}
        Wday(const std::string &str)
        {
            std::vector<std::string> s = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
            wday = std::distance(s.begin(), std::find(s.begin(), s.end(), str));
        }
        enum
        {
            Sun,
            Mon,
            Tue,
            Wed,
            Thu,
            Fri,
            Sat
        };
        uint32_t wday;
        friend bool operator==(const Wday &wd1, const Wday &wd2)
        {
            if (wd1.wday == wd2.wday)
                return true;
            return false;
        }
        friend bool operator!=(const Wday &wd1, const Wday &wd2)
        {
            if (wd1.wday == wd2.wday)
                return false;
            return true;
        }
        friend std::ostream &operator<<(std::ostream &output, const Wday &wd)
        {
            std::string s[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
            output << s[wd.wday];
            return output;
        }
        std::string ToStr() const
        {
            std::string s[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
            return s[wday];
        }
        bool is_empty() const
        {
            if (wday == GetEmpty())
                return true;
            return false;
        }
        static Wday GetCurrent()
        {
            return GetCurTM();
        }
        static Wday GetEmpty()
        {
            return 7u;
        }
    };

};

#endif