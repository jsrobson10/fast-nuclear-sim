/**
 * Copyright 2019 Bradley J. Snyder <snyder.bradleyj@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <iostream>
#include <cmath>
#include "pid.hpp"

using namespace std;
using namespace Sim::Util;

PID::PID(const Json::Value& node) :
    _max(node["max"].asDouble()),
    _min(node["min"].asDouble()),
    _Kp(node["Kp"].asDouble()),
    _Kd(node["Kd"].asDouble()),
    _Ki(node["Ki"].asDouble()),
    _pre_error(node["pre_error"].asDouble()),
    _integral(node["integral"].asDouble())
{
	
}

PID::operator Json::Value() const
{
	Json::Value node;

	node["max"] = _max;
	node["min"] = _min;
	node["Kp"] = _Kp;
	node["Ki"] = _Ki;
	node["Kd"] = _Kd;
	node["pre_error"] = _pre_error;
	node["integral"] = _integral;

	return node;
}


/**
 * Implementation
 */
PID::PID( double max, double min, double Kp, double Ki, double Kd ) :
    _max(max),
    _min(min),
    _Kp(Kp),
    _Kd(Kd),
    _Ki(Ki),
    _pre_error(0),
    _integral(0)
{
}

double PID::calculate( double dt, double setpoint, double pv )
{
    // Calculate error
    double error = setpoint - pv;

    // Proportional term
    double Pout = _Kp * error;

    // Integral term
    _integral += error * dt;
    double Iout = _Ki * _integral;

    // Derivative term
    double derivative = (error - _pre_error) / dt;
    double Dout = _Kd * derivative;

    // Calculate total output
    double output = Pout + Iout + Dout;

    // Restrict to max/min
    if( output > _max )
        output = _max;
    else if( output < _min )
        output = _min;

    // Save error to previous error
    _pre_error = error;

    return output;
}

