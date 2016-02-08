/*
 * Plausi.cpp
 *
 */

#include <string>
#include <deque>
#include <utility>
#include <ctime>
#include <cstdlib>

#include <log4cpp/Category.hh>
#include <log4cpp/Priority.hh>

#include "Plausi.h"

Plausi::Plausi(int window) :
        _window(window), _value(-1.), _time(0) {
}

bool Plausi::check(const std::string& value, time_t time) {
    log4cpp::Category& rlog = log4cpp::Category::getRoot();
    if (rlog.isInfoEnabled()) {
        rlog.info("Plausi check: %s of %s", value.c_str(), ctime(&time));
    }

   if (value.length() != 6) {
        // exactly 7 digits
    rlog.info("Plausi rejected: exactly 7 digits");
    return false;
    }
    if (value.find_first_of('?') != std::string::npos) {
        // no '?' char
        rlog.info("Plausi rejected: no '?' char");
        return false;
    }

    double dval = atof(value.c_str()) / 10.;
    _queue.push_back(std::make_pair(time, dval));

    if (_queue.size() < _window) {
        rlog.info("Plausi rejected: not enough values: %d", _queue.size());
        return false;
    }
    if (_queue.size() > _window) {
        _queue.pop_front();
    }

    time_t candTime = _queue.at(_window/2).first;
    double candValue = _queue.at(_window/2).second;
    if (candValue < _value) {
        rlog.info("Plausi rejected: value must be >= previous checked value");
        return false;
    }
    // everything is OK -> use the candidate value
    _time = candTime;
    _value = candValue;
    if (rlog.isInfoEnabled()) {
        rlog.info("Plausi accepted: %.1f of %s", _value, ctime(&_time));
    }
    return true;
}

double Plausi::getCheckedValue() {
    return _value;
}

time_t Plausi::getCheckedTime() {
    return _time;
}

std::string Plausi::queueAsString() {
    std::string str;
    char buf[20];
    str += "[";
    std::deque<std::pair<time_t, double> >::const_iterator it = _queue.begin();
    for (; it != _queue.end(); ++it) {
        sprintf(buf, "%.1f", it->second);
        str += buf;
        str += ", ";
    }
    str += "]";
    return str;
}


