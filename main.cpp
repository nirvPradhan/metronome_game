#include <iostream>
#include <chrono>
#include <vector>
#include <Windows.h>

struct Duration {
	std::chrono::steady_clock::time_point start;
	std::chrono::milliseconds metronome;
	int s_bpm; // struct bpm
	std::vector<int> gaps;

	Duration(int bpm = 100)
		:start(std::chrono::steady_clock::now()),
		 metronome(std::chrono::milliseconds(bpm_to_milliseconds(bpm))),
		 s_bpm(bpm)
	{}

	int bpm_to_milliseconds(int bpm) {
		return bpm / 60 * 1000;
	}

	bool elapsed() {
		if (std::chrono::steady_clock::now() - start > metronome) {
			start = std::chrono::steady_clock::now();
			return true;
		}
		return false;
	}

	// appends the gap from a space bar press and the closest metronome elapsed time.
	void gap_from_metronome() {
		// half of the duration b/w metronome ticks.
		auto half_bpm = std::chrono::milliseconds(bpm_to_milliseconds(s_bpm / 2));
		auto now = std::chrono::steady_clock::now();

		// if its closer to the previous metronome tick 
		if (start + half_bpm > now)
			gaps.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count());
		
		// if its closer to the next beat calculate where it should be and subtract now
		gaps.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(start + half_bpm + half_bpm - now).count());
	}

	double average_gap() {
		int average = 0;

		std::vector<int>::iterator i;
		for (i = gaps.begin(); i < gaps.end(); i++)
			average += *i;

		return average / gaps.size();
	}




};

int main(){
	std::cout << "Hello World" << std::endl;
	
	Beep(750, 800);
	Duration duration;

	for (;;) {
		if (GetKeyState('Q') & 0x8000)
			break;

		if (GetKeyState(VK_SPACE) & 0x8000) {
			Beep(200, 100);
			duration.gap_from_metronome();
		}

		if (duration.elapsed())
			Beep(400, 150);
	}

	std::cout << "AVERAGE GAP WAS: " << duration.average_gap() << std::endl;
	return 0;
}
