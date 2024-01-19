/**
 * @file IssOrbit.cpp
 * @author fugu133
 * @brief
 * @version 0.1
 * @date 2024-01-19
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <SatFind/Core>
#include <filesystem>
#include <fstream>

using namespace satfind;
namespace fs = std::filesystem;

const auto tle_path = "ISS.tle";
const auto out_path = "ISS.csv";
const auto start_dt = DateTime::now();
const auto end_dt = start_dt + Days(1);

int main() {
	if (!fs::exists(tle_path)) {
		std::cerr << "ISS.tle not found" << std::endl;
		std::cerr << "Please download ISS TLE from https://celestrak.org/NORAD/elements/gp.php?CATNR=25544" << std::endl;
		std::cerr << "Or run GetIssTle.py" << std::endl;
	} else {
		auto ifs = std::ifstream{tle_path};
		auto ofs = std::ofstream{out_path};
		auto op = OrbitalPropagator(ifs);

		ofs << "Date Time,Elapsed Time [s],Longitude [deg],Latitude [deg],Altitude [m]" << std::endl;
		for (auto dt = start_dt; dt < end_dt; dt += Minutes(1)) {
			auto elem = op.trackFlightObject(dt);
			auto pos = elem.position.toWgs84();
			ofs << dt << "," << (dt - start_dt).totalSeconds() << "," << pos.longitude().degrees() << "," << pos.latitude().degrees() << ","
				<< pos.altitude() << std::endl;
		}
	}
}