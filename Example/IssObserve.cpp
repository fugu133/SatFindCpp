/**
 * @file IssObserve.cpp
 * @author Kaiji Takeuchi
 * @brief ISSの観測可能時間を計算する
 * @details 地上局 (JAXA 美笹深宇宙探査用地上局) のアンテナでISSと受信可能な時間を計算し，CSVファイルに出力する
 * @note アンテナは座標が公開されているものから適当なものを選んだので，実際にISSと交信してるのかは知らない
 * @remark 最大仰角に制限がある場合はパス時刻が途中で分割されることに注意
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

const auto mdss_position = Wgs84Position{Dms{138, 21, 8}, Dms{36, 8, 28}, 1612.75}; // 美笹深宇宙探査用地上局の位置
const auto mdss_max_elevation = Degree{80};											// 美笹深宇宙探査用地上局の最大仰角
const auto mdss_min_elevation = Degree{15};											// 美笹深宇宙探査用地上局の最小仰角
const auto tle_path = "ISS.tle";
const auto out_path = "ISS_MDSS_PASS.csv";

const auto start_dt = DateTime::now();
const auto end_dt = start_dt + Days(7);

int main() {
	if (!fs::exists(tle_path)) {
		std::cerr << "ISS.tle not found" << std::endl;
		std::cerr << "Please download ISS TLE from https://celestrak.org/NORAD/elements/gp.php?CATNR=25544" << std::endl;
		std::cerr << "Or run GetIssTle.py" << std::endl;
	} else {
		auto ifs = std::ifstream{tle_path};
		auto ofs = std::ofstream{out_path};
		auto op = OrbitalPropagator(ifs);
		auto gs = GroundObserver(mdss_position);
		auto is_aos_time = false;
		auto max_elevation = Angle::zero();
		auto aos_time = DateTime{};
		auto los_time = DateTime{};

		ofs << "AOS Date Time,LOS Date Time,Pass Time [s],Max Elevation [deg]" << std::endl;
		for (auto dt = start_dt; dt < end_dt; dt += Seconds(1)) {
			auto iss_pos = op.trackFlightObject(dt).position;
			auto view_dir = gs.lookUpPosition(iss_pos);

			if (view_dir.elevation() >= mdss_min_elevation && view_dir.elevation() <= mdss_max_elevation) {
				max_elevation = std::max(max_elevation, view_dir.elevation());
				if (!is_aos_time) {
					aos_time = dt;
					is_aos_time = true;
				}
			} else {
				if (is_aos_time) {
					los_time = dt;
					ofs << aos_time << "," << los_time << "," << (los_time - aos_time).totalSeconds() << "," << max_elevation.degrees()
						<< std::endl;
				}
				max_elevation = Angle::zero();
				is_aos_time = false;
			}
		}

		ifs.close();
		ofs.close();
	}
}
