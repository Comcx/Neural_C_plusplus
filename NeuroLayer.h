#pragma once

#include "lainlib.h"
#include "monika.h"

template<class T, int neuro_num, int tentacle_num>
class NeuroLayer {

private:
	int _neuro_num;
	int _tentacle_num;
	array_(neuro_num, 1) (*_transfer)(array_(neuro_num, 1));
	NeuroTime<T, neuro_num, tentacle_num> _time;

	array_(neuro_num, tentacle_num) _w;
	array_(neuro_num, 1) _b;

	friend std::deque<double> load_data_from(std::string path);

public:
	NeuroLayer() {}
	NeuroLayer(array_(neuro_num, 1)(*transfer)(array_(neuro_num, 1)));
	NeuroLayer(array_(neuro_num, 1)(*transfer)(array_(neuro_num, 1)), std::string path_to_load);

	bool save_data_to(std::string path);

	array_(neuro_num, tentacle_num) w() { return this->_w; }
	array_(neuro_num, 1) b() { return this->_b; }
	Array<int, 2, 1> size();

	array_(neuro_num, 1) act(array_(tentacle_num, 1) x);
	void forget();
};


template<class T, int neuro_num, int tentacle_num>
NeuroLayer<T, neuro_num, tentacle_num>::NeuroLayer(
	array_(neuro_num, 1)(*transfer)(array_(neuro_num, 1))) {
	this->_neuro_num = neuro_num;
	this->_tentacle_num = tentacle_num;
	this->_transfer = transfer;
	
	srand((unsigned)time(NULL));
	this->_w = array_(neuro_num, tentacle_num)::Random(neuro_num, tentacle_num);
	this->_b = array_(neuro_num, 1)::Random(neuro_num, 1);
	this->_time = NeuroTime<T, neuro_num, tentacle_num>(this->_w);
}

template<class T, int neuro_num, int tentacle_num>
NeuroLayer<T, neuro_num, tentacle_num>::NeuroLayer(
	array_(neuro_num, 1)(*transfer)(array_(neuro_num, 1)), std::string path_to_load) {
	this->_neuro_num = neuro_num;
	this->_tentacle_num = tentacle_num;
	this->_transfer = transfer;
	
	std::deque<double> loaded_data = load_data_from(path_to_load);
	if (loaded_data.size() == neuro_num * tentacle_num + neuro_num) {
		for (int i = 0; i < this->_w.size(); i++) {
			this->_w(i/tentacle_num, i%tentacle_num) = loaded_data.front();
			loaded_data.pop_front();
		}

		for (int i = 0; i < this->_b.size(); i++) {
			this->_b(i) = loaded_data.front();
			loaded_data.pop_front();
		}
	} else {
		std::cerr << "-> The data you are loading is invaild!";
	}
	
	this->_time = NeuroTime<T, neuro_num, tentacle_num>(this->_w);
}

template<class T, int neuro_num, int tentacle_num>
Array<int, 2, 1> NeuroLayer<T, neuro_num, tentacle_num>::size() {
	Array<int, 2, 1> size;
	size << neuro_num, tentacle_num;
	return size;
}

template<class T, int neuro_num, int tentacle_num>
array_(neuro_num, 1) NeuroLayer<T, neuro_num, tentacle_num>::act(
	array_(tentacle_num, 1) x) {

	matrix_(neuro_num, 1) a = this->_w.matrix() * x.matrix();

	return this->_transfer(a.array() + this->_b);
}

template<class T, int neuro_num, int tentacle_num>
void NeuroLayer<T, neuro_num, tentacle_num>::forget() {
	this->_w = this->_time.refresh_intensity(this->_w);

}

std::deque<double> load_data_from(std::string path) {
	std::deque<double> loaded_data;
	std::ifstream file(path);

	std::string single_data;
	while (file >> single_data) {
		loaded_data.push_back(std::atof(single_data.c_str()));
	}

	file.close();
	return loaded_data;
}

template<class T, int neuro_num, int tentacle_num>
bool NeuroLayer<T, neuro_num, tentacle_num>::save_data_to(std::string path) {

	std::ofstream file(path);
	if (file.is_open()) {
		file << this->_w << "\n\r\n\r" << this->_b;
	} else {
		std::cerr << "-> Failed to save data to " + path;
	}

	file.close();
	return 0;
}





