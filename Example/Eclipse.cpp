/**
 * @file Eclipse.cpp
 * @author fugu133
 * @brief
 * @version 0.1
 * @date 2024-01-19
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <SatFind/Core>

using namespace satfind;

int main() {
	auto eclipse_dt = DateTime("1998-02-26T17:29:27"); // Total eclipse at 17:29:27 UTC on 26 February 1998
	auto s = SunPosition(eclipse_dt);
	auto m = MoonPosition(eclipse_dt);

	auto sp = s.eci().elements();
	auto spd = sp.normalized();
	auto mp = m.eci().elements();
	auto mpd = mp.normalized();
	auto elongation = Radian(std::acos(spd.dot(mpd)));

	std::cout << "Sun position: " << s.eci() << "\n";
	std::cout << "Moon position: " << m.eci() << "\n";
	std::cout << "Sun-Moon elongation: " << elongation << std::endl;
}