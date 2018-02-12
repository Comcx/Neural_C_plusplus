#pragma once

#include "lainlib.h"
#include "NeuroLayer.h"

template<class T, int neuro_num, int tentacle_num>
class NeuroTime {

private:
	array_(neuro_num, tentacle_num) _time;
	array_(neuro_num, tentacle_num) _previous_w;
	int _learned_time;

public:
	NeuroTime(int time = 0) : _time(0), _learned_time(0) {}
	NeuroTime(array_(neuro_num, tentacle_num) w, int time = 0) : _time(0), _previous_w(w) {}

	array_(neuro_num, tentacle_num) refresh_intensity(array_(neuro_num, tentacle_num) w);
	void refresh_by_intensity(const array_(neuro_num, tentacle_num) w);
	array_(neuro_num, tentacle_num)& value() { return this->_time; }

};

template<class T, int neuro_num, int tentacle_num>
array_(neuro_num, tentacle_num) NeuroTime<T, neuro_num, tentacle_num>::refresh_intensity(
	array_(neuro_num, tentacle_num) w) {

	this->_time = this->_time + 1;

	bool learn_flag = true;
	array_(neuro_num, tentacle_num) learn_ratio;
	for (int i = 0; i < this->_previous_w.size(); i++) {
		if (this->_previous_w(i) == 0) learn_flag = false;
	}
	if (learn_flag == true) {
		learn_ratio = w / this->_previous_w;
	} else {
		learn_ratio = w - this->_previous_w;
	}
	array_(neuro_num, tentacle_num) pre_forget_ratio = 1 - 0.487 * (this->_time-1).pow(0.06);
	array_(neuro_num, tentacle_num) forget_ratio = 1 - 0.487 * this->_time.pow(0.06);
	
	this->_previous_w = w;
	return w * (forget_ratio/ pre_forget_ratio) * learn_ratio;
}

template<class T, int neuro_num, int tentacle_num>
void NeuroTime<T, neuro_num, tentacle_num>::refresh_by_intensity(
	const array_(neuro_num, tentacle_num) w) {



}





