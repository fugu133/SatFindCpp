/**
 * @file Eclipse.cpp
 * @author fugu133
 * @brief 皆既日食時の太陽と月の位置関係を計算する
 * @note 2024年4月08日に発生する皆既日食時刻を参考にしている
 * @remark 本来は食分を評価する必要がある
 * @version 0.1
 * @date 2024-01-19
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <SatFind/Core>
#include <fstream>

using namespace satfind;

int main() {
	const auto eclipse_first_dt = DateTime("2024-04-08T15:42:15");
	const auto eclipse_last_dt = DateTime("2024-04-08T20:52:19");
	const auto delta_t = Seconds(0);
	auto ofs = std::ofstream{"TotalEclipse2024.csv"};
	auto min_sun_moon_elongation = Degree{180};
	auto max_eclipse_dt = DateTime{};

	ofs << "Date Time,Sun ECI X [m],Sun ECI Y,Sun ECI Z,Moon ECI X [m],Moon ECI Y,Moon ECI Z,Sun-Moon Elongation [deg]" << std::endl;
	for (auto dt = eclipse_first_dt; dt < eclipse_last_dt; dt += Seconds(1)) {
		auto s = SunPosition(dt, delta_t);
		auto m = MoonPosition(dt, delta_t);

		auto sp = s.eci().elements();
		auto spd = sp.normalized();
		auto mp = m.eci().elements();
		auto mpd = mp.normalized();
		auto elongation = Radian(std::acos(spd.dot(mpd)));

		if (elongation.degrees() < min_sun_moon_elongation.degrees()) {
			min_sun_moon_elongation = Degree{elongation.degrees()};
			max_eclipse_dt = dt;
		}

		ofs << dt << "," << s.eci().x() << "," << s.eci().y() << "," << s.eci().z() << "," << m.eci().x() << "," << m.eci().y() << ","
			<< m.eci().z() << "," << elongation.degrees() << std::endl;
	}

	std::cout << "Maximum Eclipse: " << max_eclipse_dt << std::endl;
}